#include "Collision.h"

Collision* Collision::instance;

Collision* Collision::getInstance()
{
	if (instance == nullptr)
	{
		instance = new Collision();
	}
	return instance;
}

bool Collision::IsIntersection(Box object1, Box object2)
{
	if (object1.position.x							< object2.position.x + object2.size.x		&&
		object1.position.x + object1.size.x			> object2.position.x					&&
		object1.position.y							< object2.position.y + object2.size.y		&&
		object1.position.y + object1.size.y			> object2.position.y
		)
		return true;
	return false;
}

Box Collision::GetBroadphaseBox(Box2D &object, float delta)
{
	Box temp;
	Box temp2 = object.GetBox();
	D3DXVECTOR2 tempv = object.GetVelocity();
	//temp.position.x = object.GetVelocity().x == 0 ? object.GetBox().position.x : object.GetBox().position.x + object.GetVelocity().x * delta;
	//temp.position.y = object.GetVelocity().y == 0 ? object.GetBox().position.y : object.GetBox().position.y + object.GetVelocity().y * delta;
	temp.position.x = tempv.x == 0 ? temp2.position.x : temp2.position.x + tempv.x * delta;
	temp.position.y = tempv.y == 0 ? temp2.position.y : temp2.position.y + tempv.y * delta;
	temp.size.x = temp2.size.x;
	temp.size.y = temp2.size.y;

	return temp;
}

float Collision::Distance(Box object1, Box object2)
{
	D3DXVECTOR2 point1;
	D3DXVECTOR2 point2;
	point1.x = object1.position.x + object1.size.x / 2;
	point1.y = object1.position.y + object1.size.y / 2;
	point2.x = object2.position.x + object2.size.x / 2;
	point2.y = object2.position.y + object2.size.y / 2;
	
	return sqrt((point2.x - point1.x)*(point2.x - point1.x) + (point2.y - point1.y)*(point2.y - point1.y));
}

void Collision::AddCheckListBox(Box2D &object, float dist)
{
	CheckBox checkBox;
	checkBox.box = object;
	checkBox.dist = dist;
	if (_CheckListBox.size() == 0)
	{
		_CheckListBox.push_back(checkBox);
		return;
	}
	for (int i = 0; i < _CheckListBox.size(); i++)
	{
		if (dist < _CheckListBox[i].dist)
		{
			_CheckListBox.insert(_CheckListBox.begin() + i, checkBox);
			return;
		}
	}
	_CheckListBox.insert(_CheckListBox.end(), checkBox);
}

void Collision::doCollision(Box2D object, float deltatime)
{
	if (_CheckListBox.size() == 0)
		return;

	for (int i = 0; i < _CheckListBox.size(); i++)
	{
		if (!IsIntersection(GetBroadphaseBox(_CheckListBox[i].box, deltatime), GetBroadphaseBox(object, deltatime)))
			continue;

		float collideTime;
		int normalX = 0;		// -1: Va chạm bên trái			1: Va chạm bên phải
		int normalY = 0;		// -1: Va chạm bên dưới			1: Va chạm bên trên
		collideTime = GetCollideTime(object, _CheckListBox[i].box, &normalX, &normalY, deltatime) / deltatime;

		object.getGameObject()->OnCollision(_CheckListBox[i].box.getGameObject(), collideTime, normalX, normalY);
	}

	_CheckListBox.clear();
}

