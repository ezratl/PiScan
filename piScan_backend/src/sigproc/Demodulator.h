/*
 * Demodulator.h
 *
 *  Created on: Feb 10, 2019
 *      Author: ezra
 */

#ifndef SIGPROC_DEMODULATOR_H_
#define SIGPROC_DEMODULATOR_H_

class DemodInterface {
public:
	virtual ~DemodInterface();

	virtual bool setFrequency(unsigned long freq) = 0;
	virtual int getRssi() = 0;
	virtual float getDecodedPL() = 0;
	virtual unsigned int getDecodedDC() = 0;
	virtual bool squelchThresholdMet() = 0;
};

class Demodulator : public DemodInterface {
public:
	Demodulator();
	~Demodulator();

private:
	bool setFrequency(unsigned long freq);
	int getRssi();
	float getDecodedPL();
	unsigned int getDecodedDC();
	bool squelchThresholdMet();
};

#endif /* SIGPROC_DEMODULATOR_H_ */
