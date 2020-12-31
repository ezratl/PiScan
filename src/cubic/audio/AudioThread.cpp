// Copyright (c) Charles J. Cliffe
// SPDX-License-Identifier: GPL-2.0+

#include "AudioThread.h"
#include "CubicSDRDefs.h"
#include <vector>
#include <algorithm>
#include "CubicSDR.h"
#include "DemodulatorThread.h"
#include "DemodulatorInstance.h"
#include <memory.h>
#include <mutex>

#include "loguru.hpp"
#include "threadname.h"
#include "PiScan.h"

#define AUDIO_THREAD_NAME   "Audio"

//50 ms
#define HEARTBEAT_CHECK_PERIOD_MICROS (50 * 1000) 

std::map<int, AudioThread* >  AudioThread::deviceController;

std::map<int, int> AudioThread::deviceSampleRate;

std::recursive_mutex AudioThread::m_device_mutex;

static AudioThread* controllerInstance = nullptr;

AudioThread* piscan::app::getAudioController() { return controllerInstance; }

AudioThread::AudioThread() : IOThread(), nBufferFrames(1024), sampleRate(0), controllerThread(nullptr) {

    audioQueuePtr = 0;
    underflowCount = 0;
    active.store(false);
    outputDevice.store(-1);
    gain = 1.0;
}

AudioThread::~AudioThread() {
    
    if (controllerThread != nullptr) {

        //
        //NOT PROTECTED by m_mutex on purpose, to prevent deadlocks with controllerThread
        // it doesn't matter, it is only called when all "normal" audio threads are detached from the controller.
        //

        terminate();
        controllerThread->join();
        delete controllerThread;
        controllerThread = nullptr;
    }
}

std::recursive_mutex & AudioThread::getMutex()
{
    return m_mutex;
}

void AudioThread::attachControllerThread(std::thread* controllerThread_in) {

    controllerThread = controllerThread_in;
}

void AudioThread::bindThread(AudioThread *other) {

    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    if (std::find(boundThreads.begin(), boundThreads.end(), other) == boundThreads.end()) {
        boundThreads.push_back(other);
    }
}

void AudioThread::removeThread(AudioThread *other) {

    //std::lock_guard<std::recursive_mutex> lock(m_mutex);

	std::unique_lock<std::recursive_mutex> lock(m_mutex, std::defer_lock);

	while(!lock.try_lock())
		std::this_thread::yield();

    auto i = std::find(boundThreads.begin(), boundThreads.end(), other);

    if (i != boundThreads.end()) {
        boundThreads.erase(i);
    }
}

void AudioThread::deviceCleanup() {
    //
    //NOT PROTECTED by m_device_mutex on purpose, to prevent deadlocks with i->second->controllerThread
    // it doesn't matter, it is only called when all "normal" audio threads are detached from the controller.
    // 
    for (auto i = deviceController.begin(); i != deviceController.end(); i++) {
        
        delete i->second;
    }

    deviceController.clear();
}

