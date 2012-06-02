#pragma once
#include "sysutils/PFMLoader.h"

static int min(int a, int b)
{
	if(a < b) return int(a); else return int(b);
}

class TextureImage
{
private:
	Vector3* image;
	int width;
	int height;

public:
	static TextureImage* loadFromPFM(const char* filename);
	Vector3 sampleImage(float u, float v);
};
