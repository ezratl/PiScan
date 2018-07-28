package com.ezratl.piscan.common.config;

import java.util.List;

import com.ezratl.piscan.common.Menuable;

public abstract class Configuration implements Menuable {

	

	@Override
	public abstract String getDescriptor();

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
	
	/* Configuration classes */
	
	// planned for embedded installations
//	public class BluetoothConfiguration extends Configuration {
//		
//	}
	
	// planned for embedded installations
//	public class WifiConfiguration extends Configuration {
//		
//	}
	
}
