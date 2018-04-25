//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include <time.h>

extern void ExitGame();

using namespace DirectX;

#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

Game::Game() :
    m_window(nullptr),
    m_outputWidth(800),
    m_outputHeight(600),
    m_featureLevel(D3D_FEATURE_LEVEL_9_1)
{
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
	m_GD = std::make_unique<GameData>();
	
	m_collisionManager = std::make_unique<CollisionManager>();

	float AR = (float)width / (float)height;
	m_cam = new Camera(0.25f * XM_PI, AR, 1.0f, 10000.0f, Vector3::UnitY, Vector3::Zero);



	m_window = window;
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    CreateDevice();

    CreateResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */
}

// Executes the basic game loop.
void Game::Tick()
{
	m_cam->SetPos(Vector3(getcamerax(), getcameray(), getcameraz()));

	DWORD currentTime = GetTickCount();
	m_GD->m_dt =  std::min((float)(currentTime - m_totalplayTime) / 1000.0f, 0.1f);
	m_totalplayTime = currentTime;


	m_timer.Tick([&]()
    {
        Update(m_timer);
    });


	m_cam->Tick(m_GD.get());

	m_collisionManager->CheckCollisions();
	m_collisionManager->UpdateCollisions();

	for (auto& object : m_objects)
	{
		object->tick(m_GD.get());
		m_world *= object->GetWorldMatrix();
	}



	m_world *= m_cam->GetWorldMatrix();

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    float elapsedTime = float(timer.GetElapsedSeconds());

    // TODO: Add your game logic here.

	float time = float(timer.GetTotalSeconds());


    elapsedTime;
}

// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    // TODO: Add your rendering code here.
	for (auto& object : m_objects)
	{
		object->GetShape()->Draw(object->GetWorldMatrix(), m_cam->GetView(), m_cam->GetProj(), object->getColor());
	}

	//m_shape->Draw(m_world, m_view, m_proj);
	m_d3dContext->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	m_d3dContext->OMSetDepthStencilState(m_states->DepthNone(), 0);
	m_d3dContext->RSSetState(m_states->CullNone());

	m_effect->SetWorld(m_world);

	m_effect->Apply(m_d3dContext.Get());

	m_d3dContext->IASetInputLayout(m_inputLayout.Get());

	m_batch->Begin();

	Vector3 xaxis(2.f, 0.f, 0.f);
	Vector3 yaxis(0.f, 0.f, 2.f);
	Vector3 origin = Vector3::Zero;

	size_t divisions = 20;

	for (size_t i = 0; i <= divisions; ++i)
	{
		float fPercent = float(i) / float(divisions);
		fPercent = (fPercent * 2.0f) - 1.0f;

		Vector3 scale = xaxis * fPercent + origin;

		VertexPositionColor v1(scale - yaxis, Colors::White);
		VertexPositionColor v2(scale + yaxis, Colors::White);
		m_batch->DrawLine(v1, v2);
	}

	for (size_t i = 0; i <= divisions; i++)
	{
		float fPercent = float(i) / float(divisions);
		fPercent = (fPercent * 2.0f) - 1.0f;

		Vector3 scale = yaxis * fPercent + origin;

		VertexPositionColor v1(scale - xaxis, Colors::White);
		VertexPositionColor v2(scale + xaxis, Colors::White);
		m_batch->DrawLine(v1, v2);
	}

	m_batch->End();

	m_GD->m_FPS = 1 / m_GD->m_dt;

	TwDraw();
    Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    // Clear the views.
    m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::CornflowerBlue);
    m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

    // Set the viewport.
    CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
    m_d3dContext->RSSetViewports(1, &viewport);
}

// Presents the back buffer contents to the screen.
void Game::Present()
{
    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.
    HRESULT hr = m_swapChain->Present(1, 0);

    // If the device was reset we must completely reinitialize the renderer.
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        OnDeviceLost();
    }
    else
    {
        DX::ThrowIfFailed(hr);
    }
}

// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowSizeChanged(int width, int height)
{
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    CreateResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}

