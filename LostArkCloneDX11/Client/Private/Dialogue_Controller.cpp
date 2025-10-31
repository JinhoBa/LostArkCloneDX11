#include "pch.h"
#include "Dialogue_Controller.h"

#include "GameInstance.h"

#include "Player.h"
#include "Player_Dialogue.h"
#include "DialogueUI.h"

CDialogue_Controller::CDialogue_Controller()
    : m_pGameInstance{ CGameInstance::GetInstance() }
{
    Safe_AddRef(m_pGameInstance);
}

HRESULT CDialogue_Controller::Initilize()
{

    return S_OK;
}

void CDialogue_Controller::Start_Dialogue(_uint iNpcID, _fvector vNpcPosition)
{
    CPlayer_Dialogue::PLAYER_DIALOGUE_DESC Desc = {};

    Desc.vPosition = _float3(40.f, 0.f, 41.f);
    XMStoreFloat3(&Desc.vTargetPosition, vNpcPosition);

    dynamic_cast<CPlayer*>(m_pGameInstance->Get_LayerObjects(
        ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_Player")).back())->Set_State(CPlayer::DIALOGUE, &Desc);

    m_pGameInstance->Bind_Camera(TEXT("Camera_Npc"), false, 1.f);
    /* UI 변경 */

        // 2 : HoldingSkillUI
    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_TO_INT(LEVEL::TUTORIAL), TEXT("Prototype_GameObject_DialogueUI"),
        ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_Dialogue"))))
        return ;
}

void CDialogue_Controller::End_Dialogue()
{
    /* UI 다시 */
    dynamic_cast<CPlayer*>(m_pGameInstance->Get_LayerObjects(
        ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_Player")).back())->Set_State(CPlayer::IDLE);

    m_pGameInstance->Get_LayerObjects(ENUM_TO_INT(LEVEL::GAMEPLAY), TEXT("Layer_Dialogue")).back()->Set_Dead();

    m_pGameInstance->Play_Sound(L"UI_OK.wav", CHANNELID::SYSTEM, 1.f);
    m_pGameInstance->Bind_Camera(TEXT("Camera_Fix"), false, 1.f);
}

CDialogue_Controller* CDialogue_Controller::Create()
{
    CDialogue_Controller* pInstance = new CDialogue_Controller();

    if (FAILED(pInstance->Initilize()))
        return nullptr;

    return pInstance;
}

void CDialogue_Controller::Free()
{
    __super::Free();

    Safe_Release(m_pGameInstance);
}
