#pragma once
#include "Component.h"
#include <memory>
#include <vector>
#include "Transform.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	std::shared_ptr<Transform> transform;
	std::vector<std::shared_ptr<Component>> componentList;

	void AddComponent(std::shared_ptr<Component> component);

	void Update();

private:

};

GameObject::GameObject()
{
	transform = std::make_shared<Transform>();
}

GameObject::~GameObject()
{
}

void GameObject::AddComponent(std::shared_ptr<Component> component) {
	component->gameobject = this; 
	componentList.push_back(component);
}

void GameObject::Update()
{
	for (std::shared_ptr<Component> component : componentList)
	{
		if (component)
		{
			component->Update(); 
		}
	}
}