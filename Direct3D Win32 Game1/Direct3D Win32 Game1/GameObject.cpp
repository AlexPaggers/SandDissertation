#include "pch.h"
#include "GameObject.h"

GameObject::GameObject(ID3D11DeviceContext1 * _d3dDevice, int _ID, float _friction, Vector3 _dir, float _diameter, float _mass, Color _color)
{
	m_pos = Vector3::Zero;
	
	m_worldMat = Matrix::Identity;
	m_fudge = Matrix::Identity;

	m_shape = DirectX::GeometricPrimitive::CreateSphere(_d3dDevice, _diameter, 3);

	m_ID = _ID;
	m_diameter = _diameter;
	m_mass = _mass;
	m_friction = _friction;
	m_color = _color;

	m_vel += _dir;

	m_acc.y = -10;
	
}

void GameObject::tick(GameData*  _GD)
{
	//DO THE CHECKING THING
	if (m_pos.y <= -_GD->m_yWall ||
		m_pos.x <= -_GD->m_xWall ||
		m_pos.x >= _GD->m_xWall ||
		m_pos.z <= -_GD->m_zWall ||
		m_pos.z >= _GD->m_zWall
		)
	{
		m_colliding = true;

		if (m_pos.y <= -_GD->m_yWall)
		{
			m_vel.y = 0;
			m_pos.y = -_GD->m_yWall;
		}

		if (m_pos.x <= -_GD->m_xWall)
		{
			m_vel.x = 0;
			m_pos.x = -_GD->m_xWall;
		}

		if (m_pos.x >= _GD->m_xWall)
		{
			m_vel.x = 0;
			m_pos.x = _GD->m_xWall;
		}

		if (m_pos.z <= -_GD->m_zWall)
		{
			m_vel.z = 0;
			m_pos.z = -_GD->m_zWall;
		}

		if (m_pos.z >= _GD->m_zWall)
		{
			m_vel.z = 0;
			m_pos.z = _GD->m_zWall;
		}

	}
	else
	{
		m_colliding = false;
	}

	if (m_colliding)
	{
		m_vel += m_acc * _GD->m_dt;
		m_vel *=  (1 - m_friction);
		m_pos += m_vel * _GD->m_dt;
	}
	else 
	{
		m_vel += m_acc * _GD->m_dt;
		m_pos += m_vel * _GD->m_dt;
	}

	Matrix  transMat = Matrix::CreateTranslation(m_pos);
	m_worldMat = m_fudge * transMat;

	//m_acc = Vector3::Zero;


}

void GameObject::AddCollidedObject(int _objectID)
{
	m_collidedObjects.push_back(_objectID);
}

void GameObject::ClearCollidedObjects()
{
	m_collidedObjects.clear();
}

bool GameObject::CheckForPreviousCollision(int _objectID)
{
	for (auto& _ID : m_collidedObjects)
	{
		if (_ID == _objectID)
		{
			return true;
		}
	}

	return false;
}
