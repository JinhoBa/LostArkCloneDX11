#include "Collider.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "Bounding.h"

CCollider::CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent{ pDevice, pContext }
{
}

CCollider::CCollider(const CCollider& Prototype)
	: CComponent{ Prototype },
	m_eType{Prototype.m_eType},
	m_isColl{Prototype.m_isColl },
	m_PreisColl{Prototype.m_PreisColl }
#ifdef _DEBUG
	, m_pBatch{Prototype.m_pBatch},
	m_pEffect{Prototype.m_pEffect},
	m_pInputLayout{Prototype.m_pInputLayout}
#endif // _DEBUG

{
#ifdef _DEBUG
	Safe_AddRef(m_pInputLayout);
#endif // _DEBUG

}

void CCollider::Hit(_bool isColl)
{
	m_isColl = isColl;
}

void CCollider::Hurt(CCollider* pCollider)
{
	m_isColl = true;

	m_Collider_Desc.pHitObject = pCollider->m_Collider_Desc.pOwner;
	memcpy(&m_Collider_Desc.vCollPosition, &m_pBounding->Get_WorldPosition(), sizeof(_float3));

	XMStoreFloat3(&m_Collider_Desc.vCollNormal,
		XMLoadFloat3(&pCollider->m_pBounding->Get_WorldPosition()) - XMLoadFloat3(&m_Collider_Desc.vCollPosition));
}

HRESULT CCollider::Initialize_Prototype(COLLIDER eType)
{
	m_eType = eType;



#ifdef _DEBUG
	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);
	m_pEffect = new BasicEffect(m_pDevice);
	m_pEffect->SetVertexColorEnabled(true);


	const void* pShaderByteCode = { nullptr };
	size_t	iShaderByteCodeLength = {};

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &iShaderByteCodeLength);

	if(FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount, pShaderByteCode, iShaderByteCodeLength, &m_pInputLayout)))
		return E_FAIL;
#endif 


	return S_OK;
}

HRESULT CCollider::Initialize(void* pArg)
{
	CBounding::BOUNDING_DESC* pDesc = static_cast<CBounding::BOUNDING_DESC*>(pArg);

	m_Collider_Desc.eColliderType = pDesc->eColliderType;
	m_Collider_Desc.pOwner = pDesc->pOwner;
	m_Collider_Desc.pHitObject = nullptr;
	m_Collider_Desc.vCollPosition = _float3(0.f, 0.f, 0.f);
	m_Collider_Desc.vCollNormal = _float3(0.f, 0.f, 0.f);

	switch (m_eType)
	{
	case Engine::COLLIDER::AABB:
		m_pBounding = CBounding_AABB::Create(m_pDevice, m_pContext, pArg);
		break;

	case Engine::COLLIDER::OBB:
		m_pBounding = CBounding_OBB::Create(m_pDevice, m_pContext, pArg);
		break;

	case Engine::COLLIDER::SPHERE:
		m_pBounding = CBounding_Sphere::Create(m_pDevice, m_pContext, pArg);
		break;
	}

	if (nullptr == m_pBounding)
		return E_FAIL;

	return S_OK;
}

void CCollider::Update(_fmatrix WorldMatrix)
{
	m_PreisColl = m_isColl;
	m_isColl = false;

	m_pBounding->Update(WorldMatrix);
}

_bool CCollider::Intersect(CCollider* pTarget)
{
	_bool bColl = {false};

	bColl = m_pBounding->Intersect(pTarget->m_eType, pTarget->m_pBounding);

	if (bColl)
	{
		m_HitObjects.push_back(pTarget->m_Collider_Desc.pOwner);
	}

	return bColl;
}

_vector CCollider::ComputePenetration(CCollider* pColldier)
{
	if (COLLIDER::SPHERE == m_eType && COLLIDER::SPHERE == pColldier->m_eType)
	{
		_vector vPositon = XMVectorSetW(XMLoadFloat3(&static_cast<CBounding_Sphere*>(m_pBounding)->Get_WorldPosition()), 1.f);
		_float fRadius = static_cast<CBounding_Sphere*>(m_pBounding)->Get_Desc()->Radius;

		_vector vSrcPositon = XMVectorSetW(XMLoadFloat3(&static_cast<CBounding_Sphere*>(pColldier->m_pBounding)->Get_WorldPosition()), 1.f);
		_float fSrcRadius = static_cast<CBounding_Sphere*>(pColldier->m_pBounding)->Get_Desc()->Radius;

		_vector vDirection = vPositon - vSrcPositon;

		_float fDepth = (fRadius + fSrcRadius) - XMVectorGetX(XMVector3Length(vDirection)) + 0.01f;

		return XMVector3Normalize(vDirection) * fDepth;
	}
	else
		return XMVectorSet(0.f, 0.f, 0.f, 0.f);
}

void CCollider::Update_OnCollision()
{
	if (true == m_isColl)
	{
		if (true == m_PreisColl && m_OnCollisionStay_Event)
			m_OnCollisionStay_Event();
		else if (false == m_PreisColl && m_OnCollisionEnter_Event)
			m_OnCollisionEnter_Event();
	}
	else
	{
		if (true == m_PreisColl && m_OnCollisionEnter_Event)
			m_OnCollisionEnter_Event();
	}
	
	m_HitObjects.clear();
}

void CCollider::Set_ColliderDesc(_float3& vCenter, _float3& vExtents, _float3 vOrientation)
{
	switch (m_eType)
	{
	case Engine::COLLIDER::AABB:
		static_cast<CBounding_AABB*>(m_pBounding)->Set_Desc(vCenter, vExtents);
		break;

	case Engine::COLLIDER::OBB:
		static_cast<CBounding_OBB*>(m_pBounding)->Set_Desc(vCenter, vExtents, vOrientation);
		break;

	case Engine::COLLIDER::SPHERE:
		static_cast<CBounding_Sphere*>(m_pBounding)->Set_Desc(vCenter, vExtents.x);
		break;
	}
}

#ifdef _DEBUG

HRESULT CCollider::Render()
{
	m_pEffect->SetWorld(XMMatrixIdentity());
	m_pEffect->SetView(m_pGameInstance->Get_Transfrom_Matrix(D3DTS::VIEW));
	m_pEffect->SetProjection(m_pGameInstance->Get_Transfrom_Matrix(D3DTS::PROJ));

	m_pContext->IASetInputLayout(m_pInputLayout);
	m_pEffect->Apply(m_pContext);

	return m_pBounding->Render(m_pBatch,true == m_isColl ? XMVectorSet(1.f, 0.f, 0.f, 1.f) :XMVectorSet(0.f, 1.f, 0.f, 1.f));
}
#endif

CCollider* CCollider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, COLLIDER eType)
{
	CCollider* pInstance = new CCollider(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eType)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create : CCollider");
		return nullptr;
	}

	return pInstance;
}

CComponent* CCollider::Clone(void* pArg)
{
	CComponent* pInstance = new CCollider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed to Clone : CCollider");
		return nullptr;
	}

	return pInstance;
}

void CCollider::Free()
{
	__super::Free();

#ifdef _DEBUG
	if (false == m_isCloned)
	{
		Safe_Delete(m_pBatch);
		Safe_Delete(m_pEffect);
	}

	Safe_Release(m_pInputLayout);
#endif // _DEBUG

	Safe_Release(m_pBounding);
}