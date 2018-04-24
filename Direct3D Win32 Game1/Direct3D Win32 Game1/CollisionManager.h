#pragma once

#include <vector>

class GameObject;

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

	void CheckCollisions();
	void UpdateCollisions();
	void AddObject(GameObject* _object);
	void RemoveObject(GameObject* _object);

private: 

	std::vector<GameObject*> m_objects;
	

};

