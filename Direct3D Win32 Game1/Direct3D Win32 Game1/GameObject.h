#pragma once
#include "SimpleMath.h"
#include "GameData.h"
#include "GeometricPrimitive.h"
#include "PrimitiveBatch.h"

using namespace DirectX::SimpleMath;

class GameObject
{
public:
	GameObject(ID3D11DeviceContext1* _d3dDevice, float _friction, Vector3 _dir, float _diameter, Color _color);
	virtual ~GameObject() = default;

	void tick(GameData* _GD);

public:

	//Getters

	Matrix GetWorldMatrix() { return m_worldMat; }

	Vector3		GetPos() { return m_pos; }
	Vector3		GetVelocity() { return m_vel; }
	Vector3		GetAcceleration() { return m_acc; }
	DirectX::GeometricPrimitive* GetShape() { return m_shape.get(); }

	Color getColor() { return m_color; }


	//Setters
	void		SetPos(Vector3 _pos) { m_pos = _pos; }
	void		SetVelocity(Vector3 _velocity) { m_vel = _velocity; }
	void		SetAcceleration(Vector3 _acc) { m_acc = _acc; }

private:

	Matrix m_worldMat;
	Matrix m_rotMat;
	Matrix m_fudge;

	Vector3 m_pos;
	Vector3 m_vel = Vector3::Zero;
	Vector3 m_acc = Vector3::Zero;

	float m_friction;
	Color m_color;

	std::unique_ptr<DirectX::GeometricPrimitive> m_shape;
	
};