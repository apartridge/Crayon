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

float Image::luminance(float r, float g, float b)
{
    return 683*(0.21*r, 0.72*g, 0.07*b); // Lecture
    //return (1/0.17697) * (0.17697*r + 0.81240*g + 0.01063*b); // Wiki (CIE color space)
    //return (0.21*r, 0.72*g, 0.07*b); // No constant (http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html)
}

void Image::doGammaCorrection()
{
    const int n = m_width*m_height;
    float avg = 0;

    for (int i = 0; i < n; ++i)
    {
        Vector3 color = m_light[i];
        color.x = pow(color.x, (1/float(2.2)));
        color.y = pow(color.y, (1/float(2.2)));
        color.z = pow(color.z, (1/float(2.2)));

        m_pixels[i].r = Map( color.x );
        m_pixels[i].g = Map( color.y );
        m_pixels[i].b = Map( color.z );
    }
}

void Image::doLinearToneMapping()
{
    if (m_light == 0 || m_pixels == 0)
       return;

    float maxVal = 0;
    const float n = m_width*m_height;
    for (int i = 0; i < n; ++i)
    {
        float maxColorChannel = std::max(m_light[i].x, std::max(m_light[i].y, m_light[i].z));
        maxVal = std::max(maxVal, maxColorChannel);
    }

    printf("Maximum color channel: %f\n", maxVal);
    if (maxVal == 0)
        return;

    for (int i = 0; i < n; ++i)
    {
        Vector3& color = m_light[i];
        m_pixels[i].r = Map( color.x / maxVal );
        m_pixels[i].g = Map( color.y / maxVal );
        m_pixels[i].b = Map( color.z / maxVal );
    }
}

void Image::doToneMapping(float alpha, float saturation)
{
    if (m_light == 0 || m_pixels == 0)
        return;
    /*
    float a = alpha, b = saturation;

    const int n = m_width*m_height;
    float maxIntensity = 0;

    // Find max intensity
    for (int i = 0; i < n; ++i)
    {
        maxIntensity = std::max(maxIntensity, m_light[i].x);
        maxIntensity = std::max(maxIntensity, m_light[i].y);
        maxIntensity = std::max(maxIntensity, m_light[i].z);
    }

    // Hax method
    for (int i = 0; i < n; ++i)
    {
        Vector3 color(
            //std::min(pow(m_light[i].x / maxIntensity, 0.35f)*1.1f, 1.0f),
            //std::min(pow(m_light[i].y / maxIntensity, 0.35f)*1.1f, 1.0f),
            //std::min(pow(m_light[i].z / maxIntensity, 0.35f)*1.1f, 1.0f)
            1/(1+exp(-a*m_light[i].x-b)),
            1/(1+exp(-a*m_light[i].y-b)),
            1/(1+exp(-a*m_light[i].z-b))
        );
        
        m_pixels[i].r = Map( color.x );
        m_pixels[i].g = Map( color.y );
        m_pixels[i].b = Map( color.z );
    }*/

    const int n = m_width*m_height;
    float avg = 0;

    for (int i = 0; i < n; ++i)
    {
        const float lum = luminance(m_light[i].x, m_light[i].y, m_light[i].z);
        avg += log(epsilon + lum);
    }

    avg = exp( (1/float(n)) * avg );

    for (int i = 0; i < n; ++i)
    {
        const float Lv = luminance(m_light[i].x, m_light[i].y, m_light[i].z);
        const float white = luminance(1, 1, 1);
        const float scaled = (alpha / avg) * Lv;
        const float Ld = (scaled / (1 + scaled)) * (1 + (scaled / (white*white)));

        // Tone mapping back to RGB from luminance scaling
        Vector3 color( 
            pow((m_light[i].x/Lv), saturation) * Ld,
            pow((m_light[i].y/Lv), saturation) * Ld,
            pow((m_light[i].z/Lv), saturation) * Ld
        );

        // Gamma correction
        color.x = pow(color.x, (1/float(2.2)));
        color.y = pow(color.y, (1/float(2.2)));
        color.z = pow(color.z, (1/float(2.2)));

        // Update pixel values
        m_pixels[i].r = Map( color.x );
        m_pixels[i].g = Map( color.y );
        m_pixels[i].b = Map( color.z );
    }
}

void Image::drawScanline(int y)
{
    glRasterPos2f(-1, -1 + 2*y / (float)m_height);
    glDrawPixels(m_width, 1, GL_RGB, GL_UNSIGNED_BYTE, &m_pixels[y*m_width]);
}

void Image::draw()
{
    printf("Image::draw called\n");
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
