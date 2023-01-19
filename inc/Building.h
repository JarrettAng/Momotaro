
#pragma once

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

		Building(BuildingManager::TYPE type, BuildingManager::SIZE size, BuildingManager::LEVEL level,
			int SynergyResidential, int SynergyCommercial, int SynergyIndustrial, int SynergyNature);
};