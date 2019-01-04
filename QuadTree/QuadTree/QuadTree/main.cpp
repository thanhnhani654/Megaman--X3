#include <iostream>
#include <fstream>
#include <string>
#include "Quadtree.h"
#include <vector>

using namespace std;

Entity* LoadData(string line);
void PrintListObject(vector<Entity*> list);
void InsertList(Quadtree* tree, vector<Entity*> list);

void main()
{
	string data;
	vector<Entity*> listObject;

	fstream f;
	f.open("Map.txt", ios::in);

	string line;

	while (!f.eof())
	{
		getline(f, line);
		data += line;
		data += "\n";
		listObject.push_back(LoadData(line));
	}
	PrintListObject(listObject);
	//cout << data;

	f.close();


	//
	Quadtree* tree = new Quadtree(1, new Rect(0, 0, 8000, 8000));

	InsertList(tree, listObject);

	cout << listObject.size() << endl;

	cout << endl << endl;

	tree->PrintObjectWhere();
	//tree->PrintObject();
	system("pause");
	return;
}

Entity* LoadData(string line)
{
	//Entity* e = new Entity();
	int i = 0;
	string data;

	//////////////////////// ID
	while (line[i] != ' ')
	{
		data += line[i];
		i++;
	}
	i++;

	int id = atoi(data.c_str());
	data = "";

	//////////////////////// X
	while (line[i] != ' ')
	{
		data += line[i];
		i++;
	}
	i++;

	int x = atoi(data.c_str());
	data = "";

	//////////////////////// Y
	while (line[i] != ' ')
	{
		data += line[i];
		i++;
	}
	i++;

	int y = atoi(data.c_str());
	data = "";

	//////////////////////// W
	while (line[i] != ' ')
	{
		data += line[i];
		i++;
	}
	i++;

	int w = atoi(data.c_str());
	data = "";

	//////////////////////// H
	while (i <= line.length())
	{
		data += line[i];
		i++;
	}
	i++;

	int h = atoi(data.c_str());
	data = "";

	Entity* e = new Entity(id, new Rect(x, y, w, h));

	return e;
}

void PrintListObject(vector<Entity*> list)
{
	for (int i = 0; i < list.size(); i++)
	{
		cout << list.at(i)->id << "\t" << list.at(i)->boundBox->x << "\t" << list.at(i)->boundBox->y << "\t" << list.at(i)->boundBox->width << "\t" << list.at(i)->boundBox->height << "\t" << endl;
	}
}

void InsertList(Quadtree* tree, vector<Entity*> list)
{
	for (int i = 0; i < list.size(); i++)
	{
		tree->Insert(list.at(i));
	}
}