/*!************************************************************************
\file:          GameObject.h
\author:		Jarrett Ang
\par DP email:	a.jiaweijarrett@digipen.edu
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief
This header file contains the class which all game objects in the scene will
inherit. The scene will be able to access the respective functions needed like
Update() or Start() during the right part of the game loop.
**************************************************************************/

#pragma once

#include <MomoMaths.h>
#include <RenderSystem.h>

class GameObject {
public:
	virtual void Start();

	virtual void Update();
	virtual void FixedUpdate();

	virtual void Render();

	virtual void Destroy();

	Vec3<int> position;
};
