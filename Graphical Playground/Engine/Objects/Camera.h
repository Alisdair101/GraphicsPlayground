#pragma once

////////// Dependencies //////////
// Library Dependencies
#include <DirectXMath.h>

// Project Dependencies
#include "IObject.h"
#include "../Managers/System Managers/WindowManager.h"

struct CameraConfig : IObjectConfig
{
	CameraConfig() {};
	virtual ~CameraConfig() {};

	std::shared_ptr<WindowManager> m_WindowMgr;

	DirectX::XMFLOAT4 m_Up;
	DirectX::XMFLOAT4 m_LookAt;
	DirectX::XMFLOAT4 m_Position;
};

class Camera : IObject
{
public:
	Camera();
	virtual			~Camera();

protected:
	void                Reset();
	void                Destroy();

	// Parent Functionality
public:
	virtual HRESULT     Initialise(std::shared_ptr<IObjectConfig> config);
	virtual void		UpdateViewProjection();

	virtual DirectX::XMMATRIX GetView() { return m_View; }
	virtual DirectX::XMMATRIX GetProjection() { return m_Projection; }

private:
	std::shared_ptr<WindowManager> m_WindowMgr;

	DirectX::XMVECTOR m_Position;
	DirectX::XMVECTOR m_LookAt;
	DirectX::XMVECTOR m_Up;

	DirectX::XMMATRIX m_View;
	DirectX::XMMATRIX m_Projection;
};

