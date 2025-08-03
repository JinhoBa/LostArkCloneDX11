#include "Sound_Manager.h"
#include "io.h"

void CSound_Manager::Initialize()
{
	// ���带 ����ϴ� ��ǥ��ü�� �����ϴ� �Լ�
	FMOD_RESULT result = FMOD::System_Create(&m_pSystem);

	// 1. ����� ����ä�� ��, 2.�ʱ�ȭ ���, �ý��� ������ ) 
	m_pSystem->init(32, FMOD_INIT_NORMAL, NULL);

	LoadSoundFile();
	m_pSystem->setDriver(0);
}

CSound_Manager* CSound_Manager::Create()
{
	CSound_Manager* pInstance = new CSound_Manager();
	pInstance->Initialize();
	return pInstance;
}

void CSound_Manager::Free()
{
	for (auto& Mypair : m_mapSound)
	{
		delete[] Mypair.first;
		Mypair.second->release();
	}
	m_mapSound.clear();

	m_pSystem->release();
	m_pSystem->close();
}

void CSound_Manager::Play_Sound(const TCHAR* pSoundKey, CHANNELID eID, float fVolume)
{
	map<TCHAR*, FMOD::Sound*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(),
		[&](auto& iter)->bool
		{
			return !lstrcmp(pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return;

	bool bPlay = FALSE;

	FMOD_RESULT res = m_pSystem->playSound(iter->second, nullptr, FALSE, &m_pChannelArr[ENUM_TO_INT(eID)]);
	if (res != FMOD_OK) {
		printf("playSound error: %s\n", FMOD_ErrorString(res));
	}
	m_pChannelArr[ENUM_TO_INT(eID)]->setVolume(fVolume);
	m_pSystem->update();
}

void CSound_Manager::PlayBGM(const TCHAR* pSoundKey, float fVolume)
{
	// iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
	auto iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)->bool
		{
			return !lstrcmp(pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return;

	m_pSystem->playSound(iter->second, nullptr, FALSE, &m_pChannelArr[ENUM_TO_INT(CHANNELID::BGM)]);
	m_pChannelArr[ENUM_TO_INT(CHANNELID::BGM)]->setMode(FMOD_LOOP_NORMAL);
	m_pChannelArr[ENUM_TO_INT(CHANNELID::BGM)]->setVolume(fVolume);
	m_pSystem->update();
}

void CSound_Manager::StopSound(CHANNELID eID)
{
	m_pChannelArr[ENUM_TO_INT(eID)]->stop();
}

void CSound_Manager::StopAll()
{
	int iChannelArrSize = ENUM_TO_INT(CHANNELID::END);
	for (int i = 0; i < iChannelArrSize; ++i)
		m_pChannelArr[i]->stop();
}

void CSound_Manager::SetChannelVolume(CHANNELID eID, float fVolume)
{
	m_pChannelArr[ENUM_TO_INT(eID)]->setVolume(fVolume);
	m_pSystem->update();
}

void CSound_Manager::LoadSoundFile()
{
	// _finddata_t : <io.h>���� �����ϸ� ���� ������ �����ϴ� ����ü
	_finddatai64_t  fd;

	// _findfirst : <io.h>���� �����ϸ� ����ڰ� ������ ��� ������ ���� ù ��° ������ ã�� �Լ�
	intptr_t handle = _findfirst64("../Bin/Resources/Sounds/*.*", &fd);

	if (handle == -1)
		return;

	int iResult = 0;

	char szCurPath[128] = "../Bin/Resources/Sounds/";	 // ��� ���
	char szFullPath[128] = "";

	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath);

		// "../Sound/" + "Success.wav"
		strcat_s(szFullPath, fd.name);
		// "../Sound/Success.wav"

		FMOD::Sound* pSound = nullptr;
		FMOD_RESULT eRes = m_pSystem->createSound(szFullPath, FMOD_DEFAULT, 0, &pSound);

		if (eRes == FMOD_OK)
		{
			int iLength = int(strlen(fd.name) + 1);

			TCHAR* pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);

			// �ƽ�Ű �ڵ� ���ڿ��� �����ڵ� ���ڿ��� ��ȯ�����ִ� �Լ�
			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength);

			m_mapSound.emplace(pSoundKey, pSound);
		}
		//_findnext : <io.h>���� �����ϸ� ���� ��ġ�� ������ ã�� �Լ�, ���̻� ���ٸ� -1�� ����
		iResult = _findnext64(handle, &fd);
	}

	m_pSystem->update();
	_findclose(handle);
}