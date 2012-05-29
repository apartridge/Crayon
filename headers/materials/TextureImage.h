#pragma once
#include "sysutils/PFMLoader.h"

class TextureImage
{
private:
	Vector3* image;
	int width;
	int height;

public:
	static TextureImage* loadFromPFM(const char* filename)
	{
		TextureImage* a = new TextureImage;

		a->image = readPFMImage(filename, &a->width, &a->height);



		return a;

	}
};
