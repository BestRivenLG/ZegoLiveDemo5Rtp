#include "ZegoDeviceManager.h"
#include "Signal/ZegoSDKSignal.h"
#include "ZegoLiveDemo.h"
#include "Base/IncludeZegoLiveRoomApi.h"

ZegoDeviceManager::ZegoDeviceManager()
{
	connect(GetAVSignal(), &QZegoAVSignal::sigAudioDeviceChanged, this, &ZegoDeviceManager::OnAudioDeviceStateChanged);
	connect(GetAVSignal(), &QZegoAVSignal::sigVideoDeviceChanged, this, &ZegoDeviceManager::OnVideoDeviceStateChanged);
    
	connect(GetAVSignal(), &QZegoAVSignal::sigCaptureSoundLevelUpdate, this, &ZegoDeviceManager::OnMicCaptureSoundLevelUpdate);
	//��ʼ������ʹ����˷硢������������ͷ
	LIVEROOM::EnableMic(m_micEnabled);
	LIVEROOM::EnableSpeaker(m_speakerEnabled);
	LIVEROOM::EnableCamera(m_cameraEnabled);
	LIVEROOM::EnableCamera(m_camera2Enabled, AV::PUBLISH_CHN_AUX);
}

ZegoDeviceManager::~ZegoDeviceManager()
{

}

//�豸��ʼ��
void ZegoDeviceManager::EnumAudioDeviceList()
{
	int nDeviceCount = 0;
	AV::DeviceInfo* pDeviceList(NULL);

	m_audioDeviceList.clear();
	//��ȡ��Ƶ�豸
	pDeviceList = LIVEROOM::GetAudioDeviceList(AV::AudioDeviceType::AudioDevice_Input, nDeviceCount);
	for (int i = 0; i < nDeviceCount; i++)
	{
		QDeviceInfo info;
		info.deviceId = pDeviceList[i].szDeviceId;
		info.deviceName = pDeviceList[i].szDeviceName;

		m_audioDeviceList.append(info);
	}

	if (m_audioDeviceList.size() > 0)
	{
		m_micListIndex = 0;
		m_audioDeviceId = m_audioDeviceList.at(m_micListIndex).deviceId;
		
		emit sigMicIdChanged(m_audioDeviceId);
	}
	else
	{
		m_audioDeviceId = "";
	}

	LIVEROOM::FreeDeviceList(pDeviceList);
	pDeviceList = NULL;

}

//�豸��ʼ��
void ZegoDeviceManager::EnumVideoDeviceList()
{
	int nDeviceCount = 0;
	AV::DeviceInfo* pDeviceList(NULL);

	m_videoDeviceList.clear();
	//��ȡ��Ƶ�豸
	pDeviceList = LIVEROOM::GetVideoDeviceList(nDeviceCount);
	for (int i = 0; i < nDeviceCount; i++)
	{
		QDeviceInfo info;
		info.deviceId = pDeviceList[i].szDeviceId;
		info.deviceName = pDeviceList[i].szDeviceName;

		m_videoDeviceList.append(info);
	}
	//��������ͷ1
	if (m_videoDeviceList.size() > 0)
	{
		m_cameraListIndex = 0;
		m_videoDeviceId = m_videoDeviceList.at(m_cameraListIndex).deviceId;
		emit sigCameraIdChanged(m_videoDeviceId);
	}
	else
	{
		m_videoDeviceId = "";
	}

	//��������ͷ2
	for (int i = 0; i < nDeviceCount; i++)
	{
		if (i != m_cameraListIndex)
		{
			m_camera2ListIndex = i;
			m_videoDeviceId2 = m_videoDeviceList.at(m_camera2ListIndex).deviceId;
			emit sigCameraId2Changed(m_videoDeviceId2);
			break;
		}
	}

	if (m_camera2ListIndex == -1)
		m_videoDeviceId2 = "";

	LIVEROOM::FreeDeviceList(pDeviceList);
	pDeviceList = NULL;
}

