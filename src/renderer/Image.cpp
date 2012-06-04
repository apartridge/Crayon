#include "renderer/Miro.h"
#include "renderer/Image.h"
#include "renderer/glut.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

#ifdef WIN32
// disable useless warnings
#pragma warning(disable:4996)
#endif

Image * g_image = 0;

Image::Image()
{
    m_pixels = 0;
    m_light = 0;
    m_width = 1;
    m_height = 1;
}

Image::~Image()
{
    if (m_pixels)
        delete [] m_pixels;
    if (m_light)
        delete [] m_light;
}

void Image::resize(int width, int height)
{
    if (m_pixels)
        delete [] m_pixels;
    if (m_light)
        delete [] m_light;
    
    m_pixels = 0;
    m_pixels = new Pixel[width*height];
    memset(m_pixels, 0, width*height*sizeof(Pixel));
    
    m_light = new Vector3[width*height];
    
    m_width = width;
    m_height = height;
}

void Image::clear(const Vector3& c)
{
    // should be bg color
    for (int y=0; y<m_height; y++)
        for (int x=0; x<m_width; x++)
            setPixel(x, y, c);
}

// map floating point values to byte values for pixels
unsigned char Map(float r)
{
    float rMap = 255*r;
    unsigned char c = rMap>255?255:(unsigned char)rMap;
    return c;
}

void Image::setPixel(int x, int y, const Vector3& p)
{
    // do some tone mapping
    if (x >= 0 && x < m_width && y < m_height && y >= 0)
    {
        m_pixels[y*m_width+x].r = Map(p.x);
        m_pixels[y*m_width+x].g = Map(p.y);
        m_pixels[y*m_width+x].b = Map(p.z);
    }

    // Store straight up light as well
    m_light[y*m_width+x] = p;
}

void Image::setPixel(int x, int y, const Pixel& p)
{
    // do some tone mapping
    if (x >= 0 && x < m_width && y < m_height && y >= 0)
    {
        m_pixels[y*m_width+x]= p;
    }
}

void Image::doToneMapping(float alpha)
{
    if (m_light == 0 || m_pixels == 0)
        return;

    const int n = m_width*m_height;
    float avg = 0;

    for (int i = 0; i < n; ++i)
    {
        const float luminance = 683 * (0.21*m_light[i].x, 0.72*m_light[i].y, 0.07*m_light[i].z);
        avg += log(epsilon + luminance);
    }
    avg = exp( (1/float(n)) * avg );

    for (int i = 0; i < n; ++i)
    {
        const float luminance = 683 * (0.21*m_light[i].x, 0.72*m_light[i].y, 0.07*m_light[i].z);
        const float scaled = (alpha / avg) * luminance;
        const float compressed = scaled / (1 + scaled);

        // Tone mapping back to RGB from luminance scaling
        Vector3 color( 
            (m_light[i].x/luminance) * compressed,
            (m_light[i].y/luminance) * compressed,
            (m_light[i].z/luminance) * compressed
        );

        // Gamma correction
        color.x = pow(color.x, (1/2.2f));
        color.y = pow(color.y, (1/2.2f));
        color.z = pow(color.z, (1/2.2f));

        // Update pixel values
        m_pixels[i].r = 255; // Map( color.x );
        m_pixels[i].g = 0; // Map( color.y );
        m_pixels[i].b = 0; // Map( color.z );
    }
}

void Image::drawScanline(int y)
{
    glRasterPos2f(-1, -1 + 2*y / (float)m_height);
    glDrawPixels(m_width, 1, GL_RGB, GL_UNSIGNED_BYTE, &m_pixels[y*m_width]);
}

void Image::draw()
{
    for (int i = 0; i < m_height; i++)
        drawScanline(i);
	glFlush();
}

void Image::writePPM(char* pcFile)
{
    writePPM(pcFile, (unsigned char*)m_pixels, m_width, m_height);
}

void Image::writePPM(char *pcFile, unsigned char *data, int width, int height)
{
    FILE *fp = fopen(pcFile, "wb");
    if (!fp)
        fprintf(stderr, "Couldn't open PPM file %s for writing\n", pcFile);
    else
    {
        fprintf(fp, "P6\n");
        fprintf(fp, "%d %d\n", width, height );
        fprintf(fp, "255\n" );

        // invert image
        int stride = width*3;
        for (int i = height-1; i >= 0; i--)
            fwrite(&data[stride*i], stride, 1, fp);
        fclose(fp);
    }
}
