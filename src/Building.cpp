/*!************************************************************************
\file:          Building.cpp
\author:		Jarrett Ang
\par DP email:	a.jiaweijarrett@digipen.edu
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief
This source file implements the Building class defined in the header file.

The functions include:
- 3 constructors
- LevelUp
Updates the building level and updates its texture
- GetSynergyArea
Gets the synergy of the surrounding cells and stores it inside the building
- Operator << and == overloads

All content � 2023 DigiPen Institute of Technology Singapore.
All rights reserved.
**************************************************************************/

///////////////////////////////////////////////////////////////////////////
#include <Building.h>
#include <algorithm>
#include <GridManager.h>
#include <DebugManager.h>
///////////////////////////////////////////////////////////////////////////
// Constructors

// Default constructor
Building::Building() : data{} { }		// Empty by design

// Single argument constructor
Building::Building(BuildingData _data) {
	data = _data;
}

// Non-default constructor
Building::Building(
	BuildingEnum::TYPE		  _type, 
	Vec2<int>				  _size, 
	BuildingEnum::LEVEL		  _level,
	BuildingEnum::ORIENTATION _orientation,
	int _SynergyResidential, 
	int _SynergyCommercial, 
	int _SynergyIndustrial, 
	int _SynergyNature, 
	std::string _name,
	std::string _desc, 
	TextureManager::TEX_TYPE _TextureID) {

	data.type = _type;
	data.size = _size;
	data.level = _level;
	data.orientation = _orientation;

	data.SynergyResidential = _SynergyResidential;
	data.SynergyCommercial = _SynergyCommercial;
	data.SynergyIndustrial = _SynergyIndustrial;
	data.SynergyNature = _SynergyNature;

	data.name = _name;
	data.desc = _desc;
	data.TextureID = _TextureID;
}

///////////////////////////////////////////////////////////////////////////
// Member functions

// Updates the building level and updates its texture
void Building::LevelUp() {
	switch (data.level) {
	case BuildingEnum::L1:
		data.level = BuildingEnum::L2;
		data.TextureID++;
		break;
	case BuildingEnum::L2:
		data.level = BuildingEnum::L3;
		data.TextureID++;
		break;
	case BuildingEnum::L3:
		Debug::PrintError(__FILE__, __LINE__, "Building cannot be leveled up anymore!");
		break;
	default:
		break;
	}
}

// Checks if the cell exists within the given vector
bool Building::HasCellInVector(std::vector<Vec2<int>> vectorToCheck,Vec2<int> cell) {
	for(Vec2<int> _cell : vectorToCheck) {
		if (_cell == cell) return true;
	}
	return false;
}

// Get the synergy of the surrounding cells and stores it inside the building
void Building::GetSynergyArea() {
	//Check that the building cells are NOT null!
	if (buildingCells.empty()) {
		std::string debugLog;
		Debug::PrintError(__FILE__, __LINE__, "NO BUILDING CELLS TO GET AREA!");
	}

	//First we get all the building cells
	std::vector<Vec2<int>> tempVec;

	// Then for every building cell, we get the diagonal AND adjacent cells.
	for(Vec2<int> cell : buildingCells){
		for(int x{-1}; x<2; ++x){
			tempVec.push_back(cell+Vec2<int>{0,x*2});
			tempVec.push_back(cell+Vec2<int>{x*2,0});
			for(int y{-1}; y<2; ++y){
				tempVec.push_back(cell+Vec2<int>{x,y});
			}
		}
	}

	// Small hack to make a compare operator then sort the vector of vectors
	std::sort(tempVec.begin(),tempVec.end(),[](Vec2<int> a, Vec2<int> b){return a < b;}); // Once we sort it, we prune

	auto last = std::unique(tempVec.begin(),tempVec.end());
	tempVec.erase(last,tempVec.end());
}

///////////////////////////////////////////////////////////////////
// Non-member operator overloads

// << overload (Prints all data members of the building to os)
std::ostream &operator<<(std::ostream &os, BuildingData const &_data) {
	os << "Building type : "		<< _data.type <<'\n';
	os << "Size : "					<< _data.size <<'\n';
	os << "Level : "				<< _data.level << '\n';
	os << "Synergy Residential : "  << _data.SynergyResidential << '\n';
	os << "Synergy Commercial : "   << _data.SynergyCommercial << '\n';
	os << "Synergy Industrial : "   << _data.SynergyIndustrial << '\n';
	os << "Synergy Nature : "		<< _data.SynergyNature;
	os << "Name : "					<< _data.name << '\n';
	os << "Desc : "					<< _data.desc  << '\n';
	os << "Texture ID : "			<< _data.TextureID  << '\n';
    return os;
}

// == overload (If size, type, level matches return true)
bool operator==(Building const &lhs, Building const &rhs) {
    return ((lhs.data.type == rhs.data.type) && (lhs.data.level == rhs.data.level) && (lhs.data.size==rhs.data.size));
}
