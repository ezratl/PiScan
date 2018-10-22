package com.ezratl.piscan.backend.config;
import java.io.*;
import java.util.ArrayList;

import com.ezratl.piscan.common.Configuration;
import com.ezratl.piscan.common.Menuable;

public class SystemConfiguration extends Configuration implements Serializable {
	/**
	 * 
	 */
	private static final long serialVersionUID = 4166668686700666708L;
	private static final String MENU_DESCRIPTOR = "Configuration";
	
	public String displayDriver, tunerDriver, audioDriver;
	public boolean hasPhysicalInterface;
	public int squelch;
	
	public SystemBackendConfiguration 	systemBackend;
	public WebInterfaceConfiguration	webInterface;
	//public BluetoothConfiguration		bluetooth;
	//public WifiConfiguration			wifi;
	
	
	public SystemConfiguration() {

	}
	
	@Override
	public String getDescriptor() {
		return MENU_DESCRIPTOR;
	}
	
	public static SystemConfiguration getConfig(File configFile) {
		ObjectInputStream instream;
		try {
			instream = new ObjectInputStream(new FileInputStream(configFile));
			Object nextObj = instream.readObject();
			instream.close();
			return (SystemConfiguration) nextObj;
		}
		catch(ClassNotFoundException e1) {
			System.out.println("Config not found or corrupt in " + configFile);
		}
		catch(ClassCastException e2) {
			System.out.println("Contents of " + configFile + "is of an incorrect class");
		}
		catch(EOFException e3) {
			
		}
		catch(FileNotFoundException e4) {
			System.out.println(configFile + " not found");
			return new SystemConfiguration();
		}
		catch(IOException e5) {
			System.out.println(e5);
		}
		return new SystemConfiguration();
	}
	
	public static int saveConfig(File configDir, SystemConfiguration config) {
		File configFile = new File(configDir + "/config.piscan");
		ObjectOutputStream outstream;
		try{
			outstream = new ObjectOutputStream(new FileOutputStream(configFile));
			outstream.writeObject(config);
			outstream.close();
			return 1;
		}
		catch(IOException e) {
			return 0;
		}
	}

	@Override
	public boolean addKey(Menuable value) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public ArrayList<String> getKeyDescriptors() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Menuable removeKey(int index) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public int getSelectedKeyIndex() {
		// TODO Auto-generated method stub
		return 0;
	}

}
