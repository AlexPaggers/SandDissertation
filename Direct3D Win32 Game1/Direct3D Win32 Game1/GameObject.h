#pragma once
#include "SimpleMath.h"
#include "GameData.h"
#include "GeometricPrimitive.h"
#include "PrimitiveBatch.h"
#include <vector>

using namespace DirectX::SimpleMath;

class GameObject
{
public:
	GameObject(	ID3D11DeviceContext1* _d3dDevice,
				int _ID,
				float _friction, 
				Vector3 _dir, 
				float _diameter,
				float _mass,
				Color _color);
	virtual ~GameObject() = default;

	void tick(GameData* _GD);

	void AddCollidedObject(int _objectID);
	void ClearCollidedObjects();

	bool CheckForPreviousCollision(int _objectID);

	//Getters

	Matrix GetWorldMatrix() { return m_worldMat; }

	int			GetID()							{ return m_ID; }
	Vector3		GetPos()						{ return m_pos; }
	Vector3		GetVelocity()					{ return m_vel; }
	Vector3		GetBufferedVelocity()			{ return m_bufferedVel; }
	Vector3		GetAcceleration()				{ return m_acc; }
	DirectX::GeometricPrimitive* GetShape()		{ return m_shape.get(); }

	Color		getColor()						{ return m_color; }

	bool		getColliding()					{ return m_colliding; }
	float		getDiameter()					{ return m_diameter; }
	float		getMass()						{ return m_mass; }

	//Setters
	void		SetPos(Vector3 _pos)			{ m_pos = _pos; }
	void		SetVelocity(Vector3 _velocity)	{ m_vel = _velocity; }
	void		SetBufferedVelocity(Vector3 _bufferedvelocity) { m_vel = _bufferedvelocity; }
	void		SetAcceleration(Vector3 _acc)	{ m_acc = _acc; }
	void		SetColliding(bool _colliding)	{ m_colliding = _colliding; }

private:

	Matrix m_worldMat;
	Matrix m_rotMat;
	Matrix m_fudge;

	Vector3 m_pos;
	Vector3 m_vel = Vector3::Zero;
	Vector3 m_bufferedVel = Vector3::Zero;
	Vector3 m_acc = Vector3::Zero;

	int	m_ID;
	float m_diameter;
	float m_friction;
	float m_mass;
	Color m_color;

	std::vector<int> m_collidedObjects;

	bool m_colliding;

	std::unique_ptr<DirectX::GeometricPrimitive> m_shape;
	
};