void ZegoDeviceManager::RefreshMicIndex()
{
	m_micListIndex = -1;
	for (int i = 0; i < m_audioDeviceList.size(); i++)
		if (m_audioDeviceList.at(i).deviceId == m_audioDeviceId)
			m_micListIndex = i;
}

void ZegoDeviceManager::RefreshCameraIndex()
{
	m_cameraListIndex = -1;
	m_camera2ListIndex = -1;
	for (int k = 0; k < m_videoDeviceList.size(); k++)
	{
		if (m_videoDeviceList.at(k).deviceId == m_videoDeviceId)
			m_cameraListIndex = k;

		if (m_videoDeviceList.at(k).deviceId == m_videoDeviceId2)
			m_camera2ListIndex = k;
	}
}

//��Ƶ�豸�л�
QDeviceState ZegoDeviceManager::SetMicrophoneIdByIndex(int index)
{
	if (index >= m_audioDeviceList.size())
		return STATE_ERROR;

	m_micListIndex = index;
	m_audioDeviceId = m_audioDeviceList.at(m_micListIndex).deviceId;
	emit sigMicIdChanged(m_audioDeviceId);

	return STATE_NORMAL;
}

//��Ƶ�豸�л�
QDeviceState ZegoDeviceManager::SetCameraIdByIndex(int index)
{
	if (index >= m_videoDeviceList.size())
		return STATE_ERROR;

	//������ͷ1ѡ���������ͷ2��ID�������߽���
	if (index == m_camera2ListIndex)
	{
		int tmp_index = m_cameraListIndex;
		m_cameraListIndex = m_camera2ListIndex;
		m_camera2ListIndex = tmp_index;

		m_videoDeviceId = m_videoDeviceList.at(m_cameraListIndex).deviceId;
		emit sigCameraIdChanged(m_videoDeviceId);

		m_videoDeviceId2 = m_videoDeviceList.at(m_camera2ListIndex).deviceId;
		emit sigCameraId2Changed(m_videoDeviceId2);

		return STATE_SWAP;
	}
	else
	{
		m_cameraListIndex = index;
		m_videoDeviceId = m_videoDeviceList.at(m_cameraListIndex).deviceId;
		emit sigCameraIdChanged(m_videoDeviceId);

		return STATE_NORMAL;
	}
}

//��Ƶ�豸2�л�
QDeviceState ZegoDeviceManager::SetCameraId2ByIndex(int index)
{
	if (index >= m_videoDeviceList.size())
		return STATE_ERROR;

	//������ͷ2ѡ���������ͷ1��ID�������߽���
	if (index == m_cameraListIndex)
	{
		int tmp_index = m_cameraListIndex;
		m_cameraListIndex = m_camera2ListIndex;
		m_camera2ListIndex = tmp_index;

		m_videoDeviceId = m_videoDeviceList.at(m_cameraListIndex).deviceId;
		emit sigCameraIdChanged(m_videoDeviceId);

		m_videoDeviceId2 = m_videoDeviceList.at(m_camera2ListIndex).deviceId;
		emit sigCameraId2Changed(m_videoDeviceId2);

		return STATE_SWAP;
	}
	else
	{
		m_camera2ListIndex = index;
		m_videoDeviceId2 = m_videoDeviceList.at(m_camera2ListIndex).deviceId;
		emit sigCameraId2Changed(m_videoDeviceId2);

		return STATE_NORMAL;
	}
}

int ZegoDeviceManager::GetAudioDeviceIndex()
{
	return m_micListIndex;
}

int ZegoDeviceManager::GetVideoDeviceIndex()
{
	return m_cameraListIndex;
}

int ZegoDeviceManager::GetVideoDevice2Index()
{
	return m_camera2ListIndex;
}

