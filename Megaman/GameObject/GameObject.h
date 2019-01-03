#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <memory>
#include "../Sprite.h"
#include "../Utility/TagMethod.h"
#include <exception>
#include "../Component/MoveComponent.h"

enum eDirection
{
	Left,
	Right,
	Top,
	Down
};

class GameObject
{
private:
	//Tọa độ
	D3DXVECTOR2 position;
	//Vector vận tốc
	D3DXVECTOR2 velocity;
	//Kiểm tra Object đã chuẩn bị để bị xóa chưa
	bool deleted;
	//Chứa các Tag đánh dấu khác nhau của Object
	Tag tags;
	
	///Có khi bỏ cái này vì không cần thiết
	//vector Chứa các Object con
	std::vector<std::reference_wrapper<GameObject>> childObject;
	//Con trỏ địa chỉ Object tía. Dùng để truy cập Object tía. Dùng để làm cái Gate với Bullets 
	GameObject* parentObject;
	///

	//vector chứa toàn bộ các GameObject trừ GhostObject
	static std::vector<std::reference_wrapper<GameObject>> listObject;

	void AddChild(GameObject &object)
	{
		childObject.push_back(object);
		parentObject = &object;
	}

public:
	//Sprite
	std::unique_ptr<Sprite> sprite;

	bool bDisable;

	//Tập hợp các hàm liên quan tới các biến bên trên
	#pragma region GetSet

	D3DXVECTOR2 GetPosition()
	{
		return position;
	}

	D3DXVECTOR2* GetPosition_Ptr()
	{
		return &position;
	}

	void GetPosition_Ref(D3DXVECTOR2 &pos)			//Dùng cho trường hợp cần khi position thay đổi thì pos thay đôi luôn						
	{
		pos = position;
	}				

	void SetPosition(D3DXVECTOR2 pos)
	{
		position = pos;
	}

	void SetPosition(float x, float y)
	{
		position.x = x;
		position.y = y;
	}

	D3DXVECTOR2 GetVelocity()
	{
		return velocity;
	}

	D3DXVECTOR2* GetVelocity_Ptr()
	{
		return &velocity;
	}

	D3DXVECTOR2 GetVelocity_Ref(D3DXVECTOR2 &vec)		//Dùng cho trường hợp cần khi position thay đổi thì pos thay đôi luôn
	{
		vec = velocity;
	} 

	void SetVelocity(D3DXVECTOR2 vec)
	{
		velocity = vec;
	}

	void SetVelocity(float x, float y)
	{
		velocity.x = x;
		velocity.y = y;
	}

	bool isDeleted()
	{
		return deleted;
	}

	void SetDeleted()
	{
		deleted = true;
	}

	Tag* GetTagMethod()
	{
		return &tags;
	}

	GameObject* getChildObject_Ptr(int i)
	{
		if (i + 1 > childObject.size())
		{
			cout << "getChildObject() has no Object at " << i << endl;
			return nullptr;
		}
		return &childObject[i].get();
	}

	void SetParent(GameObject &object)
	{
		object.AddChild(*this);
	}

	GameObject* GetParent_Ptr()
	{
		return parentObject;
	}

	void DeleteChild(int i)
	{
		if (i + 1 > childObject.size())
		{
			cout << "DeleteChild() has no Object at " << i << endl;
			return;
		}
		if (!childObject[i].get().isDeleted())
			childObject[i].get().SetDeleted();
		childObject.erase(childObject.begin() + i);
	}

	static std::vector<std::reference_wrapper<GameObject>> getListObject()
	{
		return listObject;
	}

	#pragma endregion

	virtual void Initialize();
	virtual void Ghost_Initialize();				//Initialize mà không lưu lại trong listObject
	virtual void UpdateInput(float deltatime);
	virtual void Update(float deltatime);
	virtual void Draw();
	virtual void OnCollision(GameObject* object, float collideTime, int normalX, int normalY) {}		//GameObject *object: Object va chạm với object gọi hàm
																										//float collideTime : Thời gian xảy ra va chạm
																										//int normalX/normalY: Hướng xảy ra va chạm

	virtual void Destroy();							//Dùng để xóa những con trỏ bên trong

	static GameObject* FindObjectWithTag(eTag tag);

	static void DestroyProcess();					//Dùng để xóa object nào được đánh dấu tử

};