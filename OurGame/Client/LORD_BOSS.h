#pragma once
#include "Monster.h"
class LORD_BOSS_ROLL;
class LORD_BOSS :
    public Monster
{
public:
	LORD_BOSS();
	virtual ~LORD_BOSS();
	LORD_BOSS* Clone()
	{
		LORD_BOSS* pLORD_BOSS = GameObject::Clone<LORD_BOSS>();
		return pLORD_BOSS;
	}

	virtual void SetBehaviorTree();

public:
	virtual void Initialize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void FinalUpdate();
	virtual void Render();
	virtual void Destroy();

public:
	virtual void OnCollisionEnter(Collider* _pOtherCollider); //���̳���,����ƽ�� �浹 �Ͼ���� ���Լ���
	virtual void OnCollisionStay(Collider* _pOtherCollider);
	virtual void OnCollisionExit(Collider* _pOtherCollider);

	virtual void OnTriggerEnter(Collider* _pOtherCollider); //Ű��ƽ�� �浹���Ͼ���� ���Լ���
	virtual void OnTriggerStay(Collider* _pOtherCollider);
	virtual void OnTriggerExit(Collider* _pOtherCollider);

public:
	void SetObject(LORD_BOSS_ROLL* _obj) { pObject = _obj; }

private:
	

private:
	Vec3 Dir;
	Vec3 PrevDir;
	int Health;

	MonsterBasicState PrevState;

	LORD_BOSS_ROLL* pObject;
};
