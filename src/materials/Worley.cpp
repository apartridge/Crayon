#include "../../headers/materials/Worley.h"
#include <math.h>
 

// private helper functions
namespace
{

// A hardwired lookup table to quickly determine how many feature
// points should be in each spatial cube. We use a table so we don't
// need to make multiple slower tests.  A random number indexed into
// this array will give an approximate Poisson distribution of mean
// density 2.5. Read the book for the longwinded explanation.
    static int poissonCount[256] =
    {
        4,3,1,1,1,2,4,2,2,2,5,1,0,2,1,2,2,0,4,3,2,1,2,1,3,2,2,4,2,2,5,1,2,3,2,2,2,2,2,3,
        2,4,2,5,3,2,2,2,5,3,3,5,2,1,3,3,4,4,2,3,0,4,2,2,2,1,3,2,2,2,3,3,3,1,2,0,2,1,1,2,
        2,2,2,5,3,2,3,2,3,2,2,1,0,2,1,1,2,1,2,2,1,3,4,2,2,2,5,4,2,4,2,2,5,4,3,2,2,5,4,3,
        3,3,5,2,2,2,2,2,3,1,1,4,2,1,3,3,4,3,2,4,3,3,3,4,5,1,4,2,4,3,1,2,3,5,3,2,1,3,1,3,
        3,3,2,3,1,5,5,4,2,2,4,1,3,4,1,5,3,3,5,3,4,3,2,2,1,1,1,1,1,2,4,5,4,5,4,2,1,5,1,1,
        2,3,3,3,2,5,2,3,3,2,0,2,1,1,4,2,1,3,2,1,2,2,3,2,5,5,3,4,5,5,2,4,4,5,3,2,2,2,1,4,
        2,3,3,4,2,5,4,2,4,2,2,2,4,5,3,2
    };

// This constant is manipulated to make sure that the mean value of F[0]
// is 1.0. This makes an easy natural "scale" size of the cellular features.
#define DENSITY_ADJUSTMENT  0.398150

// the function to merge-sort a "cube" of samples into the current best-found
// list of values.
    void addSamples(long xi, long maxOrder,
                    float at, float *F, float(*delta), unsigned long *ID);
    void addSamples(long xi, long yi, long maxOrder,
                    float at[2], float *F, float(*delta)[2], unsigned long *ID);
    void addSamples(long xi, long yi, long zi, long maxOrder,
                    float at[3], float *F, float(*delta)[3], unsigned long *ID);
} // namespace


void
WorleyNoise::noise1D(float at, long maxOrder,
                     float *F, float(*delta), unsigned long *ID)
{
    float x2, mx2;
    float newAt;
    long intAt, i;

    // Initialize the F values to "huge" so they will be replaced by the
    // first real sample tests. Note we'll be storing and comparing the
    // SQUARED distance from the feature points to avoid lots of slow
    // sqrt() calls. We'll use sqrt() only on the final answer.
    for (i = 0; i < maxOrder; i++) F[i] = 999999.9;

    // Make our own local copy, multiplying to make mean(F[0])==1.0
    newAt = DENSITY_ADJUSTMENT * at;

    // Find the integer cube holding the hit point
    intAt = int(floor(newAt));

    // A simple way to compute the closest neighbors would be to test all
    // boundary cubes exhaustively. This is simple with code like:
    // {
    //     long ii, jj, kk;
    //     for (ii=-1; ii<=1; ii++)
    //     addSamples(intAt+ii, maxOrder, newAt, F, delta, ID);
    // }
    // But this wastes a lot of time working on cubes which are known to be
    // too far away to matter! So we can use a more complex testing method
    // that avoids this needless testing of distant cubes. This doubles the
    // speed of the algorithm.

    // Test the central cube for closest point(s).
    addSamples(intAt, maxOrder, newAt, F, delta, ID);

    // We test if neighbor cubes are even POSSIBLE contributors by examining the
    // combinations of the sum of the squared distances from the cube's lower
    // or upper corners.
    x2 = newAt - intAt;
    mx2 = (1.0f - x2) * (1.0f - x2);
    x2 *= x2;

    // Test 2 facing neighbors of center cube.
    if (x2 < F[maxOrder-1])  addSamples(intAt - 1, maxOrder, newAt, F, delta, ID);
    if (mx2 < F[maxOrder-1]) addSamples(intAt + 1, maxOrder, newAt, F, delta, ID);

    // We're done! Convert everything to right size scale
    for (i = 0; i < maxOrder; i++)
    {
        F[i] = sqrt(F[i]) * (1.0 / DENSITY_ADJUSTMENT);
        delta[i] *= (1.0 / DENSITY_ADJUSTMENT);
    }
}


