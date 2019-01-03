#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include "GameObject\MapCollision.h"
#include "GameObject\NotorBanger.h"
#include "GameObject\HeadHunter.h"
#include "GameObject\Helit.h"
#include "GameObject\Camera.h"

using namespace std;

#define MAX_OBJECT_IN_REGION 5
#define MAX_LEVEL 10

#define _CRT_SECURE_NO_WARNINGS
#define PRINT_DEBUG FALSE
#define DONT_LOAD_NOTORBANGER FALSE
#define DONT_LOAD_HEADGUNNER FALSE
class Quadtree;
struct Rect
{
	float x;
	float y;
	float width;
	float height;

	Rect(float ix, float iy, float iwidth, float iheight)
	{
		x = ix;
		y = iy;
		width = iwidth;
		height = iheight;
	}
	Rect(){}
};

struct Entity
{
	int id;
	bool bCreated;
	Rect* boundBox;
	Quadtree* node;

	Rect* GetBoundingBox()
	{
		return boundBox;
	}
	Entity() 
	{
		boundBox = new Rect();
		bCreated = false;
	}
	Entity(int iid, Rect* irect)
	{
		id = iid;
		boundBox = irect;
		bCreated = false;
	}
};

struct MapCollisionLinker
{
	MapCollision* object;
	Entity* entity;
	bool bDisable;

	MapCollisionLinker() { bDisable = false; }

	void Disable()
	{
		bDisable = true;
		object->Disable();
		entity->bCreated = false;
	}
};
struct NotorBangerLinker
{
	NotorBanger* object;
	Entity* entity;
	bool bDisable;

	NotorBangerLinker() { bDisable = false; }

	void Disable()
	{
		bDisable = true;
		object->Disable();
		entity->bCreated = false;
	}
};
struct HeadHunterLinker
{
	HeadHunter* object;
	Entity* entity;
	bool bDisable;

	HeadHunterLinker() { bDisable = false; }

	void Disable()
	{
		bDisable = true;
		object->Disable();
		entity->bCreated = false;
	}
};
struct HelitLinker
{
	Helit* object;
	Entity* entity;
	bool bDisable;

	HelitLinker() { bDisable = false; }

	void Disable()
	{
		bDisable = true;
		object->Disable();
		entity->bCreated = false;
	}
};

struct QuadTreeInformation
{
	string id;
	int size;
	std::vector<Entity*>  m_objects_list;
};


//Cac Buoc them 1 Object moi vao QuadTree
// Tao Struct *Object*Linker cho Object do
// Tao 1 list cua Object do
// Viet Function *Object*Loader(Entity* entity) cho object do
// Vao Function ObjectLoader() them case vao
// Xu ly phan con lai trong DynamicunLoader()

class Quadtree
{
private:
	static int count;
	int            m_level;
	Rect*          m_region;
	Quadtree**     m_nodes;
	std::vector<Entity*>*          m_objects_list;

	bool           IsContain(Entity* entity);
	bool           IsContain(Rect rect);
	bool           IsContain(Rect rect, Rect rect2);
	bool		   HoanToanNamTrongNodeKeTiep(Entity* entity);
	bool		   HoanToanNamTrong(Entity* entity);
	void           Split();
	void		   ObjectLoader(Entity* entity);

	fstream f;

	//ListObject
	static std::vector<MapCollisionLinker*> listMapCollisionLinker;
	static std::vector<NotorBangerLinker*> listNotorBangerLinker;
	static std::vector<HeadHunterLinker*> listHeadHunterLinker;
	static std::vector<HelitLinker*> listHelitLinker;

public:

	string id;
	static Camera* camera;
	static Rect cameraRect;

	Quadtree() {};
	Quadtree(int level, Rect* region, string iid = "0000000000");
	~Quadtree() {};

	void           Clear();
	void           Insert(Entity* entity);

	void		   PrintObjectWhere();
	void		   LoadTree()
	{
		f.open("Resources/quadtree.txt", ios::in);
		string data = "0123456789";
		string line;
		int i;
		int nodeID;
		while (!f.eof())
		{
			getline(f, line);

			for (i = 0; i < 10; i++)
			{
				data[i] = line[i];
			}

			//cout << data << "\t";

			char te[10];

			for (i = 0; i < 10; i++)
			{
				te[i] = data[i];
			}

			nodeID = atoi(te);

			char temp[] = "000";

			temp[0] = line[11];
			temp[1] = line[12];
			temp[2] = line[13];

			int count = atoi(temp);

			//cout << count << "\t";

			int dem = 15;

			if (count == 0)
				int a = 0;
				//cout << endl;
			else
			{
				for (int i = 0; i < count; i++)
				{

					int ID = GetInt(line, dem);
					dem++;
					//cout << ID << "\t";

					int x = GetInt(line, dem);
					dem++;
					//cout << x << "\t";

					int y = GetInt(line, dem);
					dem++;
					//cout << y << "\t";

					int w = GetInt(line, dem);
					dem++;
					//cout << w << "\t";

					int h = GetInt(line, dem);
					dem++;
					//cout << h << "\t";

					Entity* entity = new Entity(ID, new Rect(x, y, w, h));

					PhanTichNodeID(data, entity);
				}
				cout << endl;
			}



		}

		f.close();
	}

	int GetInt(string line, int &i)
	{
		if (line[i] == '0')
		{
			++i;
			return 0;
		}
		char data[] = "0000";
		for (int count = 0; count < 4; count++)
		{
			char temp = line[i];
			if (line[i] == ' ' || line[i] == '/n' || line[i] == '\0')
				break;
			for (int t = 0; t < 3; t++)
			{
				data[t] = data[t + 1];
			}
			data[3] = line[i];
			i++;
		}
		return atoi(data);
	}

	void PhanTichNodeID(string ID, Entity* entity);

	int CompareID(string nodeID, string inputID);

	void TestObjectCreate();

	float loadPeriod;
	float loadPeriodCount;
	void DynamicLoad(float deltatime);
	void DynamicunLoad(float deltatime);

	//Function *Object*Loader()
	void MapCollisionLoader(Entity* entity);
	void NotorBangerLoader(Entity* entity);
	void HeadHunterLoader(Entity* entity);
	void HelitLoader(Entity* entity);
};

