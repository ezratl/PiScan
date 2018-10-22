package com.ezratl.piscan.backend.scanner;

public class TrunkSite extends AnalogSystem {
	/**
	 * 
	 */
	private static final long serialVersionUID = -4329943927668592L;
	private String tag;
	public Channel channels[];
	
	public TrunkSite(String siteName) {
		super(siteName);
	}
	
	public String toString() {
		return tag;
	}

}
