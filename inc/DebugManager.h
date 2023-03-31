/*!************************************************************************
\file:          DebugManager.h
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
#pragma once

namespace Debug {
	void Initialize();
	void Free();

	void SetDebugMode(bool turnOn);
	void ToggleDebugModeOn();
	void ToggleDebugModeOff();

	bool IsDebugModeOn();

	void ShowDebugStatus();

	void Print(std::string const& msg);
}
