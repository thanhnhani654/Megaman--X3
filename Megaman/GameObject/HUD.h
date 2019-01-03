#pragma once
#include "GameObject.h"
#include "Camera.h"

class HUD : public GameObject
{
private:
	static HUD* instance;

	Sprite HPBar;
	Sprite HpUnit;

	Camera* _camera;
public:
	static HUD* getInstance();

	void Ghost_Initialize();			
	void Update(float deltatime);
	void Draw();
};