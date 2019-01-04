#pragma once
#include "Creature.h"
#include "Megaman.h"

#define PRINT_SIZE TRUE
#define PRINT_SIZE_ENABLE_OBJECT TRUE


class Item : public Creature
{
private:
	float lifeTime;
	//Megaman* megaman;
public:
	Item() {};
	Item(D3DXVECTOR2 pos, int direct = 0);
	~Item() {};

	static std::vector<Item*> listItem;

	bool IsDisable();
	void Disable();
	void Enable();

	void Initialize();
	void ReInitialize(D3DXVECTOR2 pos, int direct);
	void Update(float deltatime);
	void Item::WallCollision(float collideTime, int normalX, int normalY, float deltatime, Box box);
	void OnCollision(float deltatime);
	void Draw();
	void Destroy();							//Dùng để xóa những con trỏ bên trong


	static void UpdateAll(float deltatime);
	static void DrawAll();
	static Item* CreateItem(D3DXVECTOR2 pos, int direct);
};