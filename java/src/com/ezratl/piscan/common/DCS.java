package com.ezratl.piscan.common;

public class DCS extends Squelch {

	/**
	 * 
	 */
	private static final long serialVersionUID = -1573750119107309722L;

	private int value; 
	public DCS(int val) {
		// TODO Auto-generated constructor stub
		value = val;
	}

	@Override
	public String toString() {
		// TODO Auto-generated method stub
		return value + " DPL";
	}

	@Override
	public boolean hasSignal() {
		// TODO Auto-generated method stub
		return false;
	}

}
