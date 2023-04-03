/*!************************************************************************
\file:          UIManager.h
\author:		Jarrett Ang
\par DP email:	a.jiaweijarrett@digipen.edu
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief
This source file implements the UIManager header file, it defines the Textbox 
class and other UI related objects in the future.

This file include:
- Textbox class definition

All content © 2023 DigiPen Institute of Technology Singapore.
All rights reserved.
**************************************************************************/

///////////////////////////////////////////////////////////////////////////
#include <UIManager.h>
#include <FontManager.h>
#include <ColorTable.h>
#include <MomoMaths.h>
///////////////////////////////////////////////////////////////////////////

namespace UI {
	///////////////////////////////////////////////////////////////////////
	// Default Constructor
	TextBox::TextBox() {
		transform = RenderSystem::Transform();
		maxWidth = 500;
		fontSize = 20;
		alignment = LEFT_JUSTIFY;
		color = COLOR_BLACK;
	}

	// Non-default Constructor
	TextBox::TextBox(Vec2<float> screenPos, std::string text, TextBoxAlignment _alignment, float _maxWidth, float _fontSize, Vec3<float> _color) {

		// Set the size of the text (normalized already)
		AEGfxGetPrintSize(FontManager::GetFont(FontManager::ROBOTO), const_cast<char*>(text.c_str()),
			_fontSize / FontManager::DEFAULT_FONT_SIZE, transform.size.x, transform.size.y);

		// Normalize the text box position to (0 - 1)
		transform.pos.x = screenPos.x / AEGfxGetWinMaxX();
		transform.pos.y = screenPos.y / AEGfxGetWinMaxY();

		fontSize = _fontSize;								// How big the font should be (XX/100)
		maxWidth = _maxWidth / AEGfxGetWinMaxX();			// Max width before wrapping the text
		float charWidth = transform.size.x / text.size();	// The x-length of each character
		alignment = _alignment;								// Set the current alignment of the texts (Left, center, right)
		color = _color;										// Set the color of the text box

		Vec2<float> currPos;								// The render pos of each line of text

		size_t start = 0, end = text.size(), cutoff = text.find_last_of(" \n\t"); // Pointers to the different parts of the string

		// Case 1: string of text fits max width, add it to texts then finish
		if (charWidth * (float)(end - start + 1) < maxWidth) {
			switch (alignment) { // Set the x-pos of the text based on the current alignment
			case LEFT_JUSTIFY:
				currPos.x = transform.pos.x;
				break;
			case CENTER_JUSTIFY:
				currPos.x = transform.pos.x + (maxWidth / 2.0f) - (charWidth * (end - start + 1) * 0.5f);
				break;
			case RIGHT_JUSTIFY:
				currPos.x = transform.pos.x + maxWidth - (charWidth * (end - start + 1));
				break;
			}
			currPos.y = transform.pos.y;

			texts.emplace_back(text, currPos);
		}
		// Case 2: Wrap the string around based on the max width until it all fits
		else {
			while (start < end) {
				// Keep looping until the largest spacing index within the max-width is reached
				while (charWidth * (float)(cutoff - start + 1) > maxWidth) {
					cutoff = text.find_last_of(" \n\t", cutoff - 1);

					if (cutoff < start) {
						cutoff = end;
						break;
					}
					if (cutoff == std::string::npos) {
						cutoff = text.find_last_of(" \n\t");
						break;
					}
				}
				// Set the x-pos of the text based on the current alignment
				switch (alignment) {
				case LEFT_JUSTIFY:
					currPos.x = transform.pos.x;
					break;
				case CENTER_JUSTIFY:
					currPos.x = transform.pos.x + (maxWidth / 2.0f) - (charWidth * (cutoff - start + 1) * 0.5f);
					break;
				case RIGHT_JUSTIFY:
					currPos.x = transform.pos.x + maxWidth - (charWidth * (cutoff - start + 1));
					break;
				}
				currPos.y = transform.pos.y - texts.size() * transform.size.y * 1.25f;

				texts.emplace_back(text.substr(start, cutoff - start), currPos);

				start = cutoff == std::string::npos ? cutoff : cutoff + 1;
				cutoff = end;
			}
		}
	}

	// Copy assignment
	TextBox& TextBox::operator=(TextBox const& rhs) {
		this->alignment = rhs.alignment;
		this->color = rhs.color;
		this->fontSize = rhs.fontSize;
		this->maxWidth = rhs.maxWidth;
		this->transform = rhs.transform;

		this->texts.clear();
		for (auto text : rhs.texts) {
			texts.push_back(text);
		}
		return *this;
	}

	///////////////////////////////////////////////////////////////////////
	// Member functions

	// Returns the position of the text box (Top left corner)
	const RenderSystem::Transform& TextBox::GetPos() const {
		return transform;
	}

	// Draws the text box (All the lines in the text box)
	void TextBox::Render() {
		for (size_t index = 0; index < texts.size(); ++index) {
			RenderSystem::AddTextToBatch(RenderSystem::UI_BATCH, texts[index].pos.x, texts[index].pos.y,
				FontManager::GetFont(FontManager::SHIN_GO), (int)fontSize, texts[index].text, 5, color);
		}
	}
}
