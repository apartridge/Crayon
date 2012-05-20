#ifndef CRAYON_PERLIN_H_INCLUDED
#define CRAYON_PERLIN_H_INCLUDED

#include <math.h>

//! Perlin noise class
/*!
    Based on Java reference implementation of:
        Improved Noise - Copyright 2002 Ken Perlin.

    (http://mrl.nyu.edu/~perlin/noise/)
*/
class PerlinNoise
{
public:
   static float noise(float x, float y, float z)
   {
      int X = int(floor(x)) & 255,
          Y = int(floor(y)) & 255,
          Z = int(floor(z)) & 255;

      x -= floor(x);
      y -= floor(y);
      z -= floor(z);
      float u = fade(x),
            v = fade(y),
            w = fade(z);

      int A = p[X  ]+Y, AA = p[A]+Z, AB = p[A+1]+Z,
          B = p[X+1]+Y, BA = p[B]+Z, BB = p[B+1]+Z;

      return lerp(w, lerp(v, lerp(u, grad(p[AA  ], x  , y  , z   ),
                                     grad(p[BA  ], x-1, y  , z   )),
                             lerp(u, grad(p[AB  ], x  , y-1, z   ),
                                     grad(p[BB  ], x-1, y-1, z   ))),
                     lerp(v, lerp(u, grad(p[AA+1], x  , y  , z-1 ),
                                     grad(p[BA+1], x-1, y  , z-1 )),
                             lerp(u, grad(p[AB+1], x  , y-1, z-1 ),
                                     grad(p[BB+1], x-1, y-1, z-1 ))));
   }

   /* Added turbulence */
   static float turbulence(int fmax, float x, float y, float z)
   {
       float sum = 0;
       for (float f = 1; f < (2 << fmax); f *= 2)
       {
           sum += noise(x*f, y*f, z*f) / f;
       }
       return sum;
   }

private:
   static float fade(float t) {return t * t * t * (t * (t * 6 - 15) + 10);}
   static float lerp(float t, float a, float b) {return a + t * (b - a);}
   static float grad(int hash, float x, float y, float z)
   {
      int h = hash & 15;
      float u = h<8 ? x : y,
            v = h<4 ? y : h==12||h==14 ? x : z;
      return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
   };

   static const unsigned char p[512];
};

#endif // CRAYON_PERLIN_H_INCLUDED
