package com.ezratl.piscan.common;

public class CTCSS extends Squelch {
	/**
	 * 
	 */
	private static final long serialVersionUID = -5710651966295398595L;
	private double value;
	
	public CTCSS(double val) {
		// TODO Auto-generated constructor stub
		value = val;
	}
	
	public String toString() {
		return value + " PL";
	}

	public boolean hasSignal() {
		// TODO Auto-generated method stub
		return false;
	}
	
	private static double tones[] = { 67.0, 69.3, 71.9, 74.4, 77.0, 79.7, 82.5, 85.4, 88.5, 91.5, 94.8, 97.4, 100.0,
			103.5, 107.2, 110.9, 114.8, 118.8, 123.0, 127.3, 131.8, 136.5, 141.3, 146.2, 151.4, 156.7, 162.2, 167.9,
			173.8, 179.9, 186.2, 192.8, 203.5, 210.7, 218.1, 225.7, 233.6, 241.8, 250.3 };

	public static CTCSS parseTone(double tone) {
		double min, max;
		for(int i = 0; i < tones.length; i++) {
			if(i == 0)
				min = 0;
			else
				min = tones[i] - ((tones[i] - tones[i - 1])/2);
			if(i == tones.length - 1)
				max = Double.MAX_VALUE;
			else
				max = tones[i] + ((tones[i + 1] - tones[i])/2);
			
			if(tone >= min && tone <= max)
				return new CTCSS(tone);
		}
		return null;
	}
}
