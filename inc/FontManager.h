/*!************************************************************************
\file FontManager.h
\author Tan Jun Rong
\par DP email: t.junrong@digipen.edu
\par Course: CSD1171B
\par Software Engineering Project
\date 29-02-2023
\brief
This header file contians prototype functions and declarations for FontManager.cpp.
**************************************************************************/
#pragma once
#include <AEEngine.h>
#include <vector>
#include <iostream>

namespace FontManager {
	const float DEFAULT_FONT_SIZE = 100.0;	// Base font size to reference to when scaling down font size.

	/*!***********************************************************************
	\brief
		Contains all font types in game.
	*************************************************************************/
	enum FONT_TYPE {
		ROBOTO,
		SHIN_GO
	};

	/*!***********************************************************************
	\brief
		Font object.
	*************************************************************************/
	struct FONT {
		FONT_TYPE type;
		s8 font;
	};

	/*!***********************************************************************
	\brief
		Initialize font manager.
	*************************************************************************/
	void Initialize();

	/*!***********************************************************************
	\brief
		Destory all font that is loaded.
	*************************************************************************/
	void Unload();

	/*!***********************************************************************
	\brief
		Get font based on font type.
	*************************************************************************/
	s8 GetFont(const FONT_TYPE& type);
}
