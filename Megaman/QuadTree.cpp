#include "Quadtree.h"

#define _CRT_SECURE_NO_WARNINGS

std::vector<MapCollisionLinker*> Quadtree::listMapCollisionLinker;
std::vector<NotorBangerLinker*> Quadtree::listNotorBangerLinker;
std::vector<HeadHunterLinker*> Quadtree::listHeadHunterLinker;
std::vector<HelitLinker*> Quadtree::listHelitLinker;
Camera* Quadtree::camera;
Rect Quadtree::cameraRect;

int Quadtree::count = 0;

Quadtree::Quadtree(int level, Rect* region, string iid)
{
	m_level = level;
	m_region = region;
	m_nodes = nullptr;
	m_objects_list = new std::vector<Entity*>();
	loadPeriod = 1.0f;
	loadPeriodCount = 0;
	id = iid;

	if (level == 1)
		camera = Camera::getInstance();
}

void Quadtree::Split()
{
	m_nodes = new Quadtree*[4];

	string temp = id;

	temp[m_level - 1] = '1';

	m_nodes[0] = new Quadtree(m_level + 1,
		new Rect(m_region->x, m_region->y, m_region->width / 2, m_region->height / 2),temp);
	temp[m_level - 1] = '2';
	m_nodes[1] = new Quadtree(m_level + 1,
		new Rect(m_region->x + m_region->width / 2, m_region->y, m_region->width / 2, m_region->height / 2), temp);
	temp[m_level - 1] = '3';
	m_nodes[2] = new Quadtree(m_level + 1,
		new Rect(m_region->x, m_region->y + m_region->height / 2, m_region->width / 2, m_region->height / 2), temp);
	temp[m_level - 1] = '4';
	m_nodes[3] = new Quadtree(m_level + 1,
		new Rect(m_region->x + m_region->width / 2,
			m_region->y + m_region->height / 2, m_region->width / 2, m_region->height / 2), temp);
}

void Quadtree::ObjectLoader(Entity * entity)
{
	switch (entity->id)
	{
	case 1:
	case 0:
		if (PRINT_DEBUG)
			cout << "Create MapCollision" << endl;
		MapCollisionLoader(entity);
		break;
	case 2:
		if (PRINT_DEBUG)
			cout << "Create NotoBanger" << endl;
		NotorBangerLoader(entity);
		break;
	case 3:
		if (PRINT_DEBUG)
			cout << "Create HeadHunter" << endl;
		HeadHunterLoader(entity);
		break;
	case 4:
		if (PRINT_DEBUG)
			cout << "Create Helit" << endl;
		HelitLoader(entity);
		break;
	}
}

void Quadtree::Insert(Entity* entity)
{
	//Insert entity into corresponding nodes
	
	if (m_nodes)
	{
		if (!this->HoanToanNamTrongNodeKeTiep(entity))
		{
			if (this->HoanToanNamTrong(entity))
			{
				m_objects_list->push_back(entity);
			}
			return;
		}

		if (m_nodes[0]->HoanToanNamTrong(entity))
			m_nodes[0]->Insert(entity);
		if (m_nodes[1]->HoanToanNamTrong(entity))
			m_nodes[1]->Insert(entity);
		if (m_nodes[2]->HoanToanNamTrong(entity))
			m_nodes[2]->Insert(entity);
		if (m_nodes[3]->HoanToanNamTrong(entity))
			m_nodes[3]->Insert(entity);

		
		return;
	}

	//Insert entity into current quadtree
	//if (this->IsContain(entity) && this->HoanToanNamTrong(entity))
	if (this->HoanToanNamTrong(entity))
	{
		m_objects_list->push_back(entity);
		//std::cout << "ID: " << entity->id << " \tLevel: " << m_level << " \tX= " << m_region->x << " \tY= " << m_region->y << " \tWidth= " << m_region->width << " \tHeight= " << m_region->height << std::endl;
	}

	//Split and move all objects in list into it’s corresponding nodes
	if (m_objects_list->size() > MAX_OBJECT_IN_REGION && m_level < MAX_LEVEL)
	{
		Split();
		int i = 0;

		while (!m_objects_list->empty())
		{
			int temp = m_objects_list->size() - 1 - i;
			if (temp < 0)
				return;
			if (!HoanToanNamTrongNodeKeTiep(m_objects_list->at(m_objects_list->size() - 1 - i)))
			{
				i++;
				continue;
			}
			if (m_nodes[0]->IsContain(m_objects_list->at(m_objects_list->size() - 1 - i)))
				m_nodes[0]->Insert(m_objects_list->at(m_objects_list->size() - 1 - i));
			if (m_nodes[1]->IsContain(m_objects_list->at(m_objects_list->size() - 1 - i)))
				m_nodes[1]->Insert(m_objects_list->at(m_objects_list->size() - 1 - i));
			if (m_nodes[2]->IsContain(m_objects_list->at(m_objects_list->size() - 1 - i)))
				m_nodes[2]->Insert(m_objects_list->at(m_objects_list->size() - 1 - i));
			if (m_nodes[3]->IsContain(m_objects_list->at(m_objects_list->size() - 1 - i)))
				m_nodes[3]->Insert(m_objects_list->at(m_objects_list->size() - 1 - i));

			m_objects_list->pop_back();
		}
	}

	//if (!m_objects_list->empty())
	//	std::cout << "ID: " << entity->id << " \tLevel: " << m_level << " \tX= " << m_region->x << "\tY= " << m_region->y << "\tWidth= " << m_region->width << "\tHeight= " << m_region->height << std::endl;
}

