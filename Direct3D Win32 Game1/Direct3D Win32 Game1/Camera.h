#pragma once

#include "SimpleMath.h"
#include "GameData.h"

using namespace DirectX::SimpleMath;

class Camera
{
public:
	Camera(float _fieldOfView, float _aspectRatio, float _nearPlaneDistance, float _farPlaneDistance, Vector3 _up = Vector3::Up, Vector3 _target = Vector3::Zero);
	~Camera();

	void Tick(GameData* _GD);


	//Getters
	Matrix GetProj() { return m_projMat; }
	Matrix GetView() { return m_viewMat; }
	Matrix GetWorldMatrix() { return m_worldMat; }

	//Setters
	void		SetPos(Vector3 _pos) { m_pos = _pos; }

protected:

	//Principle transforms/matrices for this camera
	Matrix m_projMat;
	Matrix m_viewMat;
	Matrix m_worldMat;

	//parameters for setting up a camera
	float m_fieldOfView;
	float m_aspectRatio;
	float m_nearPlaneDistance;
	float m_farPlaneDistance;

	Vector3 m_target;
	Vector3 m_up;

	Vector3 m_pos;

};

