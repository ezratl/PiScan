# PiScan
This software can take a Raspberry Pi (or another embedded computer) and turn it into a radio scanner. With only a $40 Pi and a $20 RTL-SDR dongle, you can build an inexpensive scanner with the capabilities of much more expensive equipment (at least that's the goal for this).
## License
A license hasn't been selected yet, though it's likely to be GPL - have to make sure there won't be issues down the line when trunking is added
## Features
 - System scanning
	* Traverses through a JSON list of frequencies
	* Sorts channels into chunks of bandwidth to maximize scanning performance
- Manual frequency entry
- CubicSDR-based demodulator (built on liquid-dsp)
- Configurable squelch and tuner gain
- Configurable logging
- Remote control through TCP
- Remote monitoring with RTSP

## To-Do
#### Short-term (before the first beta):
- Replace TCP connection with WebSockets and possibly HTTP
- CTCSS, DCS support
- AM demodulation
- Run-time manipulation of scan database and config
- Temporary channel lockout
- SDR configuration
#### Long term:
- P25 trunking
- DMR trunking
- Broadcastify interfacing

## System requirements
- Linux OS - Debian or Ubuntu based distros recommended (untested on other distros)
- CPU with 2+ cores
	* ARM: will work on at least an RPi 2B (900MHz BCM2836)
	* x86: should run well on most modern low-power CPUs, has been tested on dual-core Atom-class processors without maxing out the CPU
- RAM: memory usage hasn't been officially profiled, but at least 256MB is probably a sufficient minimum
- Disk space: 10MB is sufficient for the source, binaries, and data files, but more space will be needed depending on the verbosity you choose for the logfiles.
- Additional Raspberry Pi notes
	* At least a small heatsink on the SoC is necessary, since the CPU will be under load for extended periods
	* Sufficient power delivery is required (the official power adapter for example) - power issues will cause the CPU to downclock resulting in poor performance
## Setup - Build from Source
PiScan is supported and has been tested on Debian-based systems (including Raspbian and Ubuntu).

### Prerequisites
The following packages need to be installed, if not already:
    
    g++
    git
    build-essential
    cmake
    libboost-all-dev
    libliquid1d, libliquid-dev
    libsoapysdr-dev, soapysdr-module-rtlsdr
    librtlsdr0
    libpulse-dev, libasound2-dev, libjack-dev
    protobuf-compiler, libprotoc-dev
    libssh-dev

### Initializing 
In a terminal, download the source with the command:

	git clone https://github.com/ezratl/PiScan
Then configure the environment:

	sh ./bootstrap.sh
There are three options available for audio output: PulseAudio, ALSA, and JACK. Your preference can be set by running one of the following commands from the `build` directory:

	cmake ../src -DUSE_AUDIO_PULSE=ON
	cmake ../src -DUSE_AUDIO_ALSA=ON
	cmake ../src -DUSE_AUDIO_JACK=ON
By default PulseAudio is selected, however ALSA seems to use less CPU on the RPi

Additionally, some versions of liquid have a different API, resulting in build errors in the "Modem" files of the `cubic` module. If that happens, try running this from `build`:

	cmake ../src -DLIQUID_API_OLD=ON
### Building
Once the environment is properly configured, `cd` into `build` and build the LiveMedia library:

	make live555
If the build fails: If you are building in a shared folder in a VM on a Windows host, you must download and extract the Live sourcefiles manually into `src/external/live` and run the command below before trying again. This is due to Windows not playing nicely with permissions and symbolic links

	cmake ../src -DHGFS=ON
Now you can build PiScan:

	make all
Alternatively, you can build the binaries individually if you don't need all of them

	make piscan_server		# main program
	
	make piscan_hpdconv		# tool to convert Uniden Sentinel files to PiScan files
### Running
For now, you have to be in the `build` directory to run the program. In the future there will be an install command to eliminate the need for this.
You can now run PiScan from the command line:

	./src/piscan_server
See **Usage** for more information on command arguments and setting up the data files 
    

## Usage
### Command Arguments
`-v [0-9]`: stderr log verbosity. Note this does not effect generated logfiles
`-f [0-9]`: Logfile verbosity - overrides saved config
`-d`: Run with debug interface. This is very user-unfriendly, so interfacing through the client program is recommended
`-p [path]`: load data from `path` rather than the default directory. Helpful for running multiple PiScan instances
### Config and Data
All data used by PiScan is stored in its working directory (this is the `data` directory for alpha builds). It contains a few JSON files as well as a folder for logfiles.
	- `config.json` contains all settings that can be modified by the user
	- `state.json` is used by the program to maintain its stately data between runs
	- `systems.json` contains the scan database - more on what that looks like below
On the first run of PiScan, these files likely won't exist. It will continue running with default parameters, and a config and state file will be generated with these defaults when the program ends.
If there is no scan file, PiScan cannot scan so it will instead hold at 100MHz. It will not allow the user to scan, but will allow manual frequency tuning.
#### Scan Database File
`systems.json` MUST adhere to this format for PiScan to read it correctly

	{
		"systems": [
			{
				"systype": "analog",				<- available types: analog
				"tag": "Sample System",
				"lockout": false,					<- persistent system lockout
				"channels": [
					{
						"tag": "FM entry - CSQ",
						"lockout": false,			<- persistent channel lockout
						"delay": 2000,				<- scan resume delay in milliseconds
						"freq": 453150000,			<- frequency in Hz
						"chantype": "fmc"			<- available types: fmc, amc, plc, dcc
					},
					{
						"tag": "FM entry - PL",
						"lockout": false,
						"delay": 2000,
						"freq": 471837500,
						"chantype": "plc",
						"tone": "103.5"				<- stored as string rather than a float
					},
					{
						"tag": "FM entry - DCS",
						"lockout": false,
						"delay": 2000,
						"freq": 460287500,
						"chantype": "dcc"
						"code": "023"				<- stored as string rather than an int
					}
				]
			}
		]
	}
Do note that since PL and DC squelch are not yet supported, they will be squelch like a CSQ channel.

A scan file can be generated from an `.hpd` file, which is a plaintext format used by Uniden Sentinel to store favorites lists on a PC - this can be found in `Documents/Uniden/BCDx36HP/FavoriteLists`. To generate, run:

	build/scan/piscan_hpdconv -i <path_to_hpd_file> -o <directory_for_output_file>
A generator for CSV input will probably come soon, followed eventually by a GUI editor
### Interfacing
The primary way to interact with PiScan is through the [client program](https://github.com/ezratl/PiScan-Client). Guides on its usage can be found there.
In short, it communicates through a TCP connection, with a default port 1234.

To listen to PiScan's audio feed remotely, use a stream client that supports RTSP, such as VLC, and enter the MRL `rtsp://<host_address>:8554/audio`. Setting a low network caching time is advised to reduce audio latency.
## Disclaimer
This is a personal project; it lacks both the design standards and testing standards of commercially-built software, meaning no guarantees can be made of its reliability. Use at your own risk.

Do not expose the ports used by this software directly to the internet! The TCP connection has no security whatsoever, and although a more secure replacement is planned, the robustness of the software has not been tested.
