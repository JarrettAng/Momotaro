#pragma once

class GameObject {
	public:
		virtual void Start();
		
		virtual void Update();
		virtual void FixedUpdate();

		virtual void Render();

		virtual void Destroy();
};