static int audioCallback(void *outputBuffer, void * /* inputBuffer */, unsigned int nBufferFrames, double /* streamTime */, RtAudioStreamStatus status,
    void *userData) {

    float *out = (float*)outputBuffer;

    //Zero output buffer in all cases: this allow to mute audio if no AudioThread data is 
    //actually active.
    ::memset(out, 0, nBufferFrames * 2 * sizeof(float));

    //src in the controller thread:
    AudioThread *src = (AudioThread *)userData;

    //by construction, src is a controller thread, from deviceController:
    std::lock_guard<std::recursive_mutex> lock(src->getMutex());

    if (src->isTerminated()) {
        return 1;
    }

    if (status) {
        //std::cout << "Audio buffer underflow.." << (src->underflowCount++) << std::endl << std::flush;
        LOG_F(WARNING, "Audio buffer underflow: %lu", src->underflowCount++);
    }

    double peak = 0.0;

    //Process the bound threads audio:
    for (size_t j = 0; j < src->boundThreads.size(); j++) {

        AudioThread *srcmix = src->boundThreads[j];

        //lock every single boundThread srcmix in succession the time we process 
        //its audio samples.
        //std::lock_guard<std::recursive_mutex> lock(srcmix->getMutex());

        if (srcmix->isTerminated() || !srcmix->inputQueue || srcmix->inputQueue->empty() || !srcmix->isActive()) {
            continue;
        }

        std::lock_guard<std::recursive_mutex> lock2(srcmix->getMutex());

        if (!srcmix->currentInput) {
            srcmix->audioQueuePtr = 0;

            if (!srcmix->inputQueue->try_pop(srcmix->currentInput)) {
                continue;
            }

            continue;
        }

        if (srcmix->currentInput->sampleRate != src->getSampleRate()) {

            while (srcmix->inputQueue->try_pop(srcmix->currentInput)) {

                if (srcmix->currentInput) {
                    if (srcmix->currentInput->sampleRate == src->getSampleRate()) {
                        break;
                    }

                }
                srcmix->currentInput = nullptr;
            } //end while

            srcmix->audioQueuePtr = 0;

            if (!srcmix->currentInput) {
                continue;
            }
        }


        if (srcmix->currentInput->channels == 0 || !srcmix->currentInput->data.size()) {
            if (!srcmix->inputQueue->empty()) {
                srcmix->audioQueuePtr = 0;
                if (srcmix->currentInput) {

                    srcmix->currentInput = nullptr;
                }

                if (!srcmix->inputQueue->try_pop(srcmix->currentInput)) {
                    continue;
                }
            }
            continue;
        }

        double mixPeak = srcmix->currentInput->peak * srcmix->gain;

        if (srcmix->currentInput->channels == 1) {

            for (unsigned int i = 0; i < nBufferFrames; i++) {

                if (srcmix->audioQueuePtr >= srcmix->currentInput->data.size()) {
                    srcmix->audioQueuePtr = 0;
                    if (srcmix->currentInput) {

                        srcmix->currentInput = nullptr;
                    }

                    if (!srcmix->inputQueue->try_pop(srcmix->currentInput)) {
                        break;
                    }


                    double srcPeak = srcmix->currentInput->peak * srcmix->gain;
                    if (mixPeak < srcPeak) {
                        mixPeak = srcPeak;
                    }
                }
                if (srcmix->currentInput && srcmix->currentInput->data.size()) {
                    float v = srcmix->currentInput->data[srcmix->audioQueuePtr] * srcmix->gain;
                    out[i * 2] += v;
                    out[i * 2 + 1] += v;
                }
                srcmix->audioQueuePtr++;
            }
        }
        else {
            for (int i = 0, iMax = srcmix->currentInput->channels * nBufferFrames; i < iMax; i++) {

                if (srcmix->audioQueuePtr >= srcmix->currentInput->data.size()) {
                    srcmix->audioQueuePtr = 0;
                    if (srcmix->currentInput) {

                        srcmix->currentInput = nullptr;
                    }

                    if (!srcmix->inputQueue->try_pop(srcmix->currentInput)) {
                        break;
                    }

                    double srcPeak = srcmix->currentInput->peak * srcmix->gain;
                    if (mixPeak < srcPeak) {
                        mixPeak = srcPeak;
                    }
                }
                if (srcmix->currentInput && srcmix->currentInput->data.size()) {

                    out[i] = out[i] + srcmix->currentInput->data[srcmix->audioQueuePtr] * srcmix->gain;
                }
                srcmix->audioQueuePtr++;
            }
        }

        peak += mixPeak;
    }

    //normalize volume
    if (peak > 1.0) {
        float invPeak = (float)(1.0 / peak);

        for (unsigned int i = 0; i < nBufferFrames * 2; i++) {
            out[i] *= invPeak;
        }
    }

    /* copy the output buffer to a batch for the RTSP server
     * probably not a great approach but for now the easiest way to get the mixed output to the stream */
    std::shared_ptr<std::vector<float>> batch = std::make_shared<std::vector<float>>();

    for (size_t i = 0; i < nBufferFrames * 2; i+=2)
    	batch->push_back(out[i]);

    src->outputQueue.try_enqueue(batch);

    return 0;
}