void Quadtree::PrintObjectWhere()
{
	if (!m_objects_list->empty())
		for (int i = 0; i < m_objects_list->size(); i++)
		{
			std::cout << "ID: " << m_objects_list->at(i)->id << " \tLevel: " << id << " \tX= " << m_region->x << "\tY= " << m_region->y << "\tWidth= " << m_region->width << "\tHeight= " << m_region->height << "\t" << count << "\tOjectX = " << m_objects_list->at(i)->boundBox->x << " OjectY = " << m_objects_list->at(i)->boundBox->y << " OjectW = " << m_objects_list->at(i)->boundBox->width << " OjectH = " << m_objects_list->at(i)->boundBox->height << std::endl;
			count++;
		}
	std::cout << std::endl;
	/*
	f.open("quadtree.txt", ios::app);
	string data;
	data += id;

	data += ' ';

	char* temp = new char(256);
	_itoa_s(m_objects_list->size(), temp,256, 10);
	string temp2 = string(temp);
	data += temp2;
	
	for (int i = 0; i < m_objects_list->size(); i++)
	{
		data += ' ';
		_itoa_s(m_objects_list->at(i)->id, temp, 256, 10);
		temp2 = string(temp);
		data += temp2;

		data += ' ';
		_itoa_s(m_objects_list->at(i)->boundBox->x, temp, 256, 10);
		temp2 = string(temp);
		data += temp2;

		data += ' ';
		_itoa_s(m_objects_list->at(i)->boundBox->y, temp, 256, 10);
		temp2 = string(temp);
		data += temp2;

		data += ' ';
		_itoa_s(m_objects_list->at(i)->boundBox->width, temp, 256, 10);
		temp2 = string(temp);
		data += temp2;

		data += ' ';
		_itoa_s(m_objects_list->at(i)->boundBox->height, temp, 256, 10);
		temp2 = string(temp);
		data += temp2;
	}
	data += '\n';
	f << data;

	f.close();*/

	if (m_nodes)
	{
			m_nodes[0]->PrintObjectWhere();
			m_nodes[1]->PrintObjectWhere();
			m_nodes[2]->PrintObjectWhere();
			m_nodes[3]->PrintObjectWhere();

		return; // Return here to ignore rest.
	}

	
}

void Quadtree::PhanTichNodeID(string ID, Entity * entity)
{
	int result = CompareID(id, ID);

	if (result == -1)
	{
		m_objects_list->push_back(entity);
		entity->node = this;
		return;
	}

	if (!m_nodes)
	{
		Split();
	}

	m_nodes[result - 1]->PhanTichNodeID(ID, entity);
}

int Quadtree::CompareID(string nodeID, string inputID)
{
	for (int i = 0; i < 10; i++)
	{
		if (nodeID[i] != inputID[i])
		{
			char* temp = new char(1);
			temp[0] = inputID[i];
			return atoi(temp);
		}
	}
	return -1;
}

void Quadtree::TestObjectCreate()
{
	if (m_nodes)
	{
		m_nodes[0]->TestObjectCreate();
		m_nodes[1]->TestObjectCreate();
		m_nodes[2]->TestObjectCreate();
		m_nodes[3]->TestObjectCreate();
	}

	if (m_objects_list->size() == 0)
		return;
	
	for (int i = 0; i < m_objects_list->size(); i++)
	{
		MapCollision::CreateMapCollision(m_objects_list->at(i)->id,
			m_objects_list->at(i)->boundBox->x,
			m_objects_list->at(i)->boundBox->y,
			m_objects_list->at(i)->boundBox->width,
			m_objects_list->at(i)->boundBox->height);
	}
}

