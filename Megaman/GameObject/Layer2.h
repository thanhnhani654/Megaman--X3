#pragma once
#include "GameObject.h"

#define PRINT_SIZE TRUE
#define PRINT_SIZE_ENABLE_OBJECT TRUE


class Layer2 : public GameObject
{
private:
	float lifeTime;
	//Megaman* megaman;
public:
	Layer2() {};
	Layer2(D3DXVECTOR2 pos, int sprite = 0);
	~Layer2() {};

	static std::vector<Layer2*> listLayer2;

	//bool IsDisable();
	//void Disable();
	//void Enable();

	void Initialize(int sprite);
	//void ReInitialize(D3DXVECTOR2 pos, int direct);
	//void Update(float deltatime);
	//void Item::WallCollision(float collideTime, int normalX, int normalY, float deltatime, Box box);
	//void OnCollision(float deltatime);
	void Draw();
	//void Destroy();							//Dùng để xóa những con trỏ bên trong


	//static void UpdateAll(float deltatime);
	static void DrawAll();
	static Layer2* CreateLayer2(D3DXVECTOR2 pos, int sprite = 0);
	static Layer2* CreateLayer2(float x, float y, int sprite = 0);
};