void
WorleyNoise::noise2D(float at[2], long maxOrder,
                     float *F, float(*delta)[2], unsigned long *ID)
{
    float x2, y2, mx2, my2;
    float newAt[2];
    long intAt[2], i;

    // Initialize the F values to "huge" so they will be replaced by the
    // first real sample tests. Note we'll be storing and comparing the
    // SQUARED distance from the feature points to avoid lots of slow
    // sqrt() calls. We'll use sqrt() only on the final answer.
    for (i = 0; i < maxOrder; i++) F[i] = 999999.9;

    // Make our own local copy, multiplying to make mean(F[0])==1.0
    newAt[0] = DENSITY_ADJUSTMENT * at[0];
    newAt[1] = DENSITY_ADJUSTMENT * at[1];

    // Find the integer cube holding the hit point
    intAt[0] = int(floor(newAt[0]));
    intAt[1] = int(floor(newAt[1]));

    // A simple way to compute the closest neighbors would be to test all
    // boundary cubes exhaustively. This is simple with code like:
    // {
    //     long ii, jj, kk;
    //     for (ii=-1; ii<=1; ii++) for (jj=-1; jj<=1; jj++)
    //     addSamples(intAt[0]+ii, intAt[1]+jj, maxOrder, newAt, F, delta, ID);
    // }
    // But this wastes a lot of time working on cubes which are known to be
    // too far away to matter! So we can use a more complex testing method
    // that avoids this needless testing of distant cubes. This doubles the
    // speed of the algorithm.

    // Test the central cube for closest point(s).
    addSamples(intAt[0], intAt[1], maxOrder, newAt, F, delta, ID);

    // We test if neighbor cubes are even POSSIBLE contributors by examining the
    // combinations of the sum of the squared distances from the cube's lower
    // or upper corners.
    x2 = newAt[0] - intAt[0];
    y2 = newAt[1] - intAt[1];
    mx2 = (1.0 - x2) * (1.0 - x2);
    my2 = (1.0 - y2) * (1.0 - y2);
    x2 *= x2;
    y2 *= y2;

    // Test 4 facing neighbors of center cube. These are closest and most
    // likely to have a close feature point.
    if (x2 < F[maxOrder-1])  addSamples(intAt[0] - 1, intAt[1]  ,
        maxOrder, newAt, F, delta, ID);
    if (y2 < F[maxOrder-1])  addSamples(intAt[0]  , intAt[1] - 1,
        maxOrder, newAt, F, delta, ID);

    if (mx2 < F[maxOrder-1]) addSamples(intAt[0] + 1, intAt[1]  ,
        maxOrder, newAt, F, delta, ID);
    if (my2 < F[maxOrder-1]) addSamples(intAt[0]  , intAt[1] + 1,
        maxOrder, newAt, F, delta, ID);

    // Test 4 "edge cube" neighbors if necessary. They're next closest.
    if (x2 + y2 < F[maxOrder-1])   addSamples(intAt[0] - 1, intAt[1] - 1,
        maxOrder, newAt, F, delta, ID);
    if (mx2 + my2 < F[maxOrder-1]) addSamples(intAt[0] + 1, intAt[1] + 1,
        maxOrder, newAt, F, delta, ID);
    if (x2 + my2 < F[maxOrder-1])  addSamples(intAt[0] - 1, intAt[1] + 1,
        maxOrder, newAt, F, delta, ID);
    if (mx2 + y2 < F[maxOrder-1])  addSamples(intAt[0] + 1, intAt[1] - 1,
        maxOrder, newAt, F, delta, ID);

    // We're done! Convert everything to right size scale
    for (i = 0; i < maxOrder; i++)
    {
        F[i] = sqrt(F[i]) * (1.0 / DENSITY_ADJUSTMENT);
        delta[i][0] *= (1.0 / DENSITY_ADJUSTMENT);
        delta[i][1] *= (1.0 / DENSITY_ADJUSTMENT);
    }
}


