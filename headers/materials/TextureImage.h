#pragma once
#include "sysutils/PFMLoader.h"

static float maxf(float a, float b)
{
	if(a > b) return a; else return b;
}

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

	Vector3 sampleImage(float u, float v)
	{
		float ua = u - floor(u);
		float va = v - floor(v);

		float x = ua*width - 1;
		int xOne = floor(x);
		int xTwo = maxf(floor(x) + 1, width-1);
		float xOneFactor = 1 - (x - xOne);

		float y = va*height - 1;
		int yOne = floor(y);
		int yTwo = maxf(floor(y) + 1, height-1);
		float yOneFactor = 1 - (y - yOne);


		return image[xOne*width + yOne];

		return image[xOne*width + yOne]*(xOneFactor)*(yOneFactor) // x1y1
				+ image[xOne*width + yTwo]*(xOneFactor)*(1-yOneFactor) // x1y2
				+ image[xTwo*width + yOne]*(1-xOneFactor)*(yOneFactor) // x2y1
				+ image[xTwo*width + yTwo]*(1-xOneFactor)*(1-yOneFactor); // x2y2

	}
};
