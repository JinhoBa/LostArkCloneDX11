#pragma once
#include "FMOD/fmod.h"
#include "Base.h"

NS_BEGIN(Engine)
class CSound_Manager : public CBase
{
private:
	CSound_Manager() = default;
	virtual ~CSound_Manager() = default;

public:
	void Initialize();

public:
	void Play_Sound(const TCHAR* pSoundKey, CHANNELID eID, float fVolume);
	void PlayBGM(const TCHAR* pSoundKey, float fVolume);
	void StopSound(CHANNELID eID);
	void StopAll();
	void SetChannelVolume(CHANNELID eID, float fVolume);

private:
	// 사운드 리소스 정보를 갖는 객체 
	map<TCHAR*, FMOD::Sound*> m_mapSound;
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD::Channel* m_pChannelArr[ENUM_TO_INT(CHANNELID::END)];
	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD::System* m_pSystem;

private:
	void LoadSoundFile();

public:
	static	CSound_Manager* Create();
	virtual void			Free() override;

};
NS_END
