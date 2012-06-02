#include "materials/TextureImage.h"

TextureImage* TextureImage::loadFromPFM(const char* filename)
{
	TextureImage* a = new TextureImage;

	a->image = readPFMImage(filename, &a->width, &a->height);

	return a;
}

Vector3 TextureImage::sampleImage(float u, float v)
{
	float ua = u - floor(u);
	float va = v - floor(v);

	float x = ua*width;
	int xOne = ceil(x) - 1;

	int xTwo = min(xOne + 1, width-1);
	float xOneFactor = 1 - (x - xOne);

	float y = va*height;
	int yOne = ceil(y) - 1;
	int yTwo = min(yOne + 1, height-1);
	float yOneFactor = 1 - (y - yOne);

	return image[xOne*height + yOne];

	/*return image[xOne*height + yOne]*(xOneFactor)*(yOneFactor) // x1y1
			+ image[xOne*height + yTwo]*(xOneFactor)*(1-yOneFactor) // x1y2
			+ image[xTwo*height + yOne]*(1-xOneFactor)*(yOneFactor) // x2y1
			+ image[xTwo*height + yTwo]*(1-xOneFactor)*(1-yOneFactor); // x2y2*/

}