void AudioThread::enumerateDevices(std::vector<RtAudio::DeviceInfo> &devs) {
    RtAudio endac;

    int numDevices = endac.getDeviceCount();

    for (int i = 0; i < numDevices; i++) {
        RtAudio::DeviceInfo info = endac.getDeviceInfo(i);

        devs.push_back(info);

        //std::cout << std::endl;

        //std::cout << "Audio Device #" << i << " " << info.name << std::endl;
        LOG_F(1, "Audio device #%i - %s:", i, info.name.c_str());
        //std::cout << "\tDefault Output? " << (info.isDefaultOutput ? "Yes" : "No") << std::endl;
        RAW_LOG_F(1, "\tDefault output: %s", (info.isDefaultOutput ? "Yes" : "No"));
        //std::cout << "\tDefault Input? " << (info.isDefaultInput ? "Yes" : "No") << std::endl;
        RAW_LOG_F(1, "\tDefault Input: %s", (info.isDefaultInput ? "Yes" : "No"));
        //std::cout << "\tInput channels: " << info.inputChannels << std::endl;
        RAW_LOG_F(1, "\tInput channels: %u", info.inputChannels);
        //std::cout << "\tOutput channels: " << info.outputChannels << std::endl;
        RAW_LOG_F(1, "\tOutput channels: %u", info.outputChannels);
        //std::cout << "\tDuplex channels: " << info.duplexChannels << std::endl;
        RAW_LOG_F(1, "\tDuplex channels: %u", info.duplexChannels);

        //std::cout << "\t" << "Native formats:" << std::endl;
        RAW_LOG_F(1, "\tNative formats:");
        RtAudioFormat nFormats = info.nativeFormats;
        if (nFormats & RTAUDIO_SINT8) {
            RAW_LOG_F(1, "\t\t8-bit signed integer.");
        }
        if (nFormats & RTAUDIO_SINT16) {
            RAW_LOG_F(1, "\t\t16-bit signed integer.");
        }
        if (nFormats & RTAUDIO_SINT24) {
            RAW_LOG_F(1, "\t\t24-bit signed integer.");
        }
        if (nFormats & RTAUDIO_SINT32) {
            RAW_LOG_F(1, "\t\t32-bit signed integer.");
        }
        if (nFormats & RTAUDIO_FLOAT32) {
            RAW_LOG_F(1, "\t\t32-bit float normalized between plus/minus 1.0.");
        }
        if (nFormats & RTAUDIO_FLOAT64) {
            RAW_LOG_F(1, "\t\t64-bit float normalized between plus/minus 1.0.");
        }

        std::vector<unsigned int>::iterator srate;

        RAW_LOG_F(1, "\tSupported sample rates:");

        for (srate = info.sampleRates.begin(); srate != info.sampleRates.end(); srate++) {
            RAW_LOG_F(1, "\t\t%uhz", (*srate));
        }

        //std::cout << std::endl;
    }
}

void AudioThread::setDeviceSampleRate(int deviceId, int sampleRate) {

    AudioThread* matchingControllerThread = nullptr;

    //scope lock here to minimize the common unique static lock contention
    {
        std::lock_guard<std::recursive_mutex> lock(m_device_mutex);

        if (deviceController.find(deviceId) != deviceController.end()) {

            matchingControllerThread = deviceController[deviceId];
        }
    }

    //out-of-lock test
    if (matchingControllerThread != nullptr) {

        AudioThreadCommand refreshDevice;
        refreshDevice.cmd = AudioThreadCommand::AUDIO_THREAD_CMD_SET_SAMPLE_RATE;
        refreshDevice.int_value = sampleRate;
        //VSO : blocking push !
        matchingControllerThread->getCommandQueue()->push(refreshDevice);
    }
}

