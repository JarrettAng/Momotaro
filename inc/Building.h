/*!************************************************************************
\file:          Building.h
\author:		Jarrett Ang
\par DP email:	a.jiaweijarrett@digipen.edu
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief
This header file contains the data of all possible buildings in the game,
information like type, size, and level. It also holds the implementation of the
building class.
**************************************************************************/

#pragma once

#include <string>
#include <GameObject.h>
#include <TextureManager.h>

namespace BuildingEnum {
	enum TYPE {
		NONE,
		RESIDENTIAL,
		COMMERCIAL,
		INDUSTRIAL,
		NATURE,
		TYPE_LENGTH
	};

	enum SIZE {
		_1X1,
		_2X1,
		_2X2,
		SIZE_LENGTH
	};

	enum LEVEL {
		L1,
		L2,
		L3,
		LEVEL_LENGTH
	};

	enum ORIENTATION {
		//Orientation goes in a anti-clockwise direction if it were normal grid!
		RIGHT,		//0 degrees
		TOP,		//90 degrees
		LEFT,		//180 degrees
		DOWN		//270 degrees
	};
}

struct BuildingData {
	BuildingEnum::TYPE type;
	BuildingEnum::SIZE size;
	BuildingEnum::LEVEL level;
	BuildingEnum::ORIENTATION orientation;

	int SynergyResidential = 0;
	int SynergyCommercial = 0;
	int SynergyIndustrial = 0;
	int SynergyNature = 0;

	std::string name = "Building";
	std::string desc = "You're not suppose to see this!";

	TextureManager::TEX_TYPE TextureID;
	//RenderSystem::SPRITE_TYPE MeshID;
};

class Building : public virtual GameObject {
public:
	BuildingData data;
	Building();
	Building(BuildingData data);
	Building(
		BuildingEnum::TYPE type, BuildingEnum::SIZE size, BuildingEnum::LEVEL level, BuildingEnum::ORIENTATION orientation,
		int SynergyResidential, int SynergyCommercial, int SynergyIndustrial, int SynergyNature,
		std::string name, std::string desc,
		TextureManager::TEX_TYPE TextureID);
	void LevelUp();
};