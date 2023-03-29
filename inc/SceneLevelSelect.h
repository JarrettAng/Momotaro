/*!************************************************************************
\file:          LevelSelect.hpp
\author:
\par DP email:
\par Course:    CSD1171B
\par Software Engineering Project
\date:          29-03-2023
\brief


The functions include:
-
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

class LevelPreview {
	public:
	LevelPreview(float x_pos, float y_pos, float x_size, float y_size, std::string const& mapFile, std::string const& _nam);
	void Render();

	private:
	RenderSystem::Transform transform;

	std::vector<int> map;
	int mapSize;

	UI::TextBox name;
};