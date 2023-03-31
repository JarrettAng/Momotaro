/*!************************************************************************
\file:          DebugManager.cpp
\author:
\par DP email:
\par Course:    CSD1171B
\par Software Engineering Project
\date:          31-03-2023
\brief


The functions include:
-
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
	UI::TextBox debugStatus;
	bool debugOn;

	void Initialize() {
		debugStatus = UI::TextBox({ (float)AEGfxGetWinMinX() + 15.0f, (float)AEGfxGetWinMinY() + 15.0f },
								  "Debug Mode On", UI::LEFT_JUSTIFY, AEGfxGetWinMaxX() * 2.0f, 40.0f, COLOR_RED);

		InputManager::SubscribeToKey(AEVK_SEMICOLON, InputManager::TRIGGERED, ToggleDebugModeOn);
		InputManager::SubscribeToKey(AEVK_QUOTE, InputManager::TRIGGERED, ToggleDebugModeOff);
		debugOn = false;
	}

	void EnableControls() {
		InputManager::UnsubscribeKey(AEVK_SEMICOLON, InputManager::TRIGGERED, ToggleDebugModeOn);
		InputManager::UnsubscribeKey(AEVK_QUOTE, InputManager::TRIGGERED, ToggleDebugModeOff);

		InputManager::SubscribeToKey(AEVK_SEMICOLON, InputManager::TRIGGERED, ToggleDebugModeOn);
		InputManager::SubscribeToKey(AEVK_QUOTE, InputManager::TRIGGERED, ToggleDebugModeOff);
	}

	void Free() {
		InputManager::UnsubscribeKey(AEVK_SEMICOLON, InputManager::TRIGGERED, ToggleDebugModeOn);
		InputManager::UnsubscribeKey(AEVK_QUOTE, InputManager::TRIGGERED, ToggleDebugModeOff);
	}

	void SetDebugMode(bool turnOn) {
		debugOn = turnOn;
	}

	void ToggleDebugModeOn() {
		debugOn = true;
	}

	void ToggleDebugModeOff() {
		debugOn = false;
	}

	bool IsDebugModeOn() {
		return debugOn;
	}

	void ShowDebugStatus() {
		if (!debugOn) return;

		debugStatus.Render();
	}

	void Print(std::string const& msg) {
		if (!debugOn) return;

		std::cout << msg;
	}

	void PrintError(std::string const& fileName, int lineNum, std::string const& errorMsg) {
		if (!debugOn) return;

		std::cerr << "[ERROR] In file: " << fileName << " at line: " << lineNum << " " << errorMsg << "\n";
	}
}
