package com.ezratl.piscan.common.config;

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
}
