#pragma once
#include "Scene.h"

namespace hm
{
	class TitleScene :
		public Scene
	{
	public:
		TitleScene();
		virtual ~TitleScene();

	public:
		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void FinalUpdate();
		virtual void Render();
		virtual void Destroy();

		virtual void Enter();
		virtual void Exit();

	private:
		class Interface* mpActiveInterface;
		class Interface* mpSelectedInterface;
	};
}


