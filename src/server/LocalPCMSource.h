/*
 * LocalPCMSource.h
 *
 *  Created on: May 3, 2020
 *      Author: ezra
 *
 *  Implementation based on WAVAudioFileSource from live555
 *  it is stripped of format checking because we know what format we are using
 *  seeking functionality also removed because the stream is live
 */

#pragma once

#include <AudioInputDevice.hh>
#include <atomic>
#include <vector>
#include <memory>

class AudioThread; // forward declaration

namespace piscan {

class LocalPCMSource: public AudioInputDevice {
public:

  static LocalPCMSource* createNew(UsageEnvironment& env);

protected:
  LocalPCMSource(UsageEnvironment& env, AudioThread* audioController);
	// called only by createNew()

  virtual ~LocalPCMSource();

private:
  // redefined virtual functions:
  virtual void doGetNextFrame();
  virtual void doStopGettingFrames();
  virtual Boolean setInputPort(int portIndex);
  virtual double getAverageLevel() const;

protected:
  unsigned fPreferredFrameSize;

private:
  double fPlayTimePerSample; // useconds
  Boolean fFidIsSeekable;
  unsigned fLastPlayTime; // useconds
  Boolean fHaveStartedReading;

  // new members
  AudioThread* _audioController;

  std::shared_ptr<std::vector<float>> _currentBatch = nullptr;
  std::vector<float>::iterator _batchIterator;
};

} /* namespace piscan */