void Quadtree::DynamicLoad(float deltatime)
{
	/*if (loadPeriodCount > 0)
	{
		loadPeriodCount -= deltatime;
		return;
	}
	loadPeriodCount = loadPeriod;*/

	Rect rect;
	rect.x = Camera::getInstance()->GetPosition().x;
	rect.y = Camera::getInstance()->GetPosition().y;
	rect.width = Camera::getInstance()->GetSize().x;
	rect.height = Camera::getInstance()->GetSize().y;

	//cout << rect.x << "\t" << rect.y << "\t" << rect.width << "\t" << rect.height << "\t" << m_region->x << "\t"  << m_region->y << "\t" << m_region->width << "\t" << m_region->height<< endl;

	if (m_nodes)
	{
		if (m_nodes[0]->IsContain(rect))
			m_nodes[0]->DynamicLoad(deltatime);
		if (m_nodes[1]->IsContain(rect))
			m_nodes[1]->DynamicLoad(deltatime);
		if (m_nodes[2]->IsContain(rect))
			m_nodes[2]->DynamicLoad(deltatime);
		if (m_nodes[3]->IsContain(rect))
			m_nodes[3]->DynamicLoad(deltatime);
	}

	if (m_objects_list->size() > 0)
	{
		for (int i = 0; i < m_objects_list->size(); i++)
		{
			if (DONT_LOAD_NOTORBANGER)
				if (m_objects_list->at(i)->id == 2)
				{
					break;
				}

			if (DONT_LOAD_HEADGUNNER)
				if (m_objects_list->at(i)->id == 3)
				{
					break;
				}


			if (!m_objects_list->at(i)->bCreated)
			{
				Rect rect;
				rect.x = camera->GetPosition().x;
				rect.y = camera->GetPosition().y;
				rect.width = camera->GetSize().x;
				rect.height = camera->GetSize().y;

				Rect objectRect;
				objectRect.x = m_objects_list->at(i)->boundBox->x;
				objectRect.y = m_objects_list->at(i)->boundBox->y;
				objectRect.width = m_objects_list->at(i)->boundBox->width;
				objectRect.height = m_objects_list->at(i)->boundBox->height;

				if (m_objects_list->at(i)->id != 1) 
				{
					if (IsContain(rect, objectRect))
					{
						ObjectLoader(m_objects_list->at(i));
					}
				}
				else
				{
					rect.x -= 100;
					rect.width += 200;
					rect.height += 200;
					rect.y -= 100;
					if (IsContain(rect, objectRect))
					{
						ObjectLoader(m_objects_list->at(i));
					}
					rect.x += 100;
					rect.width -= 200;
					rect.height -= 200;
					rect.y += 100;
				}
			}
		}
	}
}

