/*!************************************************************************
\file:          DebugManager.h
\author:		Jarrett Ang
\par DP email:	a.jiaweijarrett@digipen.edu
\par Course:    CSD1171B
\par Software Engineering Project
\date:          31-03-2023
\brief
This header file handles the printing of any debug information to the console
as well as updating the status of "debug mode" that enables on-screen debug info
and hotkey cheats (See the MomoTown_ReadMe.txt)

The functions include:
- EnableControls
Enables debug mode to be turned on/off using the ; and ' keys
- SetDebugMode
Toggles the status of debug mode
- IsDebugModeOn
Returns the current status of debug mode
- Print
Used to print a message to cout (Only when debug mode is on)
- PrintError
Used to print error msgs to cerr (Only when debug mode is on)

All content © 2023 DigiPen Institute of Technology Singapore.
All rights reserved.
**************************************************************************/

#pragma once
///////////////////////////////////////////////////////////////////////////
#include <string>
///////////////////////////////////////////////////////////////////////////

namespace Debug {
	void Initialize();		// Sets up the debug text and debug mode to false
	void EnableControls();	// Enables debug mode to be turned on/off using the ; and ' keys
	void Free();			// Unsubscribes from the input events

	void SetDebugMode(bool turnOn);	// Toggles the status of debug mode
	void ToggleDebugModeOn();
	void ToggleDebugModeOff();

	bool IsDebugModeOn();	// Returns the current status of debug mode
	void ShowDebugStatus();	// Renders the "Debug Mode On" text

	// Used to print a message to cout (Only when debug mode is on)
	void Print(std::string const& msg);	
	// Used to print error msgs to cerr (Only when debug mode is on)
	void PrintError(std::string const& fileName, int lineNum, std::string const& errorMsg);
}
