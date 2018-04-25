#include "pch.h"
#include "CollisionManager.h"
#include "SimpleMath.h"
#include "GameObject.h"

using namespace DirectX::SimpleMath;

CollisionManager::CollisionManager()
{

}


CollisionManager::~CollisionManager()
{

}

void CollisionManager:: CollisionManager::CheckCollisions()
{
	if (m_objects.size() > 1)
	{
		for (auto& object1 : m_objects )
		{
			for (auto& object2 : m_objects)
			{
				
				if (object1 != object2 &&
					object2->CheckForPreviousCollision(object1->GetID()) == false)
				{
					float _radii = 0;

					_radii = Vector3::Distance(object1->GetPos(), object2->GetPos());

					if (_radii < object1->getDiameter()/2 + object2->getDiameter()/2)
					{

						object1->SetColliding(true);


						if (object1->GetPos().y != object2->GetPos().y)
						{
							_radii = (object1->getDiameter() / 2 + object2->getDiameter() / 2) - Vector3::Distance(object1->GetPos(), object2->GetPos());

							if (object1->GetPos().y < object2->GetPos().y)
							{
								Vector3 _direction = object2->GetPos() - object1->GetPos();
								
								_direction.Normalize();

								object2->SetPos(object2->GetPos() + _direction * (_radii));

							}
							else if (object1->GetPos().y > object2->GetPos().y)
							{
								Vector3 _direction = object1->GetPos() - object2->GetPos();

								_direction.Normalize();

								object1->SetPos(object1->GetPos() + _direction * (_radii));

							}
						}


						Vector3 U1x, U1y, U2x, U2y, V1x, V1y, V2x, V2y;


						float m1, m2, x1, x2;
						Vector3 v1temp, v1, v2, v1x, v2x, v1y, v2y, baseX;

						baseX = object1->GetPos() - object2->GetPos();

						baseX.Normalize();
						v1 = object1->GetVelocity();
						x1 = baseX.Dot(v1);
						v1x = baseX * x1;
						v1y = v1 - v1x;
						m1 = object1->getMass();

						baseX = baseX*-1;
						v2 = object2->GetVelocity();
						x2 = baseX.Dot(v2);
						v2x = baseX * x2;
						v2y = v2 - v2x;
						m2 = object2->getMass();


						
						Vector3 newVel1 = v1x* (m1 - m2) / (m1 + m2);
						newVel1 += (v2x * (2 * m2) / (m1 + m2));
						newVel1 += v1y;

						Vector3 newVel2 = v1x* (2 * m1) / (m1 + m2);
						newVel2 += v2x * (m2 - m1) / (m1 + m2);
						newVel2 += v2y;


						object1->SetBufferedVelocity(object1->GetBufferedVelocity() + newVel1);
						object2->SetBufferedVelocity(object2->GetBufferedVelocity() + newVel2);




						
						
						
						
						


						object1->AddCollidedObject(object2->GetID());

					}
					else
					{
						//object1->SetColliding(false);
					}
				}
			}
		}
	}
}

void CollisionManager::UpdateCollisions()
{
	for (auto& object : m_objects)
	{
		if (object->GetBufferedVelocity() != Vector3::Zero)
		{
			object->SetVelocity(object->GetBufferedVelocity());
			object->SetBufferedVelocity(Vector3::Zero);
			
		}

		object->ClearCollidedObjects();

	}
}

void CollisionManager::AddObject(GameObject * _object)
{
	m_objects.push_back(_object);
}

void CollisionManager::RemoveObject(GameObject * _object)
{
	m_objects.pop_back();
}
