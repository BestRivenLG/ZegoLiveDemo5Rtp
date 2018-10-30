
#ifndef ZegoSurfaceMergeApi_H
#define ZegoSurfaceMergeApi_H

#include "ZegoSurfaceMerge.h"
#include "ZegoSurfaceMergeCallback.h"
#include "ZegoSurfaceMergeDefine.h"
#include "ZegoAVView.h"

using namespace ZEGO;

class SurfaceMergeController
{
public:
	static SurfaceMergeController& getInstance()
	{
		static SurfaceMergeController instance;
		return instance;
	}

	//����ֱ��ʡ�֡�ʺ�����ͷID
	void setSurfaceSize(int surfaceWidth, int surfaceHeight);
	void setSurfaceFps(int nFps);
	void setSurfaceCameraId(const QString& cameraID);

	//������Ҫ��Ⱦ��view
	void setRenderView(void *pView);
	//���ûص�
	void setMergeCallback(SurfaceMerge::IZegoSurfaceMergeCallback *pCallback);
	//��ʼ��
	void init();
	//����ʼ��
	void uninit();
	//��ʼʹ��SurfaceMerge
	void startSurfaceMerge();

private:
	SurfaceMergeController();
	SurfaceMergeController(const SurfaceMergeController&){}
	SurfaceMergeController& operator=(const SurfaceMergeController&){}
	~SurfaceMergeController(){}

	//static SurfaceMergeController* instance;

	bool m_isRunningSurfaceMerge;
	int m_surfaceWidth;
	int m_surfaceHeight;
	int m_surfaceFps;
	QString m_surfaceCameraID;
	QZegoAVView *m_surfaceView;
};

#endif