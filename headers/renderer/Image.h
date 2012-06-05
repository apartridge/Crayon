#ifndef CRAYON_IMAGE_H_INCLUDED
#define CRAYON_IMAGE_H_INCLUDED

#include "geometry/Vector3.h"

class Image
{
public:
    struct Pixel
    {
        unsigned char r, g, b;
        Pixel(unsigned char ir, unsigned char ig, unsigned char ib) {set(ir, ig, ib);}
        Pixel() : r(0), g(0), b(0) {}
        void set(unsigned char ir, unsigned char ig, unsigned char ib) {r = ir; g = ig; b = ib;}
    };

    Image();
    ~Image();

    void resize(int width, int height);
    void setPixel(int x, int y, const Vector3& p);
    void setPixel(int x, int y, const Pixel& p);

    // Convert raw light data to pixels
    void doToneMapping(float alpha, float saturation);
    void doLinearToneMapping();
    void doGammaCorrection();

    inline static float luminance(float r, float g, float b);

    void draw();
    void drawScanline(int y);
    void clear(const Vector3& c);
    void writePPM(char* pcFile); // write data to a ppm image file
    void writePPM(char *pcName, unsigned char *data, int width, int height);

    unsigned char* getCharPixels()  {return (unsigned char*)m_pixels;}
    int width() const               {return m_width;}
    int height() const              {return m_height;}

    int m_height;
private:
    Pixel* m_pixels;
    Vector3* m_light;
    int m_width;
};

extern Image * g_image;

#endif
