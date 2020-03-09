#include "Camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

HRESULT Camera::Initialise(std::shared_ptr<IObjectConfig> config)
{
	HRESULT hr;

	std::shared_ptr<CameraConfig> cameraConfig = std::dynamic_pointer_cast<CameraConfig>(config);

	m_WindowMgr = cameraConfig->m_WindowMgr;

	m_Position = DirectX::XMVectorSet(cameraConfig->m_Position.x, cameraConfig->m_Position.y, cameraConfig->m_Position.z, cameraConfig->m_Position.w);
	m_LookAt = DirectX::XMVectorSet(cameraConfig->m_LookAt.x, cameraConfig->m_LookAt.y, cameraConfig->m_LookAt.z, cameraConfig->m_LookAt.w);
	m_Up = DirectX::XMVectorSet(cameraConfig->m_Up.x, cameraConfig->m_Up.y, cameraConfig->m_Up.z, cameraConfig->m_Up.w);

	UpdateViewProjection();

	return S_OK;
}

void Camera::Reset()
{

}

void Camera::Destroy()
{

}

void Camera::UpdateViewProjection()
{
	m_View = DirectX::XMMatrixLookAtLH(m_Position, m_LookAt, m_Up);
	m_Projection = DirectX::XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)m_WindowMgr->GetWindow(0)->GetWindowWidth() / m_WindowMgr->GetWindow(0)->GetWindowHeight(), 1.0f, 1000.0f); // TEMP hard coded values
}