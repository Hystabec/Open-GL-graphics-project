#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "Vertex.h"
#include <map>

using namespace std;

class OBJLoader
{
public:
	static vector<Vertex> LoadOBJ(const string& FolderLoc,
		const string& FileName, string& AmbiantLoc, string& DiffLoc,
		string& specLoc, string& NormalLoc, vector<unsigned int>& indicies);

	static void LoadMaterial(const string& MatLibLoc, string& AmbiantLoc,
		string& DiffLoc, string& specLoc, string& NormalLoc);
};