package com.ezratl.piscan.backend.scanner;
import java.util.*;

public class AnalogSystem extends RadioSystem {
	/**
	 * 
	 */
	private static final long serialVersionUID = 7707210176596144382L;
	public ArrayList<Channel> channels;

	public AnalogSystem(String sysName) {
		super(sysName);
		channels = new ArrayList<Channel>();
	}
	
	public AnalogSystem(String sysName, boolean lockout) {
		super(sysName, lockout);
		channels = new ArrayList<Channel>();
	}
	
	public void addEntry(Object newEntry) {
		try{
			channels.add((Channel) newEntry);
			System.out.println("\tAdded channel " + newEntry);
		}catch(ClassCastException e) {
			System.out.println(newEntry + " is not a channel!");
		}
	}
	
	public String getSystemType() {
		return "Analog";
	}

}
