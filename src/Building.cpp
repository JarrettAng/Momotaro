/*!************************************************************************
\file:          Building.cpp
\author:
\par DP email:
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief


The functions include:
-
**************************************************************************/

#include <Building.h>

Building::Building(BuildingData _data) {
	data = _data;
}

Building::Building(BuildingEnum::TYPE _type, BuildingEnum::SIZE _size, BuildingEnum::LEVEL _level,
				   int _SynergyResidential, int _SynergyCommercial, int _SynergyIndustrial, int _SynergyNature, std::string _name, std::string _desc, int _TextureID, int _MeshID) {
	data.type = _type;
	data.size = _size;
	data.level = _level;

	data.SynergyResidential = _SynergyResidential;
	data.SynergyCommercial = _SynergyCommercial;
	data.SynergyIndustrial = _SynergyIndustrial;
	data.SynergyNature = _SynergyNature;

	data.name = _name;
	data.desc = _desc;
	data.TextureID = _TextureID;
	data.MeshID = _MeshID;
}