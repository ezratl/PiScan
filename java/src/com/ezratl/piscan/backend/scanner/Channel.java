package com.ezratl.piscan.backend.scanner;

import java.text.DecimalFormat;

import com.ezratl.piscan.backend.SignalDemodulator.Squelch;

import java.io.*;

public class Channel implements Serializable {
	/**
	 * 
	 */
	protected static final long serialVersionUID = -5931231339242795401L;
	
	protected String	tag;
	protected long		frequency;
	protected boolean	lockout;
	protected boolean	scanDelay;
	protected int		hitCount;
	protected boolean	attenuate;
	protected boolean	alertOnSignal;
	protected Squelch	squelch;

	public Channel() {
		tag = ""; frequency = 0; lockout = true; scanDelay = false; hitCount = 0; attenuate = false; alertOnSignal = false; squelch = null;
	}
	
	public Channel(String name, long freq, boolean lock, boolean delay) {
		this();
		tag = name; frequency = freq; lockout = lock; scanDelay = delay;
	}
	
	public Channel(String name, long freq, boolean lock, boolean delay, int hits) {
		this(name, freq, lock, delay);
		hitCount = hits;
	}
	
	public Channel(String name, boolean lock, long freq, Squelch tone, boolean atten, boolean delay) {
		this(name, freq, lock, delay);
		attenuate = atten;
		squelch = tone;
	}
	
	public String toString() {
		return (tag + ": " + getFreqMHz() + " " + getSquelch());
	}
	
	public int hits() {
		return hitCount;
	}
	
	public final String getSquelch() {
		return squelch.toString();
	}
	
	public final String getFreqMHz() {
		return (new DecimalFormat(".#####")).format(frequency/1000000.0);
	}
	
	public final String getTag() {
		return tag;
	}
	
	public boolean checkDelay() {
		return scanDelay;
	}
	
	public void setDelay(boolean enable) {
		scanDelay = enable;
	}
	
	public void setLockout(boolean lock) {
		lockout = lock;
	}
	
	public boolean lockedOut() {
		return lockout;
	}

}
