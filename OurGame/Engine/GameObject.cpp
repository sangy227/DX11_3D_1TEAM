#include "pch.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "MonoBehavior.h"
#include "Light.h"
#include "ParticleSystem.h"
#include "Collider.h"
#include "RigidBody.h"
#include "Animator.h"
#include "Engine.h"

namespace hm
{
	GameObject::GameObject(LayerType _eLayerType)
		: Object(ObjectType::GameObject)
		, meLayerType(_eLayerType)
		, mComponents{}
		, mbDontDestroy(false)
		, mbEnable(true)
		, mbCheckFrustum(true)
	{
	}

	GameObject::GameObject(GameObject* _pOther)
		: Object(_pOther->meObjectType)
	{
	}

	GameObject::~GameObject()
	{
		if (true == IsPhysicsObject())
			gpEngine->GetPhysics()->RemoveActor(this);

		for (int i = 0; i < FIXED_COMPONENT_COUNT; ++i)
		{
			SAFE_DELETE(mComponents[i]);
		}

		for (size_t i = 0; i < mScripts.size(); ++i)
		{
			SAFE_DELETE(mScripts[i]);
		}

		mScripts.clear();
	}

	void GameObject::Initialize()
	{
		for (Component* pComponent : mComponents)
		{
			if (nullptr != pComponent)
				pComponent->Initialize();
		}

		for (MonoBehavior* pScript : mScripts)
		{
			if (nullptr != pScript)
				pScript->Initialize();
		}

	}
	void GameObject::Update()
	{
		for (Component* pComponent : mComponents)
		{
			if (nullptr != pComponent)
				pComponent->Update();
		}

		for (MonoBehavior* pScript : mScripts)
		{
			if (nullptr != pScript)
				pScript->Update();
		}
	}
	void GameObject::FixedUpdate()
	{
		for (Component* pComponent : mComponents)
		{
			if (nullptr != pComponent)
				pComponent->FixedUpdate();
		}

		for (MonoBehavior* pScript : mScripts)
		{
			if (nullptr != pScript)
				pScript->FixedUpdate();
		}
	}
	void GameObject::FinalUpdate()
	{
		for (Component* pComponent : mComponents)
		{
			if (nullptr != pComponent)
				pComponent->FinalUpdate();
		}

		for (MonoBehavior* pScript : mScripts)
		{
			if (nullptr != pScript)
				pScript->FinalUpdate();
		}
	}
	void GameObject::Render()
	{
		for (Component* pComponent : mComponents)
		{
			if (nullptr != pComponent)
				pComponent->Render();
		}

		for (MonoBehavior* pScript : mScripts)
		{
			if (nullptr != pScript)
				pScript->Render();
		}
	}
	void GameObject::Destroy()
	{
		for (Component* pComponent : mComponents)
		{
			if (nullptr != pComponent)
				pComponent->Destroy();
		}

		for (MonoBehavior* pScript : mScripts)
		{
			if (nullptr != pScript)
				pScript->Destroy();
		}
	}


	Component* GameObject::GetFixedComponent(ComponentType _eComponentType)
	{
		int index = static_cast<int>(_eComponentType);
		AssertEx(FIXED_COMPONENT_COUNT > index, L"GameObject::GetFixedComponent() - 잘못된 컴포넌트 타입 전달");
		return mComponents[index];
	}
	Transform* GameObject::GetTransform()
	{
		return static_cast<Transform*>(GetFixedComponent(ComponentType::Transform));
	}
	Camera* GameObject::GetCamera()
	{
		return static_cast<Camera*>(GetFixedComponent(ComponentType::Camera));
	}
	MeshRenderer* GameObject::GetMeshRenderer()
	{
		return static_cast<MeshRenderer*>(GetFixedComponent(ComponentType::MeshRenderer));
	}
	Light* GameObject::GetLight()
	{
		return static_cast<Light*>(GetFixedComponent(ComponentType::Light));
	}
	ParticleSystem* GameObject::GetParticleSystem()
	{
		return static_cast<ParticleSystem*>(GetFixedComponent(ComponentType::ParticleSystem));
	}
	Collider* GameObject::GetCollider()
	{
		return static_cast<Collider*>(GetFixedComponent(ComponentType::Collider));
	}
	RigidBody* GameObject::GetRigidBody()
	{
		return static_cast<RigidBody*>(GetFixedComponent(ComponentType::RigidBody));
	}
	Animator* GameObject::GetAnimator()
	{
		return static_cast<Animator*>(GetFixedComponent(ComponentType::Animator));
	}
	void GameObject::SetDontDestroyObject(const wstring& _name)
	{
		mbDontDestroy = true;
		mName = _name;
	}
	bool GameObject::IsPhysicsObject()
	{
		RigidBody* pRigidBody = GetRigidBody();
		if (nullptr == pRigidBody)
			return false;

		return pRigidBody->IsAppliedPhysics();
	}
}

