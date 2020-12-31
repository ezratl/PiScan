/*
 * AudioServerMediaSubsession.cpp
 *
 *  Created on: May 3, 2020
 *      Author: ezra
 */

#include "audio_server/AudioServerMediaSubsession.h"
#include "audio_server/LocalPCMSource.h"
#include "uLawAudioFilter.hh"
#include "SimpleRTPSink.hh"
#include "MP3Transcoder.hh"

namespace piscan {
namespace server {
namespace audio {

AudioServerMediaSubsession* AudioServerMediaSubsession
::createNew(UsageEnvironment& env, Boolean reuseFirstSource,
	    Boolean convertToULaw) {
  return new AudioServerMediaSubsession(env, reuseFirstSource, convertToULaw);
}

AudioServerMediaSubsession
::AudioServerMediaSubsession(UsageEnvironment& env, Boolean reuseFirstSource, Boolean convertToULaw)
  : OnDemandServerMediaSubsession(env,/* fileName, */reuseFirstSource),
    fConvertToULaw(convertToULaw) {
}

AudioServerMediaSubsession
::~AudioServerMediaSubsession() {
}

/*void AudioServerMediaSubsession
::seekStreamSource(FramedSource* inputSource, double& seekNPT, double streamDuration, u_int64_t& numBytes) {
  LocalPCMSource* wavSource;
  if (fBitsPerSample > 8) {
    // "inputSource" is a filter; its input source is the original WAV file source:
    wavSource = (LocalPCMSource*)(((FramedFilter*)inputSource)->inputSource());
  } else {
    // "inputSource" is the original WAV file source:
    wavSource = (LocalPCMSource*)inputSource;
  }

  unsigned seekSampleNumber = (unsigned)(seekNPT*fSamplingFrequency);
  unsigned seekByteNumber = seekSampleNumber*((fNumChannels*fBitsPerSample)/8);

  wavSource->seekToPCMByte(seekByteNumber);

  setStreamSourceDuration(inputSource, streamDuration, numBytes);
}*/

/*void AudioServerMediaSubsession
::setStreamSourceDuration(FramedSource* inputSource, double streamDuration, u_int64_t& numBytes) {
  LocalPCMSource* wavSource;
  if (fBitsPerSample > 8) {
    // "inputSource" is a filter; its input source is the original WAV file source:
    wavSource = (AudioSource*)(((FramedFilter*)inputSource)->inputSource());
  } else {
    // "inputSource" is the original WAV file source:
    wavSource = (AudioSource*)inputSource;
  }

  unsigned numDurationSamples = (unsigned)(streamDuration*fSamplingFrequency);
  unsigned numDurationBytes = numDurationSamples*((fNumChannels*fBitsPerSample)/8);
  numBytes = (u_int64_t)numDurationBytes;

  wavSource->limitNumBytesToStream(numDurationBytes);
}*/

/*void AudioServerMediaSubsession
::setStreamSourceScale(FramedSource* inputSource, float scale) {
  int iScale = (int)scale;
  AudioSource* wavSource;
  if (fBitsPerSample > 8) {
    // "inputSource" is a filter; its input source is the original WAV file source:
    wavSource = (AudioSource*)(((FramedFilter*)inputSource)->inputSource());
  } else {
    // "inputSource" is the original WAV file source:
    wavSource = (AudioSource*)inputSource;
  }

  wavSource->setScaleFactor(iScale);
}*/

FramedSource* AudioServerMediaSubsession
::createNewStreamSource(unsigned /*clientSessionId*/, unsigned& estBitrate) {
  FramedSource* resultSource = NULL;
  do {
    LocalPCMSource* wavSource = LocalPCMSource::createNew(envir());
    if (wavSource == NULL) break;

    // Get attributes of the audio source:

    //fAudioFormat = wavSource->getAudioFormat();
    fBitsPerSample = wavSource->bitsPerSample();
    // We handle only 4,8,16,20,24 bits-per-sample audio:
    if (fBitsPerSample%4 != 0 || fBitsPerSample < 4 || fBitsPerSample > 24 || fBitsPerSample == 12) {
      envir() << "The input file contains " << fBitsPerSample << " bit-per-sample audio, which we don't handle\n";
      break;
    }
    fSamplingFrequency = wavSource->samplingFrequency();
    fNumChannels = wavSource->numChannels();
    unsigned bitsPerSecond = fSamplingFrequency*fBitsPerSample*fNumChannels;

    //fFileDuration = (float)((8.0*wavSource->numPCMBytes())/(fSamplingFrequency*fNumChannels*fBitsPerSample));
    fFileDuration = 0;

    // Add in any filter necessary to transform the data prior to streaming:
    resultSource = wavSource; // by default

    /*if (fAudioFormat == PCM_PCM)
    {*/
      if (fBitsPerSample == 16)
      {
        // Note that samples in the WAV audio file are in little-endian order.
        if (fConvertToULaw)
        {
          // Add a filter that converts from raw 16-bit PCM audio to 8-bit u-law audio:
          resultSource = uLawFromPCMAudioSource::createNew(envir(), wavSource, 1 /*little-endian*/);
          bitsPerSecond /= 2;
        }
        else
        {
          // Add a filter that converts from little-endian to network (big-endian) order:
          resultSource = EndianSwap16::createNew(envir(), wavSource);
        }
      }
      else if (fBitsPerSample == 20 || fBitsPerSample == 24)
      {
        // Add a filter that converts from little-endian to network (big-endian) order:
        resultSource = EndianSwap24::createNew(envir(), wavSource);
      }
    //}

    estBitrate = (bitsPerSecond+500)/1000; // kbps
    return resultSource;
  } while (0);

  // An error occurred:
  Medium::close(resultSource);
  return NULL;
}

RTPSink* AudioServerMediaSubsession::createNewRTPSink(Groupsock* rtpGroupsock,
		unsigned char rtpPayloadTypeIfDynamic, FramedSource* /*inputSource*/) {
	do {
		char const* mimeType;
		unsigned char payloadFormatCode = rtpPayloadTypeIfDynamic; // by default, unless a static RTP payload type can be used
		//if (fAudioFormat == PCM_PCM) {
		if (fBitsPerSample == 16) {
			if (fConvertToULaw) {
				mimeType = "PCMU";
				if (fSamplingFrequency == 8000 && fNumChannels == 1) {
					payloadFormatCode = 0; // a static RTP payload type
				}
			} else {
				mimeType = "L16";
				if (fSamplingFrequency == 44100 && fNumChannels == 2) {
					payloadFormatCode = 10; // a static RTP payload type
				} else if (fSamplingFrequency == 44100 && fNumChannels == 1) {
					payloadFormatCode = 11; // a static RTP payload type
				}
			}
		} else if (fBitsPerSample == 20) {
			mimeType = "L20";
		} else if (fBitsPerSample == 24) {
			mimeType = "L24";
		} else { // fBitsPerSample == 8 (we assume that fBitsPerSample == 4 is only for PCM_IMA_ADPCM)
			mimeType = "L8";
		}

		return SimpleRTPSink::createNew(envir(), rtpGroupsock,
				payloadFormatCode, fSamplingFrequency, "audio", mimeType,
				fNumChannels);
	} while (0);

	// An error occurred:
	return NULL;
}

void AudioServerMediaSubsession::testScaleFactor(float& scale) {
  if (fFileDuration <= 0.0) {
    // The file is non-seekable, so is probably a live input source.
    // We don't support scale factors other than 1
    scale = 1;
  } else {
    // We support any integral scale, other than 0
    int iScale = scale < 0.0 ? (int)(scale - 0.5) : (int)(scale + 0.5); // round
    if (iScale == 0) iScale = 1;
    scale = (float)iScale;
  }
}

float AudioServerMediaSubsession::duration() const {
  return fFileDuration;
}


} /* namespace piscan */
}
}