void Collision::CollisionChecker(float deltatime)
{
	static int count = 0;
	//std::vector<reference_wrapper<Box2D>> templist = Box2D::listBox;
	std::vector<Box2D*> templist2 = Box2D::listBox_Ptr;
	for (int i = 0; i < templist2.size(); i++)
	{
		// Next nếu Box được lấy đã bị disable hoặc nó thuộc loại Static vì Static không di chuyển nên không cần xét nó với Box khác,
		// chỉ cần xét vật di chuyển với nó là được
		//if (count > 3)
		//	break;
		if (templist2[i]->GetType() == eBoxType::Static)
			continue;		
		// Xét Box thứ 2
		for (int ii = 0; ii < templist2.size(); ii++)
		{
		//	if (count > 2)
		//		break;
			count++;
			
			// Nếu Box thứ 2 đã bị disable hoặc trùng với Box1 thì next
			/*if (!templist[ii].get().isEnable())
				continue;*/
			if (i == ii)
				continue;
			// Kiểm tra nếu có thể xảy ra va chạm trong tương lai
			// Thì sẽ đưa vào danh sách các Box cần xét
			if (!IsIntersection(GetBroadphaseBox(*templist2[ii], deltatime), GetBroadphaseBox(*templist2[i], deltatime)))
				continue;
			/*if (!IsIntersection(templist2[ii]->GetBox(), templist2[i]->GetBox()))
				continue;
			*/
			// Đưa vào danh sách các box cần xét theo thứ tự xét khoảng cách xa dần với nhân vật
			AddCheckListBox(*templist2[ii], Distance(templist2[i]->GetBox(), templist2[ii]->GetBox()));
		}
		// Thuc hien va cham 		
		doCollision(*templist2[i], deltatime);
	}
	/*for (std::vector<reference_wrapper<Box2D>>::iterator i = templist.begin(); i != templist.end(); i++)
	{
		// Next nếu Box được lấy đã bị disable hoặc nó thuộc loại Static vì Static không di chuyển nên không cần xét nó với Box khác,
		// chỉ cần xét vật di chuyển với nó là được
		//if (count > 3)
		//	break;
		if (!(*i).get().isEnable() || (*i).get().GetType() == eBoxType::Static)
			continue;
		// Xét Box thứ 2
		for (std::vector<reference_wrapper<Box2D>>::iterator ii = templist.begin(); ii != templist.end(); ii++)
		{
			//	if (count > 2)
			//		break;
			count++;

			// Nếu Box thứ 2 đã bị disable hoặc trùng với Box1 thì next
			if (!(*ii).get().isEnable() || &(*ii).get() == &(*i).get())
				continue;

			// Kiểm tra nếu có thể xảy ra va chạm trong tương lai
			// Thì sẽ đưa vào danh sách các Box cần xét
			if (!IsIntersection(GetBroadphaseBox((*ii).get(), deltatime), GetBroadphaseBox((*i).get(), deltatime)))
				continue;

			// Đưa vào danh sách các box cần xét theo thứ tự xét khoảng cách xa dần với nhân vật
			AddCheckListBox((*ii).get(), Distance((*i).get().GetBox() , (*ii).get().GetBox()));
		}
		// Thuc hien va cham 		
		doCollision((*i).get(), deltatime);
	}*/
	//if (count != 0)
	//std::cout << "CollisionLoop = " << count << endl;
	count = 0;
}

float Collision::GetCollideTime(Box2D object1, Box2D object2, int* normalx, int* normaly, float deltatime)
{
	D3DXVECTOR2 vec1 = object1.GetVelocity();
	D3DXVECTOR2 vec2 = object2.GetVelocity();
	vec1.x -= vec2.x;
	vec1.y -= vec2.y;
	vec1.x *= deltatime;
	vec1.y *= deltatime;
	Box box1 = object1.GetBox();
	Box box2 = object2.GetBox();


	float dxEntry, dxExit;
	float dyEntry, dyExit;

	float txEntry, tyEntry, tEntry;
	float txExit, tyExit, tExit;

	if (vec1.x > 0)
	{
		dxEntry = box2.position.x - (box1.position.x + box1.size.x);
		dxExit = (box2.position.x + box2.size.x) - box1.position.x;
	}
	else
	{
		dxEntry = (box2.position.x + box2.size.x) - box1.position.x;
		dxExit = box2.position.x - (box1.position.x + box1.size.x);
	}

	if (vec1.y > 0)
	{
		dyEntry = box2.position.y - (box1.position.y + box1.size.y);
		dyExit = (box2.position.y + box2.size.y) - box1.position.y;
	}
	else
	{
		dyEntry = (box2.position.y + box2.size.y) - box1.position.y;
		dyExit = box2.position.y - (box1.position.y + box1.size.y);
	}

	if (vec1.x == 0)
	{
		txEntry = -std::numeric_limits<float>::infinity();
		txExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		txEntry = dxEntry / vec1.x;
		txExit = dxExit / vec1.x;
	}

	if (vec1.y == 0)
	{
		tyEntry = -std::numeric_limits<float>::infinity();
		tyExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		tyEntry = dyEntry / vec1.y;
		tyExit = dyExit / vec1.y;
	}

	tEntry = max(txEntry, tyEntry);
	tExit = min(txExit, tyExit);
	tEntry *= deltatime;
	tExit *= deltatime;

	if (tEntry			>			tExit ||
		txEntry < 0.0f && tyEntry < 0.0f ||
		txEntry > deltatime && tyEntry > deltatime)
		return deltatime;

	if (txEntry > tyEntry)
	{
		*normalx = dxEntry > 0.0f ? 1 : -1;
	}
	else
	{
		*normaly = dyEntry >= 0.0f ? 1 : -1;
	}

	return tEntry;
}

void Collision::BoxUpdater()
{
	/*for (std::vector<reference_wrapper<Box2D>>::iterator it = Box2D::listBox.begin(); it != Box2D::listBox.end(); it++)
	{
		(*it).get().SetPosition();
	}*/
	for (std::vector<Box2D*>::iterator it = Box2D::listBox_Ptr.begin(); it != Box2D::listBox_Ptr.end(); it++)
	{
		(*it)->SetPosition();
		if ((*it)->getGameObject()->GetTagMethod()->isHasTag(eTag::PlayerTag))
			std::cout << "Box Update Position Y: " << (*it)->GetBox().position.y << endl;
	}
}
