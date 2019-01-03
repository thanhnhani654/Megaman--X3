#include "Box2D.h"

std::vector<reference_wrapper<Box2D>> Box2D::listBox;
std::vector<Box2D*> Box2D::listBox_Ptr;

void Box2D::DynamicInitialize(GameObject* object, float width, float height)
{
	gameObject = object;
	//gameObject->GetPosition_Ref(box.position);
	box.size.x = width;
	box.size.y = height;
	SetPosition();
	deleted = false;
	bEnable = true;
	type = eBoxType::Dynamic;
	listBox.push_back(*this);
	listBox_Ptr.push_back(this);
}

void Box2D::StaticInitialize(GameObject* object, float width, float height)
{
	gameObject = object;
	//gameObject->GetPosition_Ref(box.position);
	box.size.x = width;
	box.size.y = height;
	SetPosition();
	deleted = false;
	bEnable = true;
	type = eBoxType::Static;
	listBox.push_back(*this);
	listBox_Ptr.push_back(this);
}

#pragma region GetSet
GameObject* Box2D::getGameObject()
{
	return gameObject;
}

Box Box2D::GetBox()
{
	return box;
}

void Box2D::SetBox(float &x, float &y, float w, float h)
{
	box.position.x = x;
	box.position.y = y;
	box.size.x = w;
	box.size.y = h;
	box.pivot.x = 0;
	box.pivot.y = 0;
}

void Box2D::SetPosition()
{
	box.position.x = gameObject->GetPosition().x - box.pivot.x;
	box.position.y = gameObject->GetPosition().y - box.pivot.y;
}

void Box2D::SetPivot(D3DXVECTOR2 pivot)
{
	this->box.pivot = pivot;
}

void Box2D::SetPivot(float x, float y)
{
	this->box.pivot.x = x;
	this->box.pivot.y = y;
}

void Box2D::SetSize(float width, float height)
{
	box.size.x = width;
	box.size.y = height;
}

D3DXVECTOR2 Box2D::GetPosition()
{
	return gameObject->GetPosition();
}

D3DXVECTOR2 Box2D::GetVelocity()
{
	return gameObject->GetVelocity();
}

D3DXVECTOR2 Box2D::GetSize()
{
	return box.size;
}

void Box2D::SetDeleted()
{
	deleted = true;
	Destroy();
}

bool Box2D::IsDeleted()
{
	return deleted;
}

void Box2D::Destroy()
{
	int mark = -1;
	/*std::vector<reference_wrapper<Box2D>>::iterator i;
	for ((&i) = &listBox.begin(); &i != &listBox.end; &i++)
	{
		if ((*i).get().IsDeleted())
		{
			mark = 1;
			break;
		}
	}
	if (mark != -1)
	{
		listBox.erase(i);
		listBox.shrink_to_fit();
	}*/
	mark = -1;
	std::vector<Box2D*>::iterator ii;
	for (ii = listBox_Ptr.begin(); ii != listBox_Ptr.end(); ii++)
	{
		if ((*ii)->IsDeleted())
		{
			mark = 1;
			break;
		}
	}
	if (mark != -1)
	{
		listBox_Ptr.erase(ii);
		listBox_Ptr.shrink_to_fit();
	}


}

void Box2D::Enable()
{
	bEnable = true;
}

void Box2D::Disable()
{
	bEnable = false;
}

bool Box2D::isEnable()
{
	return bEnable;
}

void Box2D::SetStaticType()
{
	type = eBoxType::Static;
}

void Box2D::SetDynamicType()
{
	type - eBoxType::Dynamic;
}

eBoxType Box2D::GetType()
{
	return type;
}

#pragma endregion