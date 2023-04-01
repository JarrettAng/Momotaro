/*!************************************************************************
\file:          DebugManager.cpp
\author:		Jarrett Ang
\par DP email:	a.jiaweijarrett@digipen.edu
\par Course:    CSD1171B
\par Software Engineering Project
\date:          31-03-2023
\brief
This source file implements the DebugManager header file, it handles the 
printing of any debug information to the console as well as updating the 
status of "debug mode" that enables on-screen debug info and hotkey cheats 
(See the MomoTown_ReadMe.txt)

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
**************************************************************************/

///////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>

#include <AEEngine.h>
#include <RenderSystem.h>
#include <InputManager.h>
#include <DebugManager.h>
#include <UIManager.h>
#include <ColorTable.h>
///////////////////////////////////////////////////////////////////////////

namespace Debug {
	UI::TextBox debugStatus; // Contains the "Debug Mode On" text
	bool debugOn;			 // Flag for whether debug mode is currently enabled

	// Sets up the debug text and debug mode to false
	void Initialize() {
		debugStatus = UI::TextBox({ (float)AEGfxGetWinMinX() + 15.0f, (float)AEGfxGetWinMinY() + 15.0f },
								  "Debug Mode On", UI::LEFT_JUSTIFY, AEGfxGetWinMaxX() * 2.0f, 40.0f, COLOR_RED);

		// Enables debug mode to be turned on/off using the ; and ' keys
		InputManager::SubscribeToKey(AEVK_SEMICOLON, InputManager::TRIGGERED, ToggleDebugModeOn);
		InputManager::SubscribeToKey(AEVK_QUOTE, InputManager::TRIGGERED, ToggleDebugModeOff);
		debugOn = false;
	}

	// Enables debug mode to be turned on/off using the ; and ' keys
	void EnableControls() {
		// Unsubscribes the input event just in case
		InputManager::UnsubscribeKey(AEVK_SEMICOLON, InputManager::TRIGGERED, ToggleDebugModeOn);
		InputManager::UnsubscribeKey(AEVK_QUOTE, InputManager::TRIGGERED, ToggleDebugModeOff);

		InputManager::SubscribeToKey(AEVK_SEMICOLON, InputManager::TRIGGERED, ToggleDebugModeOn);
		InputManager::SubscribeToKey(AEVK_QUOTE, InputManager::TRIGGERED, ToggleDebugModeOff);
	}

	// Unsubscribes from the input events
	void Free() {
		InputManager::UnsubscribeKey(AEVK_SEMICOLON, InputManager::TRIGGERED, ToggleDebugModeOn);
		InputManager::UnsubscribeKey(AEVK_QUOTE, InputManager::TRIGGERED, ToggleDebugModeOff);
	}

	// Toggles the status of debug mode
	void SetDebugMode(bool turnOn) {
		debugOn = turnOn;
	}

	void ToggleDebugModeOn() {
		debugOn = true;
	}

	void ToggleDebugModeOff() {
		debugOn = false;
	}

	// Returns the current status of debug mode
	bool IsDebugModeOn() {
		return debugOn;
	}

	// Renders the "Debug Mode On" text
	void ShowDebugStatus() {
		if (!debugOn) return;

		debugStatus.Render();
	}

	// Used to print a message to cout (Only when debug mode is on)
	void Print(std::string const& msg) {
		if (!debugOn) return;

		std::cout << msg;
	}

	// Used to print error msgs to cerr (Only when debug mode is on)
	void PrintError(std::string const& fileName, int lineNum, std::string const& errorMsg) {
		if (!debugOn) return;

		std::cerr << "[ERROR] In file: " << fileName << " at line: " << lineNum << " " << errorMsg << "\n";
	}

}