//�����л�
void ZegoDeviceManager::SetMicVolume(int volume)
{
	m_micVolume = volume;

    log_string_notice(tr("SetMicVolume: %1").arg(volume).toStdString().c_str());
    LIVEROOM::SetMicDeviceVolume(qtoc(m_audioDeviceId), volume);
    if (volume ==0 && m_micEnabled)
    {
        m_micEnabled = false;
        LIVEROOM::EnableMic(false);
    }
    else if (!m_micEnabled && volume > 0)
    {
        m_micEnabled = true;
        LIVEROOM::EnableMic(true);
    }
    
}

int ZegoDeviceManager::GetMicVolume()
{
	return m_micVolume;
}

//���������л�
void ZegoDeviceManager::SetPlayVolume(int volume)
{
	m_playVolume = volume;

	LIVEROOM::SetPlayVolume(m_playVolume);
}

int ZegoDeviceManager::GetPlayVolume()
{
	return m_playVolume;
}

QVector<QDeviceInfo> ZegoDeviceManager::GetAudioDeviceList()
{
	return m_audioDeviceList;
}

QVector<QDeviceInfo> ZegoDeviceManager::GetVideoDeviceList()
{
	return m_videoDeviceList;
}

const QString& ZegoDeviceManager::GetAudioDeviceId()
{
	return m_audioDeviceId;
}

const QString& ZegoDeviceManager::GetVideoDeviceId()
{
	return m_videoDeviceId;
}

const QString& ZegoDeviceManager::GetVideoDeviceId2()
{
	return m_videoDeviceId2;
}

void ZegoDeviceManager::SetMicEnabled(bool isUse)
{
	m_micEnabled = isUse;
	LIVEROOM::EnableMic(m_micEnabled);
}

bool ZegoDeviceManager::GetMicEnabled()
{
	return m_micEnabled;
}

void ZegoDeviceManager::SetSpeakerEnabled(bool isUse)
{
	m_speakerEnabled = isUse;
	LIVEROOM::EnableSpeaker(m_speakerEnabled);
}

bool ZegoDeviceManager::GetSpeakerEnabled()
{
	return m_speakerEnabled;
}

void ZegoDeviceManager::SetCameraEnabled(bool isUse)
{
	m_cameraEnabled = isUse;
	m_camera2Enabled = isUse;

	LIVEROOM::EnableCamera(m_cameraEnabled);
	LIVEROOM::EnableCamera(m_camera2Enabled, AV::PUBLISH_CHN_AUX);
}

bool ZegoDeviceManager::GetCameraEnabled()
{
	return m_cameraEnabled && m_camera2Enabled;
}

void ZegoDeviceManager::StartMicCaptureMonitor(int cycle)
{
	SOUNDLEVEL::SetSoundLevelMonitorCycle(cycle);
	SOUNDLEVEL::StartSoundLevelMonitor();
}

void ZegoDeviceManager::StopMicCaptureMonitor()
{
	SOUNDLEVEL::StopSoundLevelMonitor();
}

void ZegoDeviceManager::OnMicCaptureSoundLevelUpdate(float soundlevel)
{
	emit sigMicCaptureSoundLevelUpdate(soundlevel);
}

void ZegoDeviceManager::OnAudioDeviceStateChanged(AV::AudioDeviceType deviceType, const QString& strDeviceId, const QString& strDeviceName, AV::DeviceState state)
{
	if (deviceType == AV::AudioDeviceType::AudioDevice_Output)
		return;

	if (state == AV::DeviceState::Device_Added)
	{
		QDeviceInfo added_device;
		added_device.deviceId = strDeviceId;
		added_device.deviceName = strDeviceName;
		m_audioDeviceList.append(added_device);

		//��0��1
		if (m_audioDeviceList.size() == 1)
		{
			m_micListIndex = 0;
			m_audioDeviceId = m_audioDeviceList.at(m_micListIndex).deviceId;
			emit sigMicIdChanged(m_audioDeviceId);

			//emit sigDeviceExist(TYPE_AUDIO);
		}

		emit sigDeviceAdded(TYPE_AUDIO, added_device.deviceName);
	}
	else if (state == AV::DeviceState::Device_Deleted)
	{
		
		for (int i = 0; i < m_audioDeviceList.size(); ++i)
		{
			if (m_audioDeviceList.at(i).deviceId != strDeviceId)
				continue;

			m_audioDeviceList.takeAt(i);

			if (m_micListIndex == i)
			{
				if (m_audioDeviceList.size() > 0)
				{
					m_audioDeviceId = m_audioDeviceList.at(0).deviceId;
					
				}
				else
				{
					m_audioDeviceId = "";
					emit sigDeviceNone(TYPE_AUDIO);
				}

				RefreshMicIndex();
				emit sigMicIdChanged(m_audioDeviceId);
			}

			emit sigDeviceDeleted(TYPE_AUDIO, i);
		}

	}

}

