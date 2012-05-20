#ifndef CRAYON_MEDIUM_H_INCLUDED
#define CRAYON_MEDIUM_H_INCLUDED

class Medium
{
public:
	//static const Medium AIR = Medium(1.0f);
	Medium()
	{
		indexOfRefraction = 1;
	}
	Medium(float f) {
		indexOfRefraction = f;
	}
	float indexOfRefraction;
};

#endif