#pragma once
#include "GameObject.h"
#include <vector>

class Scene {
public:
	Scene();
	~Scene();

	std::vector<std::shared_ptr<GameObject>> objectList;

	void addObject(std::shared_ptr<GameObject> gameObject);

	void Update();
};

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::addObject(std::shared_ptr<GameObject> gameObject) {
	objectList.push_back(gameObject);
}

void Scene::Update()
{
	for (std::shared_ptr<GameObject> gameObject : objectList)
	{
		if (gameObject)
		{ 
			gameObject->Update();
		}
	}
}