void ZegoDeviceManager::OnVideoDeviceStateChanged(const QString& strDeviceId, const QString& strDeviceName, AV::DeviceState state)
{
	if (state == AV::DeviceState::Device_Added)
	{
		QDeviceInfo added_device;
		added_device.deviceId = strDeviceId;
		added_device.deviceName = strDeviceName;
		m_videoDeviceList.append(added_device);

		if (m_videoDeviceList.size() == 1)
		{
			m_cameraListIndex = 0;
			m_videoDeviceId = m_videoDeviceList.at(m_cameraListIndex).deviceId;
			emit sigCameraIdChanged(m_videoDeviceId);
			emit sigDeviceExist(TYPE_VIDEO);
		}
		
		if (m_videoDeviceList.size() == 2)
		{
			m_camera2ListIndex = 1;
			m_videoDeviceId2 = m_videoDeviceList.at(m_camera2ListIndex).deviceId;
			emit sigCameraId2Changed(m_videoDeviceId2);
		}

		emit sigDeviceAdded(TYPE_VIDEO, added_device.deviceName);
	}
	else if (state == AV::DeviceState::Device_Deleted)
	{
		for (int i = 0; i < m_videoDeviceList.size(); ++i)
		{
			if (m_videoDeviceList.at(i).deviceId != strDeviceId)
				continue;

			m_videoDeviceList.takeAt(i);

			if (m_cameraListIndex == i)
			{
				//����ǰ��������ͷ���ڣ���δ������ͷ2ʹ�ã���δ��ʹ�õ�����ͷ������1��
				if (m_videoDeviceList.size() > 1)
				{
					for (int j = 0; j < m_videoDeviceList.size(); j++)
					{
						if (m_videoDeviceList.at(j).deviceId != m_videoDeviceId2)
						{
							m_videoDeviceId = m_videoDeviceList.at(j).deviceId;
							break;
						}
					}
				}
				else if(m_videoDeviceList.size() == 1)   //������ͷ2���豸��������ͷ1��
				{
					m_videoDeviceId = m_videoDeviceId2;
					m_videoDeviceId2 = "";

					emit sigCameraId2Changed(m_videoDeviceId2);
					//emit sigDeviceNone(TYPE_VIDEO);
				}
				else
				{
					m_videoDeviceId = "";
				}

				//ˢ��index
				RefreshCameraIndex();

				emit sigCameraIdChanged(m_videoDeviceId);
		    }
			else if (m_camera2ListIndex == i)
			{
				if (m_videoDeviceList.size() > 1)
				{
					for (int j = 0; j < m_videoDeviceList.size(); j++)
					{
						if (m_videoDeviceList.at(j).deviceId != m_videoDeviceId)
						{
							m_videoDeviceId2 = m_videoDeviceList.at(j).deviceId;
							break;
						}
					}
				}
				else
				{
					m_videoDeviceId2 = "";
				}

				//ˢ��index
				RefreshCameraIndex();

				emit sigCameraId2Changed(m_videoDeviceId2);

			}

			emit sigDeviceDeleted(TYPE_VIDEO, i);
		}
	}
}