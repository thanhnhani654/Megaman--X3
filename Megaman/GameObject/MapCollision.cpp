#include "MapCollision.h"

std::vector<MapCollision*> MapCollision::listMapCollision; 

void MapCollision::Initialize()
{
	box.StaticInitialize(this, 800, 200);
	listMapCollision.push_back(this);
	name = "null";
	GetTagMethod()->AddTag(eTag::GroundTag);
}

void MapCollision::Initialize(int id,float x, float y, int w, int h)
{
	this->id = id;
	SetPosition(x , y);
	box.StaticInitialize(this, w, h);
	listMapCollision.push_back(this);
}

void MapCollision::Initialize2(int type, float x, float y, int w, int h)
{
	switch (type)
	{
	case 0:
		this->name = "wall";
		break;
	case 1:
		this->name = "ground";
		break;
	case 2:
		this->name = "leftScroller";
		break;
	case 3:
		this->name = "rightScroller";
		break;
	}
	GetTagMethod()->AddTag(eTag::GroundTag);
	SetPosition(x, y);
	box.StaticInitialize(this, w, h);
	bDisable = false;
	listMapCollision.push_back(this);
}

void MapCollision::ReInitialize(int type, float x, float y, int w, int h)
{
	switch (type)
	{
	case 0:
		this->name = "wall";
		break;
	case 1:
		this->name = "ground";
		break;
	case 2:
		this->name = "leftScroller";
		break;
	case 3:
		this->name = "rightScroller";
		break;
	}
	SetPosition(x, y);
	box.SetSize(w, h);
	box.SetPosition();
	Enable();
}

MapCollision* MapCollision::CreateMapCollision(int type, float x, float y, int w, int h)
{
	//Debug
	//int demSoObjectDisable = 0;
	//for (int i = 0; i < listMapCollision.size(); i++)
	//{
	//	if (listMapCollision[i]->IsDisable())
	//		//demSoObjectDisable++;
	//}
	//cout << "Amount Of MapCollsion: " << listMapCollision.size() - demSoObjectDisable << "\t\t Call From CreateMapCollsion" << endl;


	for (int i = 0; i < listMapCollision.size(); i++)
	{
		if (listMapCollision[i]->IsDisable())
		{
			listMapCollision[i]->ReInitialize(type, x, y, w, h);
			return listMapCollision[i];
		}
	}
	MapCollision* mapCollision = new MapCollision();
	mapCollision->Initialize2(type, x, y, w, h);

	

	return mapCollision;
}

void MapCollision::Disable()
{
	bDisable = true;
	box.Disable();
}

void MapCollision::Enable()
{
	bDisable = false;
	box.Enable();
}

bool MapCollision::IsDisable()
{
	return bDisable;
}

void MapCollision::LoadMapCollision()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(TEXT("Resources/Map_Collision.xml"));

	if (!result)
	{
		MessageBox(NULL, TEXT("MapCollision not found!"), TEXT("Load Map Collision Error"), NULL);
	}

	int i = 0;

	for (pugi::xml_node tool = doc.first_child(); tool; tool = tool.next_sibling())
	{
		for (pugi::xml_node tool2 = tool.first_child(); tool2; tool2 = tool2.next_sibling())
		{

			tool2;
		}
	}
	for (pugi::xml_node tool = doc.child("map").child("objectgroup").first_child(); tool; tool = tool.next_sibling())
	{
		MapCollision* mapCollision = new MapCollision();
		mapCollision->name = doc.child("map").child("objectgroup").attribute("name").value();
		int id = atoi(tool.attribute("id").value());
		float x = atof(tool.attribute("x").value());
		float y = atof(tool.attribute("y").value());
		int w = atoi(tool.attribute("width").value());
		int h = atoi(tool.attribute("height").value());
		mapCollision->Initialize(id,x, y, w, h);
	}
	for (pugi::xml_node tool = doc.child("map").child("objectgroup1").first_child(); tool; tool = tool.next_sibling())
	{
		MapCollision* mapCollision = new MapCollision();
		mapCollision->name = doc.child("map").child("objectgroup1").attribute("name").value();
		int id = atoi(tool.attribute("id").value());
		float x = atof(tool.attribute("x").value());
		float y = atof(tool.attribute("y").value());
		int w = atoi(tool.attribute("width").value());
		int h = atoi(tool.attribute("height").value());
		mapCollision->Initialize(id, x, y, w, h);
	}

}

void MapCollision::PrintList()
{
	cout << "========================================================================================================================" << endl;
	cout << "Name\t\tID\t\tX\t\tY\t\tWidth\t\tHeight" << endl << endl;
	for (std::vector<MapCollision*>::iterator it = listMapCollision.begin(); it != listMapCollision.end(); it++)
	{
		std::cout<< (*it)->name<< "\t\t" << (*it)->id << "\t\t" << (*it)->GetPosition().x << "\t\t" << (*it)->GetPosition().y << "\t\t" << (*it)->box.GetSize().x << "\t\t" << (*it)->box.GetSize().y << endl;
	}
	cout << "========================================================================================================================" << endl;
}
