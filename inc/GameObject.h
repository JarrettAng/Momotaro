#pragma once

class GameObject {
	public:
		virtual void Start();
		
		virtual void Update();
		virtual void FixedUpdate();

		virtual void Render();

		virtual void Destroy();
};

void GameObject::Start() {

}

void GameObject::Update() {

}

void GameObject::FixedUpdate() {

}

void GameObject::Render() {

}

void GameObject::Destroy() {

}
