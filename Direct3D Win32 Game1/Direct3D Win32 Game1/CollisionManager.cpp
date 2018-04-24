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

void CollisionManager::CheckCollisions()
{
	if (m_objects.size() > 1)
	{
		for (auto& object1 : m_objects)
		{
			for (auto& object2 : m_objects)
			{
				if (object1 != object2)
				{
					float onedistancyboi;
					onedistancyboi = Vector3::Distance(object1->GetPos(), object2->GetPos());
					if (onedistancyboi < 
						object1->getDiameter()/2 + object2->getDiameter()/2)
					{
						//bool meme = false;

						//Vector3 baseX = object1->GetPos() - object2->GetPos();
						//baseX.Normalize();

						//Vector3 vec1 = object1->GetVelocity();

						//float X1 = baseX.Dot(vec1);

						//vec1.x = baseX * X1;

						//vecV1.x = baseX.x * X1.x;

						//vecV1.y = vecV1 - vecV1.x;

						Vector3 U1x, U1y, U2x, U2y, V1x, V1y, V2x, V2y;


						float m1, m2, x1, x2;
						Vector3 v1temp, v1, v2, v1x, v2x, v1y, v2y, baseX;

						baseX = object1->GetPos() - object2->GetPos();

						baseX.Normalize();
						v1 = object1->GetVelocity();
						x1 = baseX.Dot(v1);
						v1x = baseX * x1;
						v1y = v1 - v1x;
						m1 = 1;

						baseX = baseX*-1;
						v2 = object2->GetVelocity();
						x2 = baseX.Dot(v2);
						v2x = baseX * x2;
						v2y = v2 - v2x;
						m2 = 1;


						
						Vector3 newVel1 = v1x* (m1 - m2) / (m1 + m2);
						newVel1 += (v2x * (2 * m2) / (m1 + m2));
						newVel1 += v1y;

						Vector3 newVel2 = v1x* (2 * m1) / (m1 + m2);
						newVel2 += v2x * (m2 - m1) / (m1 + m2);
						newVel2 += v2y;

						//Vector3 newVel1 = Vector3(v1x / v2x  * 0.5f + v1y);
						//Vector3 newVel2 = Vector3(v1x + v2x / v2y);

						object1->SetVelocity(newVel1);
						object2->SetVelocity(newVel2);


					}
				}
			}
		}
	}
}

void CollisionManager::UpdateCollisions()
{

}

void CollisionManager::AddObject(GameObject * _object)
{
	m_objects.push_back(_object);
}

void CollisionManager::RemoveObject(GameObject * _object)
{
	m_objects.pop_back();
}
