/*!************************************************************************
\file:          UIManager.h
\author:		Jarrett Ang
\par DP email:	a.jiaweijarrett@digipen.edu
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief


The functions include:
-
**************************************************************************/

#pragma once
///////////////////////////////////////////////////////////////////////////
#include <string>
#include <vector>

#include <RenderSystem.h>
///////////////////////////////////////////////////////////////////////////

namespace UI {
	enum TextBoxAlignment {
		LEFT_JUSTIFY,
		CENTER_JUSTIFY,
		RIGHT_JUSTIFY
	};

	// Holds the information for each text line in the text box class below
	struct TextBoxLineData {
		std::string text;
		Vec2<float> pos;

		TextBoxLineData(std::string _text, Vec2<float> _pos) : text(_text), pos(_pos) {}
	};

	// This class handles the text box object which splits a string into multiple lines to fit within a box
	class TextBox {
	public:
		///////////////////////////////////////////////////////////////////
		// Constructors
		TextBox();
		TextBox(Vec2<float> screenPos, std::string text, TextBoxAlignment alignment, float maxWidth, float fontSize, Vec3<float> color);

		// Copy assignment overload
		TextBox& operator=(TextBox const& rhs);

		///////////////////////////////////////////////////////////////////
		// Member functions
		const RenderSystem::Transform& GetPos() const;
		void Render();

	private:
		///////////////////////////////////////////////////////////////////
		// Data members
		RenderSystem::Transform		 transform;
		std::vector<TextBoxLineData> texts;
		TextBoxAlignment			 alignment;

		float maxWidth;
		float fontSize;

		Vec3<float> color;
	};
}
