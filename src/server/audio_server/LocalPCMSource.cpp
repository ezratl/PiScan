/*
 * LocalPCMSource.cpp
 *
 *  Created on: May 3, 2020
 *      Author: ezra
 */

#include <cmath>
#include <cstring>

#include "LocalPCMSource.h"
#include "InputFile.hh"
#include "GroupsockHelper.hh"

#include "loguru.hpp"
#include "AudioThread.h"
#include "PiScan.h"

namespace piscan::server::audio {

LocalPCMSource*
LocalPCMSource::createNew(UsageEnvironment& env) {
  do {
	AudioThread* controller = app::getAudioController();
	if(!controller)
		break;

    LocalPCMSource* newSource = new LocalPCMSource(env, controller);
    if (newSource != NULL && newSource->bitsPerSample() == 0) {
      Medium::close(newSource);
      break;
    }

    return newSource;
  } while (0);

  return NULL;
}

LocalPCMSource::LocalPCMSource(UsageEnvironment& env, AudioThread* audioController)
  : AudioInputDevice(env, 0, 0, 0, 0)/* set the real parameters later */,
    fFidIsSeekable(False), fLastPlayTime(0), fHaveStartedReading(False), _audioController(audioController) {

  do {

    fNumChannels = 1;

    // sample rate needs to be grabbed from audio controller, as well as ouput samples
    fSamplingFrequency = audioController->getSampleRate();

    fBitsPerSample = 16; //using 16 bit signed

  } while (0);


  fPlayTimePerSample = 1e6/(double)fSamplingFrequency;

  // Although PCM is a sample-based format, we group samples into
  // 'frames' for efficient delivery to clients.  Set up our preferred
  // frame size to be close to 20 ms, if possible, but always no greater
  // than 1400 bytes (to ensure that it will fit in a single RTP packet)
  unsigned maxSamplesPerFrame = (1400*8)/(fNumChannels*fBitsPerSample);
  unsigned desiredSamplesPerFrame = (unsigned)(0.02*fSamplingFrequency);
  unsigned samplesPerFrame = desiredSamplesPerFrame < maxSamplesPerFrame ? desiredSamplesPerFrame : maxSamplesPerFrame;
  fPreferredFrameSize = (samplesPerFrame*fNumChannels*fBitsPerSample)/8;

	LOG_F(2, "Starting audio stream");
}

LocalPCMSource::~LocalPCMSource() {
	LOG_F(3, "Stream destroy");
}

void LocalPCMSource::doGetNextFrame() {
	if (_audioController->isTerminated()) {
		handleClosure();
		return;
	}

	// Try to read as many bytes as will fit in the buffer provided (or "fPreferredFrameSize" if less)
	if (fPreferredFrameSize < fMaxSize) {
		fMaxSize = fPreferredFrameSize;
	}
	unsigned bytesPerSample = (fNumChannels * fBitsPerSample) / 8;
	if (bytesPerSample == 0)
		bytesPerSample = 1; // because we can't read less than a byte at a time

	//unsigned bytesToRead = fMaxSize - fMaxSize%bytesPerSample;
	unsigned samplesToRead = fMaxSize / bytesPerSample;
	unsigned numBytesRead = 0;
	size_t samplesRead = 0;

	/* loop through one sample at a time - will read from the currently referenced batch
	 * once the batch has been read it will move to the next batch in the queue */
	do {
		// grab the next batch of samples from the queue
		if (!_currentBatch) {
			if (!_audioController->outputQueue.try_dequeue(_currentBatch))
				// queue is empty - underrun
				break;
			else
				_batchIterator = _currentBatch->begin();
		}

		// convert 32 bit float to 16 bit PCM - floats have values [-1.0, 1.0]
		float sample = *_batchIterator;
		short pcm = (short) std::floor(sample * 32767);
		std::memcpy(fTo + numBytesRead, &pcm, bytesPerSample);
		samplesRead++;
		numBytesRead += bytesPerSample;

		_batchIterator++;
		if (_batchIterator >= _currentBatch->end())
			_currentBatch = nullptr;

		if (samplesRead == samplesToRead)
			break;
	} while (1);

	fFrameSize = numBytesRead;

	if (samplesRead == 0){
		LOG_F(2, "Audio buffer underrun, closing stream");
		handleClosure();
		return;
	}

	// Set the 'presentation time' and 'duration' of this frame:
	if (fPresentationTime.tv_sec == 0 && fPresentationTime.tv_usec == 0) {
		// This is the first frame, so use the current time:
		gettimeofday(&fPresentationTime, NULL);
	} else {
		// Increment by the play time of the previous data:
		unsigned uSeconds = fPresentationTime.tv_usec + fLastPlayTime;
		fPresentationTime.tv_sec += uSeconds / 1000000;
		fPresentationTime.tv_usec = uSeconds % 1000000;
	}

	// Remember the play time of this data:
	fDurationInMicroseconds = fLastPlayTime = (unsigned) ((fPlayTimePerSample
			* fFrameSize) / bytesPerSample);

	// Inform the downstream object that it has data:
	nextTask() = envir().taskScheduler().scheduleDelayedTask(0, (TaskFunc*)FramedSource::afterGetting, this);
}

void LocalPCMSource::doStopGettingFrames() {
  envir().taskScheduler().unscheduleDelayedTask(nextTask());
}

Boolean LocalPCMSource::setInputPort(int /*portIndex*/) {
  return True;
}

double LocalPCMSource::getAverageLevel() const {
  return 0.0;
}


} /* namespace piscan */
