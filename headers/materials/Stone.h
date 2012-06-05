#ifndef STONE_H_INCLUDED
#define STONE_H_INCLUDED

#include "Material.h"
#include "../geometry/Ray.h"
#include "../geometry/Scene.h"
#include "Worley.h"
#include "../geometry/Vector3.h"
#include "Perlin.h"
#include "Lambert.h"

class Stone : public Lambert
{
public:
    Stone();
    virtual ~Stone();
    virtual void preCalc() {};
    
    virtual Vector3 diffuseColor(const HitInfo& hit) const;
    static const int WORLEY_ORDER = 4;

};

#endif