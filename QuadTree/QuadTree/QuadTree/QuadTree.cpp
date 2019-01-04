#include "Quadtree.h"

int Quadtree::count = 0;

Quadtree::Quadtree(int level, Rect* region, string iid)
{
	m_level = level;
	m_region = region;
	m_nodes = nullptr;
	m_objects_list = new std::vector<Entity*>();
	id = iid;
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

void Quadtree::Insert(Entity* entity)
{
	//Insert entity into corresponding nodes
	if (entity->boundBox->x == 7685 && id == "2420000000")
		int a = 0;
	if (entity->boundBox->x == 7685 && id == "2422000000")
		int a = 0;

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
		if (entity->inserted)
			int a = 0;
		entity->inserted = true;
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
			std::cout << temp << " " << i<<endl;
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
			if (this->m_objects_list->at(m_objects_list->size() - 1 - i)->boundBox->x == 7685)
				int a = 0;
			m_objects_list->at(m_objects_list->size() - 1 - i)->inserted = false;
			//if (i > 0 && m_objects_list->size() > i)
			//	std::swap_ranges(m_objects_list->end()-(i-1), m_objects_list->end(), m_objects_list->end() - i-1);
			//m_objects_list->pop_back();
			m_objects_list = deleteElement(m_objects_list->at(m_objects_list->size() - 1 - i));
			m_objects_list->shrink_to_fit();
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

	f.open("quadtree.txt", ios::app);
	string data;
	data += id;

	data += ' ';

	char* temp = new char(256);
	string temp2 = "000";

	if (m_objects_list->size() != 0)
	{
		int size = m_objects_list->size();

		if (size < 10)
		{
			itoa(size,temp,10);
			temp2[2] = temp[0];
		}
		else if (size >= 10 && size < 100)
		{
			int size2 = size / 10;
			int size3 = size - size2 * 10;
			itoa(size2, temp, 10);
			temp2[1] = temp[0];
			itoa(size3, temp, 10);
			temp2[2] = temp[0];
		}
	}

	data += temp2;

	for (int i = 0; i < m_objects_list->size(); i++)
	{
		data += ' ';
		temp = itoa(m_objects_list->at(i)->id, temp, 10);
		temp2 = string(temp);
		data += temp2;

		data += ' ';
		temp = itoa(m_objects_list->at(i)->boundBox->x, temp, 10);
		temp2 = string(temp);
		data += temp2;

		data += ' ';
		temp = itoa(m_objects_list->at(i)->boundBox->y, temp, 10);
		temp2 = string(temp);
		data += temp2;

		data += ' ';
		temp = itoa(m_objects_list->at(i)->boundBox->width, temp, 10);
		temp2 = string(temp);
		data += temp2;

		data += ' ';
		temp = itoa(m_objects_list->at(i)->boundBox->height, temp, 10);
		temp2 = string(temp);
		data += temp2;
	}
	data += '\n';
	f << data;

	f.close();

	if (m_nodes)
	{
			m_nodes[0]->PrintObjectWhere();
			m_nodes[1]->PrintObjectWhere();
			m_nodes[2]->PrintObjectWhere();
			m_nodes[3]->PrintObjectWhere();

		return; // Return here to ignore rest.
	}

	
}

void Quadtree::PrintObject()
{
	if (!m_objects_list->empty())
		for (int i = 0; i < m_objects_list->size(); i++)
		{
			std::cout << "ID: " << m_objects_list->at(i)->id << "\tX: " << m_objects_list->at(i)->boundBox->x << "\tY: " << m_objects_list->at(i)->boundBox->y << "\tWidth: " << m_objects_list->at(i)->boundBox->width << "\tHeight: " << m_objects_list->at(i)->boundBox->height << std::endl;
			count++;
		}
	std::cout << std::endl;

	if (m_nodes)
	{
		m_nodes[0]->PrintObject();
		m_nodes[1]->PrintObject();
		m_nodes[2]->PrintObject();
		m_nodes[3]->PrintObject();

		return; // Return here to ignore rest.
	}
}

std::vector<Entity*>* Quadtree::deleteElement(Entity * entity)
{
	std::vector<Entity*>* tempList = new std::vector<Entity*>();
	for (int i = 0; i < m_objects_list->size(); i++)
	{
		if (m_objects_list->at(i) != entity)
			tempList->push_back(m_objects_list->at(i));
	}

	return tempList;
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
