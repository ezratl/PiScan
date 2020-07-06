#pragma once

namespace piscan::sigproc {
	enum Modulation {
		NFM,
		FM,
		AM
	};

	class DemodInterface {
	public:
		virtual ~DemodInterface() {};

		virtual bool setFrequency(long long freq) = 0;
		virtual bool setTunerFrequency(long long freq) = 0;
		virtual float getSignalLevel() = 0;
		virtual float getDecodedPL() = 0;
		virtual unsigned int getDecodedDC() = 0;
		virtual bool squelchThresholdMet() = 0;
		virtual bool setModem(Modulation mode) = 0;
		virtual void setSquelch(float level) = 0;
		virtual float getSNR() = 0;
		virtual int getSignalStrength() = 0;
	};

    class Demodulator;
}