void AudioThread::setSampleRate(int sampleRate) {

    bool thisIsAController = false;

    //scope lock here to minimize the common unique static lock contention
    {
        std::lock_guard<std::recursive_mutex> lock(m_device_mutex);

        if (deviceController[outputDevice.load()] == this) {
            thisIsAController = true;
            deviceSampleRate[outputDevice.load()] = sampleRate;
        }
    }

    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    if (thisIsAController) {

        dac.stopStream();
        dac.closeStream();

        //Set bounded sample rate:
        for (size_t j = 0; j < boundThreads.size(); j++) {
            AudioThread *srcmix = boundThreads[j];
            srcmix->setSampleRate(sampleRate);  
        }

        //make a local copy, snapshot of the list of demodulators
        std::vector<DemodulatorInstancePtr> demodulators = wxGetApp().getDemodMgr().getDemodulators();

        for (auto demod : demodulators) {
            if (demod->getOutputDevice() == outputDevice.load()) {
                demod->setAudioSampleRate(sampleRate);
            }
        }

        dac.openStream(&parameters, NULL, RTAUDIO_FLOAT32, sampleRate, &nBufferFrames, &audioCallback, (void *)this, &opts);
        dac.startStream();
    }

    this->sampleRate = sampleRate;
}

int AudioThread::getSampleRate() {
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    return this->sampleRate;
}

void AudioThread::setupDevice(int deviceId) {

    //global lock to setup the device...
    std::lock_guard<std::recursive_mutex> lock(m_device_mutex);

    parameters.deviceId = deviceId;
    parameters.nChannels = 2;
    parameters.firstChannel = 0;

    opts.streamName = "CubicSDR Audio Output";

    try {
        if (deviceController.find(outputDevice.load()) != deviceController.end()) {
            //'this' is not the controller, so remove it from the bounded list:
            //beware, we must take the controller mutex, because the audio callback may use the list of bounded
            //threads at that moment:
            std::lock_guard<std::recursive_mutex> lock(deviceController[outputDevice.load()]->getMutex());

            deviceController[outputDevice.load()]->removeThread(this);
        }
#ifndef _MSC_VER
        opts.priority = sched_get_priority_max(SCHED_FIFO);
#endif
        //    opts.flags = RTAUDIO_MINIMIZE_LATENCY;
        opts.flags = RTAUDIO_SCHEDULE_REALTIME;

        if (deviceSampleRate.find(parameters.deviceId) != deviceSampleRate.end()) {
            sampleRate = deviceSampleRate[parameters.deviceId];
        }
        else {
            //std::cout << "Error, device sample rate wasn't initialized?" << std::endl;
            LOG_F(ERROR, "Error, device sample rate wasn't initialized?");
            return;
            //            sampleRate = AudioThread::getDefaultAudioSampleRate();
            //            deviceSampleRate[parameters.deviceId] = sampleRate;
        }

        //Create a new controller:
        if (deviceController.find(parameters.deviceId) == deviceController.end()) {

            //Create a new controller thread for parameters.deviceId:
            AudioThread* newController = new AudioThread();

            newController->setInitOutputDevice(parameters.deviceId, sampleRate);
            newController->bindThread(this);
            newController->attachControllerThread(new std::thread(&AudioThread::threadMain, newController));

            deviceController[parameters.deviceId] = newController;
        }
        else if (deviceController[parameters.deviceId] == this) {
        	// this may be problematic if more than one audio controller is active
        	controllerInstance = this;
        	outputQueue = moodycamel::ReaderWriterQueue<std::shared_ptr<std::vector<float>>>(4);
            //Attach callback
            dac.openStream(&parameters, NULL, RTAUDIO_FLOAT32, sampleRate, &nBufferFrames, &audioCallback, (void *)this, &opts);
            dac.startStream();
        }
        else {
            //we are a bound thread, add ourselves to the controller deviceController[parameters.deviceId].
            //beware, we must take the controller mutex, because the audio callback may use the list of bounded
            //threads at that moment:
            std::lock_guard<std::recursive_mutex> lock(deviceController[parameters.deviceId]->getMutex());

            deviceController[parameters.deviceId]->bindThread(this);
        }
        active = true;

    }
    catch (RtAudioError& e) {
        e.printMessage();
        return;
    }
    if (deviceId != -1) {
        outputDevice = deviceId;
    }
}

int AudioThread::getOutputDevice() {

    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    if (outputDevice == -1) {
        return dac.getDefaultOutputDevice();
    }
    return outputDevice;
}

void AudioThread::setInitOutputDevice(int deviceId, int sampleRate) {

    //global lock 
    std::lock_guard<std::recursive_mutex> lock(m_device_mutex);

    outputDevice = deviceId;
    if (sampleRate == -1) {
        if (deviceSampleRate.find(parameters.deviceId) != deviceSampleRate.end()) {
            sampleRate = deviceSampleRate[deviceId];
        }
    }
    else {
        deviceSampleRate[deviceId] = sampleRate;
    }
    this->sampleRate = sampleRate;
}

