/*!************************************************************************
\file:          LevelSelect.hpp
\author:		Jarrett Ang
\par DP email:	a.jiaweijarrett@digipen.edu
\par Course:    CSD1171B
\par Software Engineering Project
\date:          29-03-2023
\brief
This header file handles the level select scene in the game.

The functions include:
- Standard base functions from scene
- LevelPreview class declaration
**************************************************************************/

#pragma once
///////////////////////////////////////////////////////////////////////////
#include <Scene.h>
#include <UIManager.h>
#include <string>
#include <vector>
///////////////////////////////////////////////////////////////////////////

class SceneLevelSelect : public Scene {
public:
	void Load();
	void Initialize();

	void Update();
	void Draw();

	void Free();
	void Unload();
};

///////////////////////////////////////////////////////////////////////////

// The button that shows the map to load in level select
class LevelPreview {
public:
	// Non-default constructor
	LevelPreview(float x_pos, float y_pos, float x_size, float y_size, std::string const& mapFile, std::string const& _nam);
	void Render();

private:
	RenderSystem::Transform transform; // The screen position of the level preview button
	std::vector<int> map;			   // The grid display to show the map preview
	int mapSizeX;					   // The x-length of the grid
	float cellSize;					   // Screen size of each cell in the grid
	Vec2<float> startingCellPos;	   // The screen position of the top left cell
	UI::TextBox name;				   // The name of the map
};