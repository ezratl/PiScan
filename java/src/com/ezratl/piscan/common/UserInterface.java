package com.ezratl.piscan.common;

public abstract class UserInterface {
	public static enum Views {
		STARTUP,
		SCANNER_CONNECT,
		
	}
	
	public interface InterfaceUpdateHandler {
		abstract void onViewChangeRequest(Views view);
		abstract void onViewDataAvailable();
		abstract void onSystemStatusUpdate();
	}
}
