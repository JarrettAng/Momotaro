
#pragma once

#include <string>

#include <GameObject.h>

namespace BuildingEnum {
	enum TYPE {
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
}

struct BuildingData {
	BuildingEnum::TYPE type;
	BuildingEnum::SIZE size;
	BuildingEnum::LEVEL level;

	int SynergyResidential = 0;
	int SynergyCommercial = 0;
	int SynergyIndustrial = 0;
	int SynergyNature = 0;

	std::string name = "Building";
	std::string desc = "You're not suppose to see this!";

	int TextureID;
	int MeshID;
};

class Building : public virtual GameObject {
	public:
		BuildingData data;

		Building(BuildingData data);
		Building(BuildingEnum::TYPE type, BuildingEnum::SIZE size, BuildingEnum::LEVEL level,
			int SynergyResidential, int SynergyCommercial, int SynergyIndustrial, int SynergyNature, std::string name, std::string desc, int TextureID, int MeshID);
};