#pragma once

#include <Scene.h>

class SceneSplashscreen : public Scene {
public:
	void Load();
	void Initialize();

	void Update();
	void FixedUpdate();
	void Draw();

	void Free();
	void Unload();
};
