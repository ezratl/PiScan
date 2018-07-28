package com.ezratl.piscan.backend;

import java.util.List;

import com.ezratl.piscan.common.Menuable;

public abstract class Tuner implements Menuable {

	public static enum TunerErrorCode {
		TUNER_OK,
		TUNER_ERROR,
		TUNER_INVALID_FREQUENCY
	}
	
	private static FrequencyRange freqRangesSupported[];
	
	public static boolean supportsFrequency(long freq) {
		for(int i = 0; i < freqRangesSupported.length; i++) {
			if(freqRangesSupported[i].isInRange(freq))
				return true;
		}
		return false;
	}
	
	private class FrequencyRange implements Menuable {
		public static final double FREQ_MHZ_DIVIDE_RATIO = 1000000;
		
		private long minFreq;
		private long maxFreq;
		
		public FrequencyRange(long min, long max) {
			if(min > max) {
				minFreq = max;
				maxFreq = min;
			}
			else {
				minFreq = min;
				maxFreq = max;
			}
		}
		public boolean isInRange(long freq) {
			return (freq >= minFreq && freq <= maxFreq);
		}
		@Override
		public String getDescriptor() {
			return (String.valueOf(minFreq / FREQ_MHZ_DIVIDE_RATIO) + " - " + String.valueOf(maxFreq / FREQ_MHZ_DIVIDE_RATIO));
		}
		@Override
		public List<Menuable> getKeys() {
			return null;
		}
		@Override
		public int numberOfKeys() {
			return 0;
		}
		@Override
		public Menuable getKey(int index) {
			return null;
		}
		@Override
		public boolean selectKey(int index) {
			return false;
		}
		@Override
		public Menuable getSelectedKey() {
			return null;
		}
		@Override
		public boolean setKey(int index, Menuable value) {
			return false;
		}
		@Override
		public boolean isMenu() {
			return false;
		}
		@Override
		public boolean isSelectable() {
			return false;
		}
		@Override
		public boolean isModifiable() {
			return false;
		}
	}
}
