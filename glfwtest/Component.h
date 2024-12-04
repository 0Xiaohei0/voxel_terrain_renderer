#pragma once
#include <memory>

class GameObject;

class Component
{
public:
	Component();
	~Component();

	GameObject* gameobject = nullptr;

	virtual void Start();
	virtual void Update();

private:

};

Component::Component()
{
}

Component::~Component()
{
}

void Component::Start()
{
}

void Component::Update()
{
}