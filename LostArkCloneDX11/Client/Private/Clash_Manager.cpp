#include "pch.h"
#include "Clash_Manager.h"

#include "GameInstance.h"
#include "GameManager.h"

#include "GameObject.h"

#include "ClashUI.h"

CClash_Manager::CClash_Manager()
    : m_pGameInstance{ CGameInstance::GetInstance() }
    , m_pGameManager{ CGameManager::GetInstance() }
    , m_iNumSuccess{ 0 } 
    , m_iPatternIndex{ 0 }
    , m_iIndex{ 0 }
    , m_fPlayTime{0.f}
    , m_fCreateTime{ 0.f }
{
    Safe_AddRef(m_pGameInstance);
    Safe_AddRef(m_pGameManager);
}

HRESULT CClash_Manager::Initilize()
{
    Ready_Data();

    m_isFinish = false;

    return S_OK;
}

_uint CClash_Manager::Update_Clash(_float fTimeDelta)
{
    if (0.f == m_fPlayTime)
    {
        m_iPatternIndex = 1.5f < m_pGameInstance->Random(1, 2) ? 0 : 1;
        m_iNumSuccess = 0;
        m_iIndex = 0;
        m_fCreateTime = 0.f;
    }

    m_fPlayTime += fTimeDelta;
    m_fCreateTime += fTimeDelta;
   
    Add_ClashUI();

    /* 입력 체크 */
    if (0 != m_iIndex)
    {
        _bool isSuccess = false;
        if (!m_pGameInstance->Get_LayerObjects(ENUM_TO_INT(LEVEL::BOSS), TEXT("Layer_ClashUI")).empty())
        {
            isSuccess = dynamic_cast<CClashUI*>(m_pGameInstance->Get_LayerObjects(ENUM_TO_INT(LEVEL::BOSS), TEXT("Layer_ClashUI")).front())->Check_Circle();
           
            if (true == isSuccess)
                m_iNumSuccess++;
        }
        else
        {
            m_isFinish = true;
            m_fPlayTime = 0.f;

            /* 성공 여부 반환 */
            if (2 < m_iNumSuccess)
                return 1;
            else
                return 2;
        }

       
    }

    return 0;
}

void CClash_Manager::Add_ClashUI()
{
    /* 추가 */
    if ((_uint)m_Datas[m_iPatternIndex].size() > m_iIndex && m_fCreateTime >= m_Datas[m_iPatternIndex][m_iIndex].fOffsetTime )
    {
        CClashUI::CLASH_UI_DESC Desc = {};
        Desc.vPosition = m_Datas[m_iPatternIndex][m_iIndex].vPosition;
        Desc.iKey = m_Datas[m_iPatternIndex][m_iIndex].iKey;

        if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(
            ENUM_TO_INT(LEVEL::BOSS), TEXT("Prototype_GameObject_ClashUI"), ENUM_TO_INT(LEVEL::BOSS), TEXT("Layer_ClashUI"), &Desc)))
            return;

        m_iIndex++;
        m_fCreateTime = 0.f;
    }
}

void CClash_Manager::Ready_Data()
{
    CLASH_DATA Desc = {};
    /*0번*/
    Desc.iKey = DIK_Q;
    Desc.fOffsetTime = 0.5f;
    Desc.vPosition = _float2(250.f, 360.f);

    m_Datas[0].push_back(Desc);

    Desc.iKey = DIK_W;
    Desc.fOffsetTime = 0.5f;
    Desc.vPosition = _float2(500.f, 360.f);

    m_Datas[0].push_back(Desc);

    Desc.iKey = DIK_E;
    Desc.fOffsetTime = 0.5f;
    Desc.vPosition = _float2(750.f, 360.f);

    m_Datas[0].push_back(Desc);

    Desc.iKey = DIK_R;
    Desc.fOffsetTime = 0.5f;
    Desc.vPosition = _float2(1000.f, 360.f);

    m_Datas[0].push_back(Desc);

    /*1번*/
    Desc.iKey = DIK_Q;
    Desc.fOffsetTime = 0.5f;
    Desc.vPosition = _float2(250.f, 200.f);

    m_Datas[1].push_back(Desc);

    Desc.iKey = DIK_Q;
    Desc.fOffsetTime = 0.5f;
    Desc.vPosition = _float2(400.f, 300.f);

    m_Datas[1].push_back(Desc);

    Desc.iKey = DIK_Q;
    Desc.fOffsetTime = 0.5f;
    Desc.vPosition = _float2(550.f, 400.f);

    m_Datas[1].push_back(Desc);

    Desc.iKey = DIK_R;
    Desc.fOffsetTime = 1.0f;
    Desc.vPosition = _float2(900.f, 550.f);

    m_Datas[1].push_back(Desc);

    Desc.iKey = DIK_E;
    Desc.fOffsetTime = 1.0f;
    Desc.vPosition = _float2(800.f, 600.f);

    m_Datas[1].push_back(Desc);

}

CClash_Manager* CClash_Manager::Create()
{
    CClash_Manager* pInstance = new CClash_Manager();

    if (FAILED(pInstance->Initilize()))
        return nullptr;

    return pInstance;
}

void CClash_Manager::Free()
{
    __super::Free();

    m_Datas[0].clear();
    m_Datas[1].clear();

    Safe_Release(m_pGameInstance);
    Safe_Release(m_pGameManager);
}
