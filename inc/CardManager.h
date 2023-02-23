/*!************************************************************************
\file:          CardManager.h
\author:		Jarrett Ang
\par DP email:	a.jiaweijarrett@digipen.edu
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief
This header file holds the data of each card in the player's hand and handles
the creation/deletion of the cards.

The functions include:
- Initialize
Draws the cards for the player to play with at the same of the game scene
- PrepareUIRenderBatch
Sends data to UIManager to render the cards
- DrawCard
Draws a draw into the player's hand of a given type, level, size
- DrawRandomCard
Draws a random draw into the player's hand
**************************************************************************/

#pragma once

#include <string>
#include <vector>

#include <Card.h>
#include <BuildingManager.h>
#include <EventSystem.h>

namespace CardManager {
	class InfoBox {
	public:
		InfoBox();
		void Render();
		void UpdateInfo(Vec2<float> cardPos_TopCenter, const Card* hoveredCard);

	private:
		// Positions of the background
		RenderSystem::Transform transform; 
		RenderSystem::Transform arrowPos;
		RenderSystem::Transform headerLine; // White line seperating header from stats

		// Texts
		UI::TextBox name;
		UI::TextBox desc;
		std::vector<UI::TextBox> sNames;
		std::vector<UI::TextBox> sScores;

		Vec4<float> color;
	};

	struct SynergyInfo {
		int score;
		std::string name;
		Vec3<float> color;

		SynergyInfo(int _score, std::string _name, Vec3<float> _color) {
			score = _score;
			name = _name;
			color = _color;
		}
	};
}

namespace CardManager {
	extern EventSystem::Event<const BuildingData*> onNewCardSelected;
	
	void Initialize();
	void Update();

	void PrepareUIRenderBatch();

	void DrawCard(BuildingEnum::TYPE type, BuildingEnum::LEVEL level);

	void DrawRandomCard(BuildingEnum::LEVEL level);
	void DrawRandomCard(BuildingEnum::TYPE type);

	void Free();
}
