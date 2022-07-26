#ifndef RESOUCEMANAGER_H
#define RESOUCEMANAGER_H

#include <unordered_map>


class ResourceManager
{
public :
	enum EModel
	{
		triangle,
		rectangle,
		cube
	};

private :
	std::unordered_map<EModel, float*> modelsMap;
};

#endif

