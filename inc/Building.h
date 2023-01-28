
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

	int SynergyResidential;
	int SynergyCommercial;
	int SynergyIndustrial;
	int SynergyNature;

	std::string name;
	std::string desc;

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