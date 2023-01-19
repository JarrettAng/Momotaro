
#include <BuildingManager.h>
#include <Building.h>

Building::Building(BuildingManager::TYPE _type, BuildingManager::SIZE _size, BuildingManager::LEVEL _level,
				   int _SynergyResidential, int _SynergyCommercial, int _SynergyIndustrial, int _SynergyNature) {
	type = _type;
	size = _size;
	level = _level;

	SynergyResidential = _SynergyResidential;
	SynergyCommercial = _SynergyCommercial;
	SynergyIndustrial = _SynergyIndustrial;
	SynergyNature = _SynergyNature;
}