void
WorleyNoise::noise3D(float at[3], long maxOrder,
                     float *F, float(*delta)[3], unsigned long *ID)
{
    float x2, y2, z2, mx2, my2, mz2;
    float newAt[3];
    long intAt[3], i;

    // Initialize the F values to "huge" so they will be replaced by the
    // first real sample tests. Note we'll be storing and comparing the
    // SQUARED distance from the feature points to avoid lots of slow
    // sqrt() calls. We'll use sqrt() only on the final answer.
    for (i = 0; i < maxOrder; i++) F[i] = 999999.9;

    // Make our own local copy, multiplying to make mean(F[0])==1.0
    newAt[0] = DENSITY_ADJUSTMENT * at[0];
    newAt[1] = DENSITY_ADJUSTMENT * at[1];
    newAt[2] = DENSITY_ADJUSTMENT * at[2];

    // Find the integer cube holding the hit point
    intAt[0] = int(floor(newAt[0]));
    intAt[1] = int(floor(newAt[1]));
    intAt[2] = int(floor(newAt[2]));

    // A simple way to compute the closest neighbors would be to test all
    // boundary cubes exhaustively. This is simple with code like:
    // {
    //     long ii, jj, kk;
    //     for (ii=-1; ii<=1; ii++) for (jj=-1; jj<=1; jj++) for (kk=-1; kk<=1; kk++)
    //     addSamples(intAt[0]+ii,intAt[1]+jj,intAt[2]+kk,
    //     maxOrder, newAt, F, delta, ID);
    // }
    // But this wastes a lot of time working on cubes which are known to be
    // too far away to matter! So we can use a more complex testing method
    // that avoids this needless testing of distant cubes. This doubles the
    // speed of the algorithm.

    // Test the central cube for closest point(s).
    addSamples(intAt[0], intAt[1], intAt[2], maxOrder, newAt, F, delta, ID);

    // We test if neighbor cubes are even POSSIBLE contributors by examining the
    // combinations of the sum of the squared distances from the cube's lower
    // or upper corners.
    x2 = newAt[0] - intAt[0];
    y2 = newAt[1] - intAt[1];
    z2 = newAt[2] - intAt[2];
    mx2 = (1.0 - x2) * (1.0 - x2);
    my2 = (1.0 - y2) * (1.0 - y2);
    mz2 = (1.0 - z2) * (1.0 - z2);
    x2 *= x2;
    y2 *= y2;
    z2 *= z2;

    // Test 6 facing neighbors of center cube. These are closest and most
    // likely to have a close feature point.
    if (x2 < F[maxOrder-1])  addSamples(intAt[0] - 1, intAt[1]  , intAt[2]  ,
        maxOrder, newAt, F, delta, ID);
    if (y2 < F[maxOrder-1])  addSamples(intAt[0]  , intAt[1] - 1, intAt[2]  ,
        maxOrder, newAt, F, delta, ID);
    if (z2 < F[maxOrder-1])  addSamples(intAt[0]  , intAt[1]  , intAt[2] - 1,
        maxOrder, newAt, F, delta, ID);

    if (mx2 < F[maxOrder-1]) addSamples(intAt[0] + 1, intAt[1]  , intAt[2]  ,
        maxOrder, newAt, F, delta, ID);
    if (my2 < F[maxOrder-1]) addSamples(intAt[0]  , intAt[1] + 1, intAt[2]  ,
        maxOrder, newAt, F, delta, ID);
    if (mz2 < F[maxOrder-1]) addSamples(intAt[0]  , intAt[1]  , intAt[2] + 1,
        maxOrder, newAt, F, delta, ID);

    // Test 12 "edge cube" neighbors if necessary. They're next closest.
    if (x2 + y2 < F[maxOrder-1]) addSamples(intAt[0] - 1, intAt[1] - 1, intAt[2]  ,
        maxOrder, newAt, F, delta, ID);
    if (x2 + z2 < F[maxOrder-1]) addSamples(intAt[0] - 1, intAt[1]  , intAt[2] - 1,
        maxOrder, newAt, F, delta, ID);
    if (y2 + z2 < F[maxOrder-1]) addSamples(intAt[0]  , intAt[1] - 1, intAt[2] - 1,
        maxOrder, newAt, F, delta, ID);
    if (mx2 + my2 < F[maxOrder-1]) addSamples(intAt[0] + 1, intAt[1] + 1, intAt[2]  ,
        maxOrder, newAt, F, delta, ID);
    if (mx2 + mz2 < F[maxOrder-1]) addSamples(intAt[0] + 1, intAt[1]  , intAt[2] + 1,
        maxOrder, newAt, F, delta, ID);
    if (my2 + mz2 < F[maxOrder-1]) addSamples(intAt[0]  , intAt[1] + 1, intAt[2] + 1,
        maxOrder, newAt, F, delta, ID);
    if (x2 + my2 < F[maxOrder-1]) addSamples(intAt[0] - 1, intAt[1] + 1, intAt[2]  ,
        maxOrder, newAt, F, delta, ID);
    if (x2 + mz2 < F[maxOrder-1]) addSamples(intAt[0] - 1, intAt[1]  , intAt[2] + 1,
        maxOrder, newAt, F, delta, ID);
    if (y2 + mz2 < F[maxOrder-1]) addSamples(intAt[0]  , intAt[1] - 1, intAt[2] + 1,
        maxOrder, newAt, F, delta, ID);
    if (mx2 + y2 < F[maxOrder-1]) addSamples(intAt[0] + 1, intAt[1] - 1, intAt[2]  ,
        maxOrder, newAt, F, delta, ID);
    if (mx2 + z2 < F[maxOrder-1]) addSamples(intAt[0] + 1, intAt[1]  , intAt[2] - 1,
        maxOrder, newAt, F, delta, ID);
    if (my2 + z2 < F[maxOrder-1]) addSamples(intAt[0]  , intAt[1] + 1, intAt[2] - 1,
        maxOrder, newAt, F, delta, ID);

    // Final 8 "corner" cubes
    if (x2 + y2 + z2 < F[maxOrder-1]) addSamples(intAt[0] - 1, intAt[1] - 1, intAt[2] - 1,
        maxOrder, newAt, F, delta, ID);
    if (x2 + y2 + mz2 < F[maxOrder-1]) addSamples(intAt[0] - 1, intAt[1] - 1, intAt[2] + 1,
        maxOrder, newAt, F, delta, ID);
    if (x2 + my2 + z2 < F[maxOrder-1]) addSamples(intAt[0] - 1, intAt[1] + 1, intAt[2] - 1,
        maxOrder, newAt, F, delta, ID);
    if (x2 + my2 + mz2 < F[maxOrder-1]) addSamples(intAt[0] - 1, intAt[1] + 1, intAt[2] + 1,
        maxOrder, newAt, F, delta, ID);
    if (mx2 + y2 + z2 < F[maxOrder-1]) addSamples(intAt[0] + 1, intAt[1] - 1, intAt[2] - 1,
        maxOrder, newAt, F, delta, ID);
    if (mx2 + y2 + mz2 < F[maxOrder-1]) addSamples(intAt[0] + 1, intAt[1] - 1, intAt[2] + 1,
        maxOrder, newAt, F, delta, ID);
    if (mx2 + my2 + z2 < F[maxOrder-1]) addSamples(intAt[0] + 1, intAt[1] + 1, intAt[2] - 1,
        maxOrder, newAt, F, delta, ID);
    if (mx2 + my2 + mz2 < F[maxOrder-1]) addSamples(intAt[0] + 1, intAt[1] + 1, intAt[2] + 1,
        maxOrder, newAt, F, delta, ID);

    // We're done! Convert everything to right size scale
    for (i = 0; i < maxOrder; i++)
    {
        F[i] = sqrt(F[i]) * (1.0 / DENSITY_ADJUSTMENT);
        delta[i][0] *= (1.0 / DENSITY_ADJUSTMENT);
        delta[i][1] *= (1.0 / DENSITY_ADJUSTMENT);
        delta[i][2] *= (1.0 / DENSITY_ADJUSTMENT);
    }
}

