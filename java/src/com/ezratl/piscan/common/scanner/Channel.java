package com.ezratl.piscan.common.scanner;

import java.text.DecimalFormat;

import com.ezratl.piscan.common.Squelch;

import java.io.*;

public class Channel implements Serializable {
	/**
	 * 
	 */
	private static final long serialVersionUID = -5931231339242795401L;
	private String tag;
	private String modulation;
	private long freq;
	private boolean lockedOut;
	private boolean scanDelay;
	private int hitCount;
	private Squelch squelchTone;
	private boolean attenuate;

	public Channel(String name, double freqM, String mod, boolean lock, boolean delay) {
		tag = name; freq = (long) (1000000 * freqM); modulation = mod; lockedOut = lock; scanDelay = delay; hitCount = 0;
	}
	
	public Channel(String name, double freqM, String mod, boolean lock, boolean delay, int hits) {
		tag = name; freq = (long) (1000000 * freqM); modulation = mod; lockedOut = lock; scanDelay = delay; hitCount = hits;
	}
	
	public Channel(String name, boolean lock, long freqM, String mod, String tone, boolean atten, boolean delay) {
		tag = name; freq = freqM; modulation = mod; lockedOut = lock; scanDelay = delay; hitCount = 0; attenuate = atten;
		squelchTone = Squelch.parseTone(tone);
	}
	
	public String toString() {
		return tag;
	}
	
	public int hits() {
		return hitCount;
	}
	
	public String getSquelch() {
		return squelchTone.toString();
	}
	
	public String getFreqMHz() {
		return (new DecimalFormat(".#####")).format(freq/1000000.0);
	}
	
	public String getModulation() {
		return modulation;
	}
	
	public boolean attenuation() {
		return attenuate;
	}
	
	public boolean checkDelay() {
		return scanDelay;
	}
	
	public void lockOut() {
		lockedOut = true;
	}
	
	public void unLock() {
		lockedOut = false;
	}
	
	public boolean lockedOut() {
		return lockedOut;
	}
	
	public boolean hasSignal() {
		return false;
	}
	
	public void tune() {
		
	}

}
