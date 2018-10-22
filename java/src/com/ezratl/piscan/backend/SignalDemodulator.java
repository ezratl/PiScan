package com.ezratl.piscan.backend;

import java.text.DecimalFormat;

public abstract class SignalDemodulator {
	
	protected static final int DEMOD_DEFAULT_BANDWIDTH = 12500;
	protected static final int DEMOD_NARROW_BANDWIDTH = 8000;
	protected static final int DEMOD_WIDE_BANDWIDTH = 16000;

	public static enum Modes {
		FM,
		AM,
		PL,
		DC,
	}
	
	public static abstract class Squelch {
		protected Modes mode;
		
		public Squelch() {}
		
		public abstract String toString();
		
		public Modes getMode() {
			return mode;
		}
		
		public static class SquelchFM extends Squelch {
			protected int bandwidth;
			
			public SquelchFM() {
				super();
				mode = Modes.FM;
			}
			
			public SquelchFM(int bandwidth) {
				setBandwidth(bandwidth);
			}

			public int getBandwidth() {
				return bandwidth;
			}

			public void setBandwidth(int bandwidth) {
				this.bandwidth = bandwidth;
			}

			@Override
			public String toString() {
				return "FM";
			}
			
		}
		
		public static class SquelchAM extends Squelch {
			protected int bandwidth;
			
			public SquelchAM() {
				super();
				mode = Modes.AM;
			}

			public int getBandwidth() {
				return bandwidth;
			}

			public void setBandwidth(int bandwidth) {
				this.bandwidth = bandwidth;
			}

			@Override
			public String toString() {
				return "AM";
			}
			
		}
		
		public static class SquelchPL extends SquelchFM {
			protected float tone; 
			
			public SquelchPL() {
				super();
			}
			
			public SquelchPL(float tone) {
				this();
				this.tone = formatTone(tone);
				if(this.tone != 0f)
					mode = Modes.PL;
			}
			
			public SquelchPL(float tone, int bandwidth) {
				this(tone);
				this.bandwidth = bandwidth;
			}
			
			public String toString() {
				if(mode == Modes.PL)
					return ((new DecimalFormat(".#")).format(tone) + "PL");
				else
					return super.toString();
			}
			
			private static final float toneLookup[] = { 67.0f, 69.3f, 71.9f, 74.4f, 77.0f, 79.7f, 82.5f, 85.4f, 88.5f, 91.5f, 94.8f, 97.4f, 100.0f,
					103.5f, 107.2f, 110.9f, 114.8f, 118.8f, 123.0f, 127.3f, 131.8f, 136.5f, 141.3f, 146.2f, 151.4f, 156.7f, 162.2f, 167.9f,
					173.8f, 179.9f, 186.2f, 192.8f, 203.5f, 210.7f, 218.1f, 225.7f, 233.6f, 241.8f, 250.3f };
			protected static float formatTone(float tone) {
				float min, max;
				for(int i = 0; i < toneLookup.length; i++) {
					if(i == 0)
						min = toneLookup[i];
					else
						min = toneLookup[i] - ((toneLookup[i] - toneLookup[i - 1])/2);
					if(i == toneLookup.length - 1)
						max = toneLookup[i];
					else
						max = toneLookup[i] + ((toneLookup[i + 1] - toneLookup[i])/2);
					
					if(tone >= min && tone <= max)
						return toneLookup[i];
				}
				return 0f;
			}
			public static SquelchPL parseTone(float tone) {
				return new SquelchPL(tone);
			}
			public static SquelchPL parseTone(String tone) {
				return parseTone(Float.parseFloat(tone));
			}
		}
		
		public static class SquelchDC extends SquelchFM {
			protected int tone;
			
			public SquelchDC() {
				super();
				tone = 0;
			}
			
			public SquelchDC(int tone) {
				this();
				this.tone = tone;
				this.mode = Modes.DC;
			}
			
			public SquelchDC(int tone, int bandwidth) {
				this(tone);
				this.bandwidth = bandwidth;
			}
			
			public String toString() {
				if(mode == Modes.DC)
					return (String.valueOf(tone) + "DC");
				else
					return super.toString();
			}
			
			public static SquelchDC parseTone(String tone) {
				return new SquelchDC(Integer.parseInt(tone));
			}
		}
		
	}
}
