package com.ezratl.piscan.backend.scanner;
import java.io.*;

public abstract class RadioSystem implements Serializable {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1893524845450392765L;
	protected boolean lockedOut;
	protected String tag;
	protected int channelCount;

	public RadioSystem(String sysName) {
		tag = sysName;
		channelCount = 0;
	}
	
	public RadioSystem(String sysName, boolean lockout) {
		tag = sysName;
		lockedOut = lockout;
	}
	
	public void setName(String newName) {
		tag = newName;
	}
	
	public abstract void addEntry(Object newEntry);

	public String toString() {
		return tag;
	}

	public int channels() {
		return channelCount;
	}
	
	public void setLockout(boolean lock) {
		lockedOut = lock;
	}
	
	public boolean lockedOut() {
		return lockedOut;
	}
	
	public abstract String getSystemType();
	
	public static RadioSystem getSystem(File sysFile) {
		ObjectInputStream instream;
		try {
			instream = new ObjectInputStream(new FileInputStream(sysFile));
			Object nextObj = instream.readObject();
			instream.close();
			return (RadioSystem) nextObj;
		}
		catch(ClassNotFoundException e1) {
			System.out.println("System not found or corrupt in " + sysFile);
		}
		catch(ClassCastException e2) {
			System.out.println("Contents of " + sysFile + "is of an incorrect class");
		}
		catch(EOFException e3) {
			
		}
		catch(IOException e4) {
			System.out.println(e4);
		}
		return null;
	}
	
	public static int saveSystem(File sysDir, RadioSystem system, int index) {
		//tag must be checked for illegal characters before making file path
		StringBuilder newFileName = new StringBuilder(system.toString());
		for(int i = 0; i < newFileName.length(); i++)
			if(newFileName.charAt(i) == '/' || newFileName.charAt(i) == '\\')
				newFileName.deleteCharAt(i);
		
		File systemFile = new File(sysDir + "/" + String.valueOf(index) + " - " + newFileName + ".system");
		ObjectOutputStream outstream;
		try{
			systemFile.createNewFile();
			outstream = new ObjectOutputStream(new FileOutputStream(systemFile));
			outstream.writeObject(system);
			outstream.close();
			return 1;
		}
		catch(IOException e) {
			System.out.println(systemFile);
			e.printStackTrace();
			return 0;
		}
	}
}
