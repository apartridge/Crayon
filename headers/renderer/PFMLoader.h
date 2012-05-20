#ifndef CRAYON_PFM_LOADER_H_INCLUDED
#define CRAYON_PFM_LOADER_H_INCLUDED

#include "../../headers/geometry/Vector3.h"

Vector3* readPFMImage(const char * filename, int * width, int * height);

#endif // CRAYON_PFM_LOADER_H_INCLUDED
