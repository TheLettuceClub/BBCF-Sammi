# BBCF-Sammi

This mod exports Blazblue: Central Fiction's game state to any app you wish that is capable of reading WebSockets data. In particular: SAMMI, the streaming assistant, is targeted.
Very WIP at the moment! Expect bugs, inconsistencies, and lack of data. Please get in contact (below), if you have questions/concerns!

## Installation
1. Navigate to your BBCF install directory. You can right click on the game in Steam and then click "browse local files" under "manage" to get to the right folder.
2. Put all three .dll files in the release zip into that folder, next to BBCF.exe
    * If you're also using BBCFIM, rename my dinput8.dll to BBCF-sammi.dll, and add it to BBCFIM's settings.ini at the bottom where it says "DinputDllWrapper".
3. Launch the game. If a console window appears with some text as the game launches, then it's working.
4. Connect with your client of choice (port: 42617). This is implementation specific. SAMMI instructions to come soon (tm).

## Disclaimer
This software does not enable cheating. It only exposes a read-only version of the game's state for other software. If you make a cheat with this code you suck as a human being, and I'm not liable for your crappy life or its resultant choices.

## API overview
The mod sends out 5 types of events based on the state of the game. They are detailed below.
Each event consists of a JSON object with multiple fields. Tabbing implies a '.'
<br><br>
#### State Update:
* Abstract: the general state of the game and characters. Occurs every frame.
* Fields:
    * data
	    * event: will always be "bbcf_stateUpdate"
	    * eventInfo
    		* p1/2
    			* health
    			* posx
    			* character
    			* prevAction
    			* currAction
    			* heat
    			* Overdrive
    			* barrierGauge
    			* drive
    			* maxDrive
    			* side
    		* frameCount

#### Hit Event:
* Abstract: fires every time any character gets hit or blocks
* Fields:
* data
	* event: "bbcf_hitEvent"
	* eventInfo
		* attacker
		* attackerAction
		* defender
		* defenderAction
		* defenderPrevAction
		* attackLevel
		* attackFlag
		* untechTime
		* hitstopOverride
		* airPushbackX
		* airPushbackY
		* damage
		* scalingTicks
		* frameCount

#### Round Start:
* Abstract: fires whenever a round starts. Will have more data eventually.
* Fields:
* data
	* event: "bbcf_roundStartEvent"
	* eventInfo
		* frameCount

#### Round End:
* Abstract: fires whenever a round ends. Will have more data eventually.
* Fields:
* data
	* event: "bbcf_roundEndEvent"
	* eventInfo
		* frameCount

#### Object Creation:
* Abstract: Fires whenever a new game object (particle effect, projectile, etc) is created. May have more info eventually.
* Fields:
* data
	* event: "bbcf_objectCreatedEvent"
	* eventInfo
		* currAction
		* sprite
		* frameCount
		
## Contact:
Please only contact me if you have issues or major feature requests. Do not ask me how to set up stuff in SAMMI, other documentation covers that.
To reach me:
* quattrodan3 on Discord
* nleff72 on Twitter/BlueSky
* email: dan (at) lakeviewcomputerspecialists (dot) com
