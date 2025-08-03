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
	// ���� ���ҽ� ������ ���� ��ü 
	map<TCHAR*, FMOD::Sound*> m_mapSound;
	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD::Channel* m_pChannelArr[ENUM_TO_INT(CHANNELID::END)];
	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD::System* m_pSystem;

private:
	void LoadSoundFile();

public:
	static	CSound_Manager* Create();
	virtual void			Free() override;

};
NS_END
