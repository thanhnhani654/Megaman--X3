#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

#define MAX_OBJECT_IN_REGION 5
#define MAX_LEVEL 10
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
	bool inserted;
	Rect* boundBox;

	Rect* GetBoundingBox()
	{
		return boundBox;
	}
	Entity() 
	{
		boundBox = new Rect();
	}
	Entity(int iid, Rect* irect)
	{
		id = iid;
		boundBox = irect;
	}
};

struct QuadTreeInformation
{
	string id;
	int size;
	std::vector<Entity*>  m_objects_list;
};

class Quadtree
{
private:
	static int count;
	int            m_level;
	Rect*          m_region;
	Quadtree**     m_nodes;
	std::vector<Entity*>*          m_objects_list;

	bool           IsContain(Entity* entity);
	bool		   HoanToanNamTrongNodeKeTiep(Entity* entity);
	bool		   HoanToanNamTrong(Entity* entity);
	void           Split();

	fstream f;
public:
	string id;

	Quadtree() {};
	Quadtree(int level, Rect* region, string iid = "0000000000");
	~Quadtree() {};

	void           Clear();
	void           Insert(Entity* entity);

	void		   PrintObjectWhere();
	void		   PrintObject();
	std::vector<Entity*>* deleteElement(Entity* entity);
};

