#include "pch.h"
#include "GameObject.h"

GameObject::GameObject(ID3D11DeviceContext1 * _d3dDevice, float _friction, Vector3 _dir, float _diameter, float _mass, Color _color)
{
	m_pos = Vector3::Zero;
	
	m_worldMat = Matrix::Identity;
	m_fudge = Matrix::Identity;

	m_shape = DirectX::GeometricPrimitive::CreateSphere(_d3dDevice, _diameter, 9);

	m_diameter = _diameter;
	m_mass = _mass;
	m_friction = _friction;
	m_color = _color;

	m_vel += _dir;
	//m_acc.y = -10;
	
}

void GameObject::tick(GameData*  _GD)
{

	if (m_pos.y <= -5)
	{
		m_pos.y = -5;
		m_vel.y = 0;

		m_colliding = true;

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
