/*Class for converting Uniden HomePatrol favorites list to PiScan database
 * Written by Ezra Taimuty-Loomis
 * <date>
 * */

package com.ezratl.piscan.scanmanager;

import java.io.*;
import java.util.*;
import piScan.*;

public class SentinelConverter {

	public static void main(String [] args) {
		String param;
		if(args.length == 0) {
			Scanner in = new Scanner(System.in);
			param = in.next();
			in.close();
		}
		else
			param = args[0];
		new SentinelConverter(new File(param));
	}
	
	public SentinelConverter(File favList) {
		systems = new ArrayList<RadioSystem>();
		try {
			BufferedReader filein = new BufferedReader(new FileReader(favList));
			String nextLine;
			while((nextLine = filein.readLine()) != null) {
				parseLine(nextLine);
			}
			filein.close();
		}
		catch(IOException e) {
			System.out.println(favList);
			e.printStackTrace();
		}
		
		for(RadioSystem sys: systems) {
			System.out.println("System: " + sys + "\tType: " + sys.getSystemType());
			if(sys.getSystemType().equals("Analog"))
				for(Channel chan: ((AnalogSystem) sys).channels) {
					System.out.print("\tChannel: " + chan + "\tFreq: " + chan.getFreqMHz() + chan.getModulation() + "\tLockout: " + chan.lockedOut());
					System.out.println("\tSquelch: " + chan.getSquelch() + "\tAtten: " + chan.attenuation() + "\tDelay: " + chan.checkDelay());
				}
		}
	}
	
	public ArrayList<RadioSystem> getConvertedSystems(){
		return systems;
	}
	
	private ArrayList<RadioSystem> systems;
	private RadioSystem currentSys;
	
	private void parseLine(String nextLine) {
		Scanner lineScanner = new Scanner(nextLine);
		lineScanner.useDelimiter("\t+");
		String nextHeader = lineScanner.next();
		if(nextHeader.equals("C-Group")) {
			//System.out.println("Found system");
			currentSys = new AnalogSystem(lineScanner.next(), Boolean.parseBoolean(lineScanner.next()));
			systems.add(currentSys);
			System.out.println("Added system " + currentSys);
		}
		else if(nextHeader.equals("C-Freq")) {
			//System.out.println("Found channel");
			String tag = lineScanner.next(), lockout = lineScanner.next(), freq = lineScanner.next(), mod = lineScanner.next(), tone = lineScanner.next();
			lineScanner.next();
			String atten = lineScanner.next(), delay = lineScanner.next();
			Channel newChannel = new Channel(tag, parseBool(lockout), Long.parseLong(freq), mod, tone, parseBool(atten), parseBool(delay));
			currentSys.addEntry(newChannel);
			//System.out.println("Added channel " + newChannel);
		}
		lineScanner.close();
	}
	
	private boolean parseBool(String token) {
		return (token.equals("On") || token.equals("2"));
	}
}
