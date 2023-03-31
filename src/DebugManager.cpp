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

bool debugOn = false;

void ToggleDebugMode(bool turnOn) {
	debugOn = turnOn;
}

bool IsDebugModeOn() {
	return debugOn;
}

void Print(std::string const& msg) {
	if (!debugOn) return;

	std::cout << msg;
}
