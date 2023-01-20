
#pragma once

namespace BuildingManager {
	enum TYPE {
		RESIDENTIAL,
		COMMERCIAL,
		INDUSTRIAL,
		NATURE,
		TYPE_LAST
	};

	enum SIZE {
		_1X1,
		_2X1,
		_2X2,
		SIZE_LAST
	};

	enum LEVEL {
		L1,
		L2,
		L3,
		LEVEL_LAST
	};

	void Initialize();
	void Clear();
}