void Game::AddSand()
{
	m_selectedcolor.x = m_TWselectedcolor[0];
	m_selectedcolor.y = m_TWselectedcolor[1];
	m_selectedcolor.z = m_TWselectedcolor[2];

	m_selecteddirection.x = m_TWselecteddirection[0];
	m_selecteddirection.y = m_TWselecteddirection[1];
	m_selecteddirection.z = m_TWselecteddirection[2];

	if (m_selectedsize > 0)
	{
		GameObject *_newParticle = new GameObject(	m_d3dContext.Get(), 
													m_objects.size(),
													m_selectedfriction, 
													m_selecteddirection, 
													m_selectedsize,
													m_selectedmass,
													m_selectedcolor);
		m_objects.push_back(_newParticle);
		m_collisionManager->AddObject(_newParticle);
	}

}


void TW_CALL SandCallback(void* clientData)
{
	Game* game = static_cast<Game*>(clientData);
	game->AddSand();
}

// These are the resources that depend on the device.
void Game::CreateDevice()
{
    UINT creationFlags = 0;

#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    static const D3D_FEATURE_LEVEL featureLevels [] =
    {
        // TODO: Modify for supported Direct3D feature levels



        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };

	

	m_world = Matrix::Identity;

    // Create the DX11 API device object, and get a corresponding context.
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;
    DX::ThrowIfFailed(D3D11CreateDevice(
        nullptr,                            // specify nullptr to use the default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        featureLevels,
        _countof(featureLevels),
        D3D11_SDK_VERSION,
        device.ReleaseAndGetAddressOf(),    // returns the Direct3D device created
        &m_featureLevel,                    // returns feature level of device created
        context.ReleaseAndGetAddressOf()    // returns the device immediate context
        ));

#ifndef NDEBUG
    ComPtr<ID3D11Debug> d3dDebug;
    if (SUCCEEDED(device.As(&d3dDebug)))
    {
        ComPtr<ID3D11InfoQueue> d3dInfoQueue;
        if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
        {
#ifdef _DEBUG
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
            D3D11_MESSAGE_ID hide [] =
            {
                D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
                // TODO: Add more message IDs here as needed.
            };
            D3D11_INFO_QUEUE_FILTER filter = {};
            filter.DenyList.NumIDs = _countof(hide);
            filter.DenyList.pIDList = hide;
            d3dInfoQueue->AddStorageFilterEntries(&filter);
        }
    }
#endif

    DX::ThrowIfFailed(device.As(&m_d3dDevice));
    DX::ThrowIfFailed(context.As(&m_d3dContext));

    // TODO: Initialize device dependent objects here (independent of window size).
	
	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());

	m_effect = std::make_unique<BasicEffect>(m_d3dDevice.Get());
	m_effect->SetVertexColorEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	DX::ThrowIfFailed(
		m_d3dDevice->CreateInputLayout(VertexPositionColor::InputElements,
			VertexPositionColor::InputElementCount,
			shaderByteCode, byteCodeLength,
			m_inputLayout.ReleaseAndGetAddressOf()));

	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(m_d3dContext.Get());

	//Tweak Bar Stuff

	TwInit(TW_DIRECT3D11, device.Get());
	TwWindowSize(1000, 1000);

	TwBar *myBar;
	myBar = TwNewBar("NameOfMyTweakBar");

	int* meme = (int*)m_objects.size();

	TwAddVarRW(myBar, "FPS", TW_TYPE_FLOAT, &m_GD->m_FPS, NULL);
	TwAddVarRW(myBar, "Particle Count", TW_TYPE_FLOAT, &meme, NULL);

	TwAddSeparator(myBar, "", NULL);

	TwAddVarRW(myBar, "Friction", TW_TYPE_FLOAT, &m_selectedfriction, "Min = 0, Max=1, step=0.05");
	TwAddVarRW(myBar, "Mass", TW_TYPE_FLOAT, &m_selectedmass, "Min = 0, step = 0.005");
	TwAddVarRW(myBar, "Diameter", TW_TYPE_FLOAT, &m_selectedsize, "Min = 0, step = 0.005");
	TwAddVarRW(myBar, "Colour", TW_TYPE_COLOR3F, &m_TWselectedcolor, "");
	TwAddVarRW(myBar, "Direction", TW_TYPE_DIR3F, &m_TWselecteddirection, "");
	TwAddButton(myBar, "Create Sand", SandCallback, this, "label='Create a new grain of sand'");

	TwAddSeparator(myBar, "", NULL);
	TwAddVarRW(myBar, "Box Size X", TW_TYPE_FLOAT, &m_GD->m_xWall, "Min = 5, Max=15, step=0.1");
	TwAddVarRW(myBar, "Box Size Y", TW_TYPE_FLOAT, &m_GD->m_yWall, "Min = 5, Max=15, step=0.1");
	TwAddVarRW(myBar, "Box Size Z", TW_TYPE_FLOAT, &m_GD->m_zWall, "Min = 5, Max=15, step=0.1");
	

}


// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateResources()
{
    // Clear the previous window size specific context.
    ID3D11RenderTargetView* nullViews [] = { nullptr };
    m_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
    m_renderTargetView.Reset();
    m_depthStencilView.Reset();
    m_d3dContext->Flush();

    UINT backBufferWidth = static_cast<UINT>(m_outputWidth);
    UINT backBufferHeight = static_cast<UINT>(m_outputHeight);
    DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
    DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    UINT backBufferCount = 2;

    // If the swap chain already exists, resize it, otherwise create one.
    if (m_swapChain)
    {
        HRESULT hr = m_swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

        if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
        {
            // If the device was removed for any reason, a new device and swap chain will need to be created.
            OnDeviceLost();

            // Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
            // and correctly set up the new device.
            return;
        }
        else
        {
            DX::ThrowIfFailed(hr);
        }
    }
    else
    {
        // First, retrieve the underlying DXGI Device from the D3D Device.
        ComPtr<IDXGIDevice1> dxgiDevice;
        DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

        // Identify the physical adapter (GPU or card) this device is running on.
        ComPtr<IDXGIAdapter> dxgiAdapter;
        DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

        // And obtain the factory object that created it.
        ComPtr<IDXGIFactory2> dxgiFactory;
        DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

        // Create a descriptor for the swap chain.
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.Width = backBufferWidth;
        swapChainDesc.Height = backBufferHeight;
        swapChainDesc.Format = backBufferFormat;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = backBufferCount;

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
        fsSwapChainDesc.Windowed = TRUE;

        // Create a SwapChain from a Win32 window.
        DX::ThrowIfFailed(dxgiFactory->CreateSwapChainForHwnd(
            m_d3dDevice.Get(),
            m_window,
            &swapChainDesc,
            &fsSwapChainDesc,
            nullptr,
            m_swapChain.ReleaseAndGetAddressOf()
            ));

        // This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
        DX::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER));
    }

    // Obtain the backbuffer for this window which will be the final 3D rendertarget.
    ComPtr<ID3D11Texture2D> backBuffer;
    DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

    // Create a view interface on the rendertarget to use on bind.
    DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));

    // Allocate a 2-D surface as the depth/stencil buffer and
    // create a DepthStencil view on this surface to use on bind.
    CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

    ComPtr<ID3D11Texture2D> depthStencil;
    DX::ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
    DX::ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf()));

    // TODO: Initialize windows-size dependent objects here.

	m_view = Matrix::CreateLookAt(Vector3(2.f, 2.f, 2.f),
		Vector3::Zero, Vector3::UnitY);
	m_proj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f,
		float(backBufferWidth) / float(backBufferHeight), 0.1f, 10.f);

	m_effect->SetView(m_view);
	m_effect->SetProjection(m_proj);

}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

    m_depthStencilView.Reset();
    m_renderTargetView.Reset();
    m_swapChain.Reset();
    m_d3dContext.Reset();
    m_d3dDevice.Reset();




    CreateDevice();

    CreateResources();
}