void WorleyNoise::turbulence(float F[4], int fmax, float x, float y, float z)
{
	unsigned long *id = new unsigned long; // (!)
    for (int i = 0; i < 4; ++i)
        F[i] = 0;

    for (float f = 1; f < (2 << fmax); f *= 2)
    {
        float tmp[4];
	    float at[3] = { x*f, y*f, z*f };
	    float delta[4][3];
        noise3D(at, 4, tmp, delta, id);

        // Normalize on frequency
        for (int i = 0; i < 4; ++i)
            F[i] += tmp[i] / f;
    }
}

// private helper functions
namespace
{

void
addSamples(long xi, long maxOrder, float at, float *F, float(*delta), unsigned long *ID)
{
    float dx, fx, d2;
    long count, i, j, index;
    unsigned long seed, this_id;

    // Each cube has a random number seed based on the cube's ID number.
    // The seed might be better if it were a nonlinear hash like Perlin uses
    // for noise but we do very well with this faster simple one.
    // Our LCG uses Knuth-approved constants for maximal periods.
    seed = 702395077 * xi;

    // How many feature points are in this cube?
    count = poissonCount[seed>>24]; // 256 element lookup table. Use MSB

    seed = 1402024253 * seed + 586950981; // churn the seed with good Knuth LCG

    for (j = 0; j < count; j++) // test and insert each point into our solution
    {
        this_id = seed;
        seed = 1402024253 * seed + 586950981; // churn

        // compute the 0..1 feature point location's XYZ
        fx = (seed + 0.5) * (1.0 / 4294967296.0);
        seed = 1402024253 * seed + 586950981; // churn

        // delta from feature point to sample location
        dx = xi + fx - at;
        d2 = dx * dx; // Euclidian distance, squared

        if (d2 < F[maxOrder-1]) // Is this point close enough to rememember?
        {
            // Insert the information into the output arrays if it's close enough.
            // We use an insertion sort.  No need for a binary search to find
            // the appropriate index.. usually we're dealing with order 2,3,4 so
            // we can just go through the list. If you were computing order 50
            // (wow!!) you could get a speedup with a binary search in the sorted
            // F[] list.

            index = maxOrder;
            while (index > 0 && d2 < F[index-1]) index--;

            // We insert this new point into slot # <index>

            // Bump down more distant information to make room for this new point.
            for (i = maxOrder - 2; i >= index; i--)
            {
                F[i+1] = F[i];
                ID[i+1] = ID[i];
                delta[i+1] = delta[i];
            }
            // Insert the new point's information into the list.
            F[index] = d2;
            ID[index] = this_id;
            delta[index] = dx;
        }
    }
}

void
addSamples(long xi, long yi, long maxOrder,
           float at[2], float *F, float(*delta)[2], unsigned long *ID)
{
    float dx, dy, fx, fy, d2;
    long count, i, j, index;
    unsigned long seed, this_id;

    // Each cube has a random number seed based on the cube's ID number.
    // The seed might be better if it were a nonlinear hash like Perlin uses
    // for noise but we do very well with this faster simple one.
    // Our LCG uses Knuth-approved constants for maximal periods.
    seed = 702395077 * xi + 915488749 * yi;

    // How many feature points are in this cube?
    count = poissonCount[seed>>24]; // 256 element lookup table. Use MSB

    seed = 1402024253 * seed + 586950981; // churn the seed with good Knuth LCG

    for (j = 0; j < count; j++) // test and insert each point into our solution
    {
        this_id = seed;
        seed = 1402024253 * seed + 586950981; // churn

        // compute the 0..1 feature point location's XYZ
        fx = (seed + 0.5) * (1.0 / 4294967296.0);
        seed = 1402024253 * seed + 586950981; // churn
        fy = (seed + 0.5) * (1.0 / 4294967296.0);
        seed = 1402024253 * seed + 586950981; // churn

        // delta from feature point to sample location
        dx = xi + fx - at[0];
        dy = yi + fy - at[1];
        d2 = dx * dx + dy * dy; // Euclidian distance, squared

        if (d2 < F[maxOrder-1]) // Is this point close enough to rememember?
        {
            // Insert the information into the output arrays if it's close enough.
            // We use an insertion sort.  No need for a binary search to find
            // the appropriate index.. usually we're dealing with order 2,3,4 so
            // we can just go through the list. If you were computing order 50
            // (wow!!) you could get a speedup with a binary search in the sorted
            // F[] list.

            index = maxOrder;
            while (index > 0 && d2 < F[index-1]) index--;

            // We insert this new point into slot # <index>

            // Bump down more distant information to make room for this new point.
            for (i = maxOrder - 2; i >= index; i--)
            {
                F[i+1] = F[i];
                ID[i+1] = ID[i];
                delta[i+1][0] = delta[i][0];
                delta[i+1][1] = delta[i][1];
            }
            // Insert the new point's information into the list.
            F[index] = d2;
            ID[index] = this_id;
            delta[index][0] = dx;
            delta[index][1] = dy;
        }
    }
}

void
addSamples(long xi, long yi, long zi, long maxOrder,
           float at[3], float *F, float(*delta)[3], unsigned long *ID)
{
    float dx, dy, dz, fx, fy, fz, d2;
    long count, i, j, index;
    unsigned long seed, this_id;

    // Each cube has a random number seed based on the cube's ID number.
    // The seed might be better if it were a nonlinear hash like Perlin uses
    // for noise but we do very well with this faster simple one.
    // Our LCG uses Knuth-approved constants for maximal periods.
    seed = 702395077 * xi + 915488749 * yi + 2120969693 * zi;

    // How many feature points are in this cube?
    count = poissonCount[seed>>24]; // 256 element lookup table. Use MSB

    seed = 1402024253 * seed + 586950981; // churn the seed with good Knuth LCG

    for (j = 0; j < count; j++) // test and insert each point into our solution
    {
        this_id = seed;
        seed = 1402024253 * seed + 586950981; // churn

        // compute the 0..1 feature point location's XYZ
        fx = (seed + 0.5) * (1.0 / 4294967296.0);
        seed = 1402024253 * seed + 586950981; // churn
        fy = (seed + 0.5) * (1.0 / 4294967296.0);
        seed = 1402024253 * seed + 586950981; // churn
        fz = (seed + 0.5) * (1.0 / 4294967296.0);
        seed = 1402024253 * seed + 586950981; // churn

        // delta from feature point to sample location
        dx = xi + fx - at[0];
        dy = yi + fy - at[1];
        dz = zi + fz - at[2];
        d2 = dx * dx + dy * dy + dz * dz; // Euclidian distance, squared

        if (d2 < F[maxOrder-1]) // Is this point close enough to rememember?
        {
            // Insert the information into the output arrays if it's close enough.
            // We use an insertion sort.  No need for a binary search to find
            // the appropriate index.. usually we're dealing with order 2,3,4 so
            // we can just go through the list. If you were computing order 50
            // (wow!!) you could get a speedup with a binary search in the sorted
            // F[] list.

            index = maxOrder;
            while (index > 0 && d2 < F[index-1]) index--;

            // We insert this new point into slot # <index>
    
            // Bump down more distant information to make room for this new point.
            for (i = maxOrder - 2; i >= index; i--)
            {
                F[i+1] = F[i];
                ID[i+1] = ID[i];
                delta[i+1][0] = delta[i][0];
                delta[i+1][1] = delta[i][1];
                delta[i+1][2] = delta[i][2];
            }
            // Insert the new point's information into the list.
            F[index] = d2;
            ID[index] = this_id;
            delta[index][0] = dx;
            delta[index][1] = dy;
            delta[index][2] = dz;
        }
    }
}

} // namespace
