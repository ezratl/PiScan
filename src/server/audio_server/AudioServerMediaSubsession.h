/*
 * AudioServerMediaSubsession.h
 *
 *  Created on: May 3, 2020
 *      Author: ezra
 *
 *  Implementation based on WAVAudioFileServerMediaSubsession from live555
 *  Some format checking is stripped away because we know what we're using
 */

#pragma once

#include "OnDemandServerMediaSubsession.hh"

namespace piscan {
namespace server {
namespace audio {

class AudioServerMediaSubsession: public OnDemandServerMediaSubsession{
public:
  static AudioServerMediaSubsession*
  createNew(UsageEnvironment& env, Boolean reuseFirstSource = true, Boolean convertToULaw = False);
      // If "convertToULaw" is True, 16-bit audio streams are converted to
      // 8-bit u-law audio prior to streaming.

protected:
  AudioServerMediaSubsession(UsageEnvironment& env, Boolean reuseFirstSource, Boolean convertToULaw);
      // called only by createNew();
  virtual ~AudioServerMediaSubsession();

protected: // redefined virtual functions
  //virtual void seekStreamSource(FramedSource* inputSource, double& seekNPT, double streamDuration, u_int64_t& numBytes);
  //virtual void setStreamSourceScale(FramedSource* inputSource, float scale);
  //virtual void setStreamSourceDuration(FramedSource* inputSource, double streamDuration, u_int64_t& numBytes);

  virtual FramedSource* createNewStreamSource(unsigned clientSessionId,
					      unsigned& estBitrate);
  virtual RTPSink* createNewRTPSink(Groupsock* rtpGroupsock,
                                    unsigned char rtpPayloadTypeIfDynamic,
				    FramedSource* inputSource);
  virtual void testScaleFactor(float& scale);
  virtual float duration() const;

protected:
  Boolean fConvertToULaw;

  // The following parameters of the input stream are set after
  // "createNewStreamSource" is called:
  //unsigned char fAudioFormat;
  unsigned char fBitsPerSample = 16;
  unsigned fSamplingFrequency = 0;
  unsigned fNumChannels = 1;
  float fFileDuration = 0;
};

} /* namespace piscan */
}
}
