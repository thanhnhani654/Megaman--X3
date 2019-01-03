#include "TagMethod.h"

void Tag::AddTag(eTag tag)
{
	tags.push_back(tag);
}

bool Tag::isHasTag(eTag tag)
{
	for (int i = 0; i < tags.size(); i++)
	{
		if (tags[i] == tag)
			return true;
	}
	return false;
}

void Tag::EraseTag(eTag tag)
{
	bool isHasTag = false;
	vector<eTag>::iterator it;
	for (it = tags.begin(); it != tags.end(); it++)
	{
		if ((*it) == tag)
		{
			isHasTag = true;
			break;
		}
	}
	if (isHasTag)
	{
		tags.erase(it);
		return;
	}
	cout << "No has tag: " << tag << endl;
}