/*!************************************************************************
\file:          Building.cpp
\author:		Jarrett Ang
\par DP email:	a.jiaweijarrett@digipen.edu
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief


The functions include:
-
**************************************************************************/

#include <Building.h>

Building::Building() : data{} {}	//Empty by design
Building::Building(BuildingData _data)
{
	data = _data;
}

Building::Building(
	BuildingEnum::TYPE _type, BuildingEnum::SIZE _size, BuildingEnum::LEVEL _level, BuildingEnum::ORIENTATION _orientation,
	int _SynergyResidential, int _SynergyCommercial, int _SynergyIndustrial, int _SynergyNature, std::string _name,
	std::string _desc, TextureManager::TEX_TYPE _TextureID/*, RenderSystem::SPRITE_TYPE _MeshID*/) {
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
	//data.MeshID = _MeshID;
}

void Building::LevelUp()
{
	switch (data.level)
	{
	case BuildingEnum::L1:
		data.level = BuildingEnum::L2;
		data.TextureID = static_cast<TextureManager::TEX_TYPE>(data.TextureID + 1);
		break;
	case BuildingEnum::L2:
		data.level = BuildingEnum::L3;
		std::cout << data.TextureID << '\n';
		data.TextureID = static_cast<TextureManager::TEX_TYPE>(data.TextureID + 1);
		break;
	case BuildingEnum::L3:
		std::cerr << "Error : Building cannot be leveled up anymore! Building.cpp\n";
		break;
	default:
		break;
	}
}
