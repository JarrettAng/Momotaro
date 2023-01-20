
#pragma once

#include <string>

#include <GameObject.h>
#include <BuildingManager.h>

class Building : public virtual GameObject {
	public:
		BuildingManager::TYPE type;
		BuildingManager::SIZE size;
		BuildingManager::LEVEL level;

		int SynergyResidential;
		int SynergyCommercial;
		int SynergyIndustrial;
		int SynergyNature;

		std::string name;
		std::string desc;

		int TextureID;
		int MeshID;

		Building(BuildingManager::TYPE type, BuildingManager::SIZE size, BuildingManager::LEVEL level,
			int SynergyResidential, int SynergyCommercial, int SynergyIndustrial, int SynergyNature, std::string name, std::string desc, int TextureID, int MeshID);
};