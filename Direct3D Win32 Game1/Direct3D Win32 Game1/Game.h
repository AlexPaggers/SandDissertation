//
// Game.h
//

#pragma once

#include "StepTimer.h"
#include "DirectXHelpers.h"
#include "GeometricPrimitive.h"
#include "Effects.h"
#include "SimpleMath.h"
#include "CommonStates.h"
#include "PrimitiveBatch.h"
#include "GameObject.h"
#include "GameData.h"
#include "Camera.h"

#include <vector>
#include <AntTweakBar.h>

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game
{
public:

    Game();

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const;

	void AddSand();

private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();
    void Present();

    void CreateDevice();
    void CreateResources();

    void OnDeviceLost();

    // Device resources.
    HWND                                            m_window;
    int                                             m_outputWidth;
    int                                             m_outputHeight;

    D3D_FEATURE_LEVEL                               m_featureLevel;
    Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext;

    Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

    // Rendering loop timer.
    DX::StepTimer                                   m_timer;

	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;


	std::unique_ptr<DirectX::CommonStates> m_states;
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_batch;

	std::vector<GameObject*> m_objects;
	std::unique_ptr<GameData> m_GD;

	Camera* m_cam;

	Color m_selectedcolor;
	float m_TWselectedcolor[3] = { 0.0f , 0.0f, 0.0f };
	float m_selectedfriction = 0;
	float m_selectedsize = 0;
	float m_TWselecteddirection[3] = { 0.0f,0.0f,0.0f };
	Vector3 m_selecteddirection;


	DWORD m_totalplayTime;

	float camera_x = 0;
	float camera_y = 0;
	float camera_z = 50;


	float getcamerax() { return camera_x; }
	void setcamerax(float _camerax) { camera_x = _camerax; }

	float getcameray() { return camera_y; }
	void setcameray(float _cameray) { camera_y = _cameray; }

	float getcameraz() { return camera_z; }
	void setcameraz(float _cameraz) { camera_z = _cameraz; }

};