void AudioThread::run() {
#ifdef __APPLE__
    pthread_t tID = pthread_self();	 // ID of this thread
    int priority = sched_get_priority_max(SCHED_RR) - 1;
    sched_param prio = { priority }; // scheduling priority of thread
    pthread_setschedparam(tID, SCHED_RR, &prio);
#endif

    //    std::cout << "Audio thread initializing.." << std::endl;
    setThreadName(AUDIO_THREAD_NAME);
    LOG_F(INFO, "Audio thread initializing");

    if (dac.getDeviceCount() < 1) {
        //std::cout << "No audio devices found!" << std::endl;
        LOG_F(ERROR, "No audio devices found!");
        return;
    }

    setupDevice((outputDevice.load() == -1) ? (dac.getDefaultOutputDevice()) : outputDevice.load());

    //    std::cout << "Audio thread started." << std::endl;
    LOG_F(INFO, "Audio thread started");

    inputQueue = std::static_pointer_cast<AudioThreadInputQueue>(getInputQueue("AudioDataInput"));

    //Infinite loop, witing for commands or for termination
    while (!stopping) {
        AudioThreadCommand command;

        if (!cmdQueue.pop(command, HEARTBEAT_CHECK_PERIOD_MICROS)) {
            continue;
        }

        if (command.cmd == AudioThreadCommand::AUDIO_THREAD_CMD_SET_DEVICE) {
            setupDevice(command.int_value);
        }
        if (command.cmd == AudioThreadCommand::AUDIO_THREAD_CMD_SET_SAMPLE_RATE) {
            setSampleRate(command.int_value);
        }
    } //end while

    // Drain any remaining inputs, with a non-blocking pop
    if (inputQueue != nullptr) {
        inputQueue->flush();
    }

    //Nullify currentInput...
    currentInput = nullptr;

    //Stop : Retreive the matching controling thread in a scope lock:
    AudioThread* controllerThread = nullptr;
    {
        std::lock_guard<std::recursive_mutex> global_lock(m_device_mutex);
        controllerThread = deviceController[parameters.deviceId];
    }

    if (controllerThread != this) {
        //'this' is not the controller, so remove it from the bounded list:
        //beware, we must take the controller mutex, because the audio callback may use the list of bounded
        //threads at that moment:
        std::lock_guard<std::recursive_mutex> lock(controllerThread->getMutex());

        controllerThread->removeThread(this);
    }
    else {
        // 'this' is a controller thread:
        try {
            if (dac.isStreamOpen()) {
                dac.stopStream(); 
            }
            dac.closeStream();
        }
        catch (RtAudioError& e) {
            e.printMessage();
        }
    }

    //    std::cout << "Audio thread done." << std::endl;
    LOG_F(INFO, "Audio thread done");
}

void AudioThread::terminate() {
    IOThread::terminate();
}

bool AudioThread::isActive() {
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    return active;
}

void AudioThread::setActive(bool state) {

    AudioThread* matchingControllerThread = nullptr;

    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    //scope lock here to minimize the common unique static lock contention
    {
        std::lock_guard<std::recursive_mutex> lock(m_device_mutex);

        if (deviceController.find(parameters.deviceId) != deviceController.end()) {

            matchingControllerThread = deviceController[parameters.deviceId];
        }
    }

    if (matchingControllerThread == nullptr) {
        return;
    }

    if (state && !active && inputQueue) {
        matchingControllerThread->bindThread(this);
    }
    else if (!state && active) {
        matchingControllerThread->removeThread(this);
    }

    // Activity state changing, clear any inputs
    if (inputQueue) {
        inputQueue->flush();
    }
    active = state;
}

AudioThreadCommandQueue *AudioThread::getCommandQueue() {
    return &cmdQueue;
}

void AudioThread::setGain(float gain_in) {

    if (gain_in < 0.0) {
        gain_in = 0.0;
    }
    if (gain_in > 2.0) {
        gain_in = 2.0;
    }
    gain = gain_in;
}
