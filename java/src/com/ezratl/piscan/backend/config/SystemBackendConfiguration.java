package com.ezratl.piscan.backend.config;

import java.util.ArrayList;

import com.ezratl.piscan.common.Configuration;
import com.ezratl.piscan.common.Menuable;

public class SystemBackendConfiguration extends Configuration {
	private static final String MENU_DESCRIPTOR = "Drivers";
	
	private TunerOption selectedTuner;

	@Override
	public String getDescriptor() {
		return MENU_DESCRIPTOR;
	}

	protected static enum TunerOption {
		NONE,
		RTL_SDR,
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
	public int getSelectedKeyIndex() {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public Menuable removeKey(int index) {
		// TODO Auto-generated method stub
		return null;
	}
}
