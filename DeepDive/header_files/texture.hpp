#pragma once
#include "../lib.h"

unsigned int loadTexture(char const* path, bool flip);

unsigned int loadCubeMap(vector<string> faces);