void Quadtree::DynamicunLoad(float deltatime)
{
	Rect rect;
	rect.x = Camera::getInstance()->GetPosition().x;
	rect.y = Camera::getInstance()->GetPosition().y;
	rect.width = Camera::getInstance()->GetSize().x;
	rect.height = Camera::getInstance()->GetSize().y;

	for (int i = 0; i < listMapCollisionLinker.size(); i++)
	{
		if (listMapCollisionLinker[i]->bDisable)
			continue;

		Rect objectRect;
		objectRect.x = listMapCollisionLinker[i]->object->GetPosition().x;
		objectRect.y = listMapCollisionLinker[i]->object->GetPosition().y;
		objectRect.width = listMapCollisionLinker[i]->object->box.GetSize().x;
		objectRect.height = listMapCollisionLinker[i]->object->box.GetSize().y;
		//if (!IsContain(objectRect, rect))
		//if (listMapCollisionLinker[i]->entity->boundBox->x == 1086)
		//	cout << listMapCollisionLinker[i]->entity->boundBox->x << "\t" << listMapCollisionLinker[i]->entity->boundBox->y << endl;
		rect.x -= 100;
		rect.width += 200;
		rect.height += 200;
		rect.y -= 100;
		if (!IsContain(*listMapCollisionLinker[i]->entity->node->m_region, rect))
		{
			if (PRINT_DEBUG)
				cout << "Delete MapCollision" << endl;
			listMapCollisionLinker[i]->Disable();
		}
		rect.x += 100;
		rect.width -= 200;
		rect.height -= 200;
		rect.y += 100;
	}

	for (int i = 0; i < listNotorBangerLinker.size(); i++)
	{
		if (listNotorBangerLinker[i]->bDisable)
			continue;

		if (listNotorBangerLinker[i]->object->bDisable)
		{
			listNotorBangerLinker[i]->bDisable = true;
			continue;
		}

		Rect objectRect;
		objectRect.x = listNotorBangerLinker[i]->object->GetPosition().x;
		objectRect.y = listNotorBangerLinker[i]->object->GetPosition().y;
		objectRect.width = listNotorBangerLinker[i]->object->box.GetSize().x;
		objectRect.height = listNotorBangerLinker[i]->object->box.GetSize().y;

		if (!IsContain(*listNotorBangerLinker[i]->entity->node->m_region, rect))
		{
			listNotorBangerLinker[i]->Disable();
			if (PRINT_DEBUG)
				cout << "Delete NotorBanger" << endl;
		}
	}

	for (int i = 0; i < listHeadHunterLinker.size(); i++)
	{
		if (listHeadHunterLinker[i]->bDisable)
			continue;

		if (listHeadHunterLinker[i]->object->IsDisable())
		{
			listHeadHunterLinker[i]->bDisable = true;
			continue;
		}
		Rect objectRect;
		objectRect.x = listHeadHunterLinker[i]->object->GetPosition().x;
		objectRect.y = listHeadHunterLinker[i]->object->GetPosition().y;
		objectRect.width = listHeadHunterLinker[i]->object->box.GetSize().x;
		objectRect.height = listHeadHunterLinker[i]->object->box.GetSize().y;

		if (!IsContain(*listHeadHunterLinker[i]->entity->node->m_region, rect))
		{
			if (PRINT_DEBUG)
				cout << "Delete HeadHunter" << endl;
			listHeadHunterLinker[i]->Disable();
		}
	}

	for (int i = 0; i < listHelitLinker.size(); i++)
	{
		if (listHelitLinker[i]->bDisable)
			continue;

		if (listHelitLinker[i]->object->IsDisable())
		{
			listHelitLinker[i]->bDisable = true;
			continue;
		}

		Rect objectRect;
		objectRect.x = listHelitLinker[i]->object->GetPosition().x;
		objectRect.y = listHelitLinker[i]->object->GetPosition().y;
		objectRect.width = listHelitLinker[i]->object->box.GetSize().x;
		objectRect.height = listHelitLinker[i]->object->box.GetSize().y;

		if (!IsContain(*listHelitLinker[i]->entity->node->m_region, rect))
		{
			if (PRINT_DEBUG)
				cout << "Delete Helit" << endl;
			listHelitLinker[i]->Disable();
		}
	}
}

void Quadtree::MapCollisionLoader(Entity * entity)
{
	MapCollision* object = MapCollision::CreateMapCollision(entity->id, entity->boundBox->x, entity->boundBox->y, entity->boundBox->width, entity->boundBox->height);
	entity->bCreated = true;

	//cout << entity->boundBox->x << "\t" << entity->boundBox->y << "\t" << entity->boundBox->width << "\t" << entity->boundBox->height << "\n";

	for (int i = 0; i < listMapCollisionLinker.size(); i++)
	{
		if (listMapCollisionLinker[i]->bDisable)
		{
			listMapCollisionLinker[i]->bDisable = false;
			listMapCollisionLinker[i]->object = object;
			listMapCollisionLinker[i]->entity = entity;
		}
	}

	MapCollisionLinker* linker = new MapCollisionLinker();
	linker->object = object;
	linker->entity = entity;
	listMapCollisionLinker.push_back(linker);
}

void Quadtree::NotorBangerLoader(Entity * entity)
{
	NotorBanger* object = NotorBanger::CreateNotorBanger(D3DXVECTOR2(entity->boundBox->x, entity->boundBox->y),entity->direction);
	entity->bCreated = true;

	//cout << entity->boundBox->x << "\t" << entity->boundBox->y << "\t" << entity->boundBox->width << "\t" << entity->boundBox->height << "\n";

	for (int i = 0; i < listNotorBangerLinker.size(); i++)
	{
		if (listNotorBangerLinker[i]->bDisable)
		{
			listNotorBangerLinker[i]->bDisable = false;
			listNotorBangerLinker[i]->object = object;
			listNotorBangerLinker[i]->entity = entity;
		}
	}

	NotorBangerLinker* linker = new NotorBangerLinker();
	linker->object = object;
	linker->entity = entity;
	listNotorBangerLinker.push_back(linker);
}

