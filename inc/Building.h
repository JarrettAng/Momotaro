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
///////////////////////////////////////////////////////////////////////////
#include <string>
#include <GameObject.h>
#include <TextureManager.h>
///////////////////////////////////////////////////////////////////////////

namespace BuildingEnum {
	///////////////////////////////////////////////////////////////////////
	// Every building in the game has a type (Including trees which is nature)
	enum TYPE {
		NONE,
		RESIDENTIAL,
		COMMERCIAL,
		INDUSTRIAL,
		NATURE,
		TYPE_LENGTH
	};
	///////////////////////////////////////////////////////////////////////
	// Every building has a level (the level goes up after merging e.g. lvl 1 > lvl 2)
	enum LEVEL {
		L1,
		L2,
		L3,
		LEVEL_LENGTH
	};
	///////////////////////////////////////////////////////////////////////
	// Orientation goes in a anti-clockwise direction if it were normal grid!
	enum ORIENTATION {
		RIGHT,		// 0   degrees
		TOP,		// 90  degrees
		LEFT,		// 180 degrees
		DOWN,		// 270 degrees
		ORIENTATION_LENGTH
	};
}

///////////////////////////////////////////////////////////////////////////
// This struct contains all information needed for a building to work for the grid & card system
// It is a data member of the building class below, which contains additional functionalities
struct BuildingData {
	BuildingEnum::TYPE		  type{};
	Vec2<int>				  size{};
	BuildingEnum::LEVEL		  level{};
	BuildingEnum::ORIENTATION orientation{};

	// How many points to gain/lose for each type surrounding *this* building
	int SynergyResidential = 0;
	int SynergyCommercial  = 0;
	int SynergyIndustrial  = 0;
	int SynergyNature      = 0;

	// Text data to draw on the card
	std::string name = "Building";
	std::string desc = "You're not suppose to see this!";

	// Which texture to use to render *this* building
	TextureManager::TEX_TYPE TextureID{};
};

///////////////////////////////////////////////////////////////////////////
// This class contains the data and logic of the all the building objects used
// by the grids and cards in the game.
class Building {
	public:
		///////////////////////////////////////////////////////////////////
		// Constructors
		Building();						// Default ctor
		Building(BuildingData data);	// Copy ctor
		Building(						// Non-default ctor
			BuildingEnum::TYPE	      type, 
			Vec2<int>				  size, 
			BuildingEnum::LEVEL		  level, 
			BuildingEnum::ORIENTATION orientation,
			int SynergyResidential, 
			int SynergyCommercial, 
			int SynergyIndustrial, 
			int SynergyNature,
			std::string name, 
			std::string desc,
			TextureManager::TEX_TYPE TextureID);

		///////////////////////////////////////////////////////////////////
		// Member functions
		
		// Updates the building level and updates its texture
		void LevelUp();

		// Checks if the cell exists within the given vector
		bool HasCellInVector(std::vector<Vec2<int>> vectorToCheck,Vec2<int> cell);

		// Get the synergy of the surrounding cells and stores it inside the building
		void GetSynergyArea();

		///////////////////////////////////////////////////////////////////
		// Data members
		BuildingData data;
		std::vector<Vec2<int>> buildingCells;
		std::vector<Vec2<int>> synergyAreaCells;
};

///////////////////////////////////////////////////////////////////
// Non-member operator overloads

// << overload (Prints all data members of the building to os)
std::ostream& operator<<(std::ostream& os, BuildingData const& _data);

// == overload (If size, type, level matches return true)
bool operator==(Building const& lhs,Building const& rhs);