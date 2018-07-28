package com.ezratl.piscan.common;
import java.io.*;

abstract class Squelch implements Serializable {
	/**
	 * 
	 */
	private static final long serialVersionUID = 7687287006799233291L;

	public abstract String toString();

	public abstract boolean hasSignal();
	
	public static Squelch parseTone(String tone) {
		System.out.println(tone);
		StringBuilder newTone = new StringBuilder(tone);
		if(tone.startsWith("TONE=C")) {
			newTone.delete(0, 6);
			return new CTCSS(Double.parseDouble(newTone.toString()));
		}
		else if(tone.startsWith("TONE=D")) {
			newTone.delete(0, 6);
			return new DCS(Integer.parseInt(newTone.toString()));
		}
		return new CSQ();
	}
}
