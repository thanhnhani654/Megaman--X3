#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "../GameObject/GameObject.h"
#include <vector>

struct Box
{
	D3DXVECTOR2 position;
	D3DXVECTOR2 pivot;
	D3DXVECTOR2 size;			//x = width, y = height
};

enum eBoxType {Dynamic, Static};

class Box2D
{
private:
	/*D3DXVECTOR2 position;
	D3DXVECTOR2 velocity;*/

	GameObject *gameObject;
	Box box;
	bool deleted;
	bool bEnable;
	eBoxType type;

	
public:
	static std::vector<reference_wrapper<Box2D>> listBox;
	static std::vector<Box2D*> listBox_Ptr;
#pragma region GetSet
	GameObject* getGameObject();
	Box GetBox();
	void SetBox(float &x, float &y, float w, float h);
	void SetPosition();
	void SetPivot(D3DXVECTOR2 pivot);
	void SetPivot(float x, float y);
	void SetSize(float width, float height);
	D3DXVECTOR2 GetPosition();
	D3DXVECTOR2 GetVelocity();
	D3DXVECTOR2 GetSize();
	void SetDeleted();
	bool IsDeleted();
	void Destroy();
	void Enable();
	void Disable();
	bool isEnable();
	void SetStaticType();
	void SetDynamicType();
	eBoxType GetType();

#pragma endregion

	void DynamicInitialize(GameObject* object, float width, float height);
	void StaticInitialize(GameObject* object, float width, float height);
};