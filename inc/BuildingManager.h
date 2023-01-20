
#pragma once

namespace BuildingManager {
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

	void Initialize();
	void Clear();
}
