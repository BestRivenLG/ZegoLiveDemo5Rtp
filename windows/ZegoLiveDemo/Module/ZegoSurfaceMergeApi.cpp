#include "ZegoSurfaceMergeApi.h"
#include "Base/IncludeZegoLiveRoomApi.h"

SurfaceMergeController::SurfaceMergeController()
{
	m_surfaceWidth = 0;
	m_surfaceHeight = 0;
	m_surfaceFps = 0;
}

//����ֱ���
void SurfaceMergeController::setSurfaceSize(int surfaceWidth, int surfaceHeight)
{
	m_surfaceWidth = surfaceWidth;
	m_surfaceHeight = surfaceHeight;
}
//����֡��
void SurfaceMergeController::setSurfaceFps(int nFps)
{
	m_surfaceFps = nFps;
}
//��������ͷID
void SurfaceMergeController::setSurfaceCameraId(const QString& cameraID)
{
	m_surfaceCameraID = cameraID;
}
//������Ҫ��Ⱦ��View
void SurfaceMergeController::setRenderView(void *pView)
{
	m_surfaceView = (QZegoAVView *)pView;
}

//���ûص�
void SurfaceMergeController::setMergeCallback(SurfaceMerge::IZegoSurfaceMergeCallback *pCallback)
{
	SurfaceMerge::SetMergeCallback(pCallback);
}

//��ʼ��
void SurfaceMergeController::init()
{
	SurfaceMerge::Init();
}

//����ʼ��
void SurfaceMergeController::uninit()
{
	SurfaceMerge::UnInit();
}

//��ʼʹ��SurfaceMerge
void SurfaceMergeController::startSurfaceMerge()
{
	if (m_surfaceWidth == 0 || m_surfaceHeight == 0 || m_surfaceFps == 0)
		return;
	
	SurfaceMerge::SetFPS(m_surfaceFps);
	SurfaceMerge::SetCursorVisible(true);
	SurfaceMerge::SetSurfaceSize(m_surfaceWidth, m_surfaceHeight);

	SurfaceMerge::ZegoCaptureItem *itemList = new SurfaceMerge::ZegoCaptureItem[2];

	SurfaceMerge::ZegoCaptureItem itemCam;
	strcpy(itemCam.captureSource.deviceId, m_surfaceCameraID.toStdString().c_str());
	itemCam.captureType = SurfaceMerge::CaptureType::Camera;
	itemCam.position = { m_surfaceWidth - m_surfaceWidth / 4, m_surfaceHeight - m_surfaceHeight / 4, m_surfaceWidth / 4, m_surfaceHeight / 4 };  //����ͷĬ���������½�

	unsigned int count = 0;
	SurfaceMerge::ScreenItem *screenList = SurfaceMerge::EnumScreenList(count);
	SurfaceMerge::ZegoCaptureItem itemWin;
	for (int i = 0; i < count; i++)
	{
		if (screenList[i].bPrimary)
		{
			strcpy(itemWin.captureSource.screenName, screenList[i].szName);
			break;
		}
	}

	itemWin.captureType = SurfaceMerge::CaptureType::Screen;
	itemWin.position = { 0, 0, m_surfaceWidth, m_surfaceHeight };
	itemList[0] = itemCam;
	itemList[1] = itemWin;

	SurfaceMerge::UpdateSurface(itemList, 2);
	m_surfaceView->setSurfaceMergeView(true);
#ifdef USE_EXTERNAL_SDK
	m_surfaceView->setSurfaceMergeItemRect(itemWin, itemCam);
#endif
	SurfaceMerge::SetRenderView((void *)m_surfaceView->winId());

	delete[]itemList;
	SurfaceMerge::FreeScreenList(screenList);
}