void Quadtree::HeadHunterLoader(Entity * entity)
{
	HeadHunter* object = HeadHunter::CreateHeadHunter(D3DXVECTOR2(entity->boundBox->x, entity->boundBox->y), entity->direction);
	entity->bCreated = true;

	//cout << entity->boundBox->x << "\t" << entity->boundBox->y << "\t" << entity->boundBox->width << "\t" << entity->boundBox->height << "\n";

	for (int i = 0; i < listHeadHunterLinker.size(); i++)
	{
		if (listHeadHunterLinker[i]->bDisable)
		{
			listHeadHunterLinker[i]->bDisable = false;
			listHeadHunterLinker[i]->object = object;
			listHeadHunterLinker[i]->entity = entity;
		}
	}

	HeadHunterLinker* linker = new HeadHunterLinker();
	linker->object = object;
	linker->entity = entity;
	listHeadHunterLinker.push_back(linker);
}

void Quadtree::HelitLoader(Entity * entity)
{
	Helit* object = Helit::CreateHelit(D3DXVECTOR2(entity->boundBox->x, entity->boundBox->y), entity->direction);
	entity->bCreated = true;

	//cout << entity->boundBox->x << "\t" << entity->boundBox->y << "\t" << entity->boundBox->width << "\t" << entity->boundBox->height << "\n";

	for (int i = 0; i < listHelitLinker.size(); i++)
	{
		if (listHelitLinker[i]->bDisable)
		{
			listHelitLinker[i]->bDisable = false;
			listHelitLinker[i]->object = object;
			listHelitLinker[i]->entity = entity;
		}
	}

	HelitLinker* linker = new HelitLinker();
	linker->object = object;
	linker->entity = entity;
	listHelitLinker.push_back(linker);
}

void Quadtree::Clear()
{
	//Clear all nodes
	if (m_nodes)
	{
		for (int i = 0; i < 4; i++)
		{
			m_nodes[i]->Clear();
			delete m_nodes[i];
		}
		delete[] m_nodes;
	}

	//Clear current quadtree
	m_objects_list->clear();

	delete m_objects_list;
	delete m_region;
}

bool Quadtree::IsContain(Entity* entity)
{
	Rect* bound = entity->GetBoundingBox();

	return !(bound->x + bound->width < m_region->x ||
		bound->y + bound->height < m_region->y ||
		bound->x > m_region->x + m_region->width ||
		bound->y > m_region->y + m_region->height);
}

bool Quadtree::IsContain(Rect rect)
{
	return !(rect.x + rect.width < m_region->x ||
		rect.y + rect.height < m_region->y ||
		rect.x > m_region->x + m_region->width ||
		rect.y > m_region->y + m_region->height);
}

bool Quadtree::IsContain(Rect rect, Rect rect2)
{
	return !(rect.x + rect.width < rect2.x ||
		rect.y + rect.height < rect2.y ||
		rect.x > rect2.x + rect2.width ||
		rect.y > rect2.y + rect2.height);
}

bool Quadtree::HoanToanNamTrongNodeKeTiep(Entity * entity)
{
	Rect* bound = entity->GetBoundingBox();

	Rect* rect1 = new Rect(m_region->x, m_region->y, m_region->width / 2, m_region->height / 2);
	Rect* rect2 = new Rect(m_region->x + m_region->width / 2, m_region->y, m_region->width / 2, m_region->height / 2);
	Rect* rect3 = new Rect(m_region->x, m_region->y + m_region->height / 2, m_region->width / 2, m_region->height / 2);
	Rect* rect4 = new Rect(m_region->x + m_region->width / 2,m_region->y + m_region->height / 2, m_region->width / 2, m_region->height / 2);

	if (bound->x >= rect1->x && bound->x + bound->width <= rect1->x + rect1->width &&
		bound->y >= rect1->y && bound->y + bound->height <= rect1->y + rect1->height)
		return true;
	if (bound->x >= rect2->x && bound->x + bound->width <= rect2->x + rect2->width &&
		bound->y >= rect2->y && bound->y + bound->height <= rect2->y + rect2->height)
		return true;
	if (bound->x >= rect3->x && bound->x + bound->width <= rect3->x + rect3->width &&
		bound->y >= rect3->y && bound->y + bound->height <= rect3->y + rect3->height)
		return true;
	if (bound->x >= rect4->x && bound->x + bound->width <= rect4->x + rect4->width &&
		bound->y >= rect4->y && bound->y + bound->height <= rect4->y + rect4->height)
		return true;


	return false;
}

bool Quadtree::HoanToanNamTrong(Entity * entity)
{
	Rect* bound = entity->GetBoundingBox();

	if (bound->x >= m_region->x && bound->x + bound->width <= m_region->x + m_region->width &&
		bound->y >= m_region->y && bound->y + bound->height <= m_region->y + m_region->height)
		return true;

	return false;
}
