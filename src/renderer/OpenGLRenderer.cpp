#include "renderer/openglrenderer.h"
extern Vector3 pointInFocusLocked;
extern bool pointInFocusShow;

OpenGLRenderer::OpenGLRenderer()
{

}

void glutPrint(float x, float y, void* font, char* text, float r, float g, float b, float a) 
{ 
    if(!text || !strlen(text)) return; 
    bool blending = false; 
    if(glIsEnabled(GL_BLEND)) blending = true; 
    glEnable(GL_BLEND); 
    glColor4f(r,g,b,a); 
    glRasterPos2f(x,y); 
    while (*text) { 
        glutBitmapCharacter(font, *text); 
        text++; 
    } 
    if(!blending) glDisable(GL_BLEND); 
}  

void OpenGLRenderer::drawScene(Scene& scene, Camera& camera, Image* image)
{
    glClearColor(camera.bgColor().x, camera.bgColor().y, camera.bgColor().z, 1);
	glClear(0);
    glDrawBuffer(GL_BACK);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set up the screen with our camera parameters
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	//glutPrint(1.0f, 1.0f, GLUT_BITMAP_HELVETICA_12, "Hello World!", 1.0f, 0.0f, 0.0f, 0.5f);

    gluPerspective(camera.fov(), g_image->width()/(float)g_image->height(),
                    0.01, 10000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    Vector3 vCenter = camera.eye() + camera.viewDir();
    gluLookAt(camera.eye().x, camera.eye().y, camera.eye().z,
                vCenter.x, vCenter.y, vCenter.z,
                camera.up().x, camera.up().y, camera.up().z);

	if (camera.getOpenGLShowTriad())
    {
		triad3D();
	}

	if (camera.openGLRenderMode() == Camera::OPENGL_RENDER_FILL)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	// Draw lights
	for (int i = 0; i < scene.lights()->size(); i++)
	{
		scene.lights()->at(i)->renderGL();
	}

	// Draw A Point In Focus. Press L.
	if (pointInFocusShow)
	{
		glColor4f(1, 0, 0, 1);
		glTranslatef(pointInFocusLocked.x, pointInFocusLocked.y, pointInFocusLocked.z);
		glutSolidSphere(0.1, 20, 20);
		glTranslatef(-pointInFocusLocked.x, -pointInFocusLocked.y, -pointInFocusLocked.z);
	}

    // Draw objects
    if (camera.openGLRenderMode() == Camera::OPENGL_RENDER_FILL || 
	    camera.openGLRenderMode() == Camera::OPENGL_RENDER_WIRE)
	{
		for (size_t i = 0; i < scene.objects()->size(); ++i)
		{
			scene.objects()->at(i)->renderGL();
		}

	}
	else if (camera.openGLRenderMode() == Camera::OPENGL_RENDER_BOUNDINGBOX)
	{
		glColor3f(1,0,0);
		drawVolumeNode(scene.entireScene, 0);
	}

    // Draw photon map (todo: key toggle)
    scene.photonMap()->renderGL();

    glutSwapBuffers();
}

extern int show_level;
extern int levels[100];

void OpenGLRenderer::drawVolumeNode(BoundingVolumeNode& a, int level)
{
	if(level > show_level){
		return;
	}

	glColor3f(1, 1, 1);

	if(level == show_level)
	{
		//cube(a.box);

		if(a.num_leaves == 0)
		{
			if(levels[show_level] == 1)
			{
				glColor3f(1,0,0);
				cube(a.children[0].box);
				glColor3f(1,0.4,0.4);
				a.children[0].renderGL();
			}
			else if(levels[show_level] == 2)
			{
				

				glColor3f(0,0,1);
				cube(a.children[1].box);
				glColor3f(0.4,0.4,1);
				a.children[1].renderGL();
			}

		}
		else
		{
			a.renderGL();
		}
	}

	if(a.children != NULL && levels[level] == 1)
	{
		drawVolumeNode(a.children[0], level+1);
	}
	if(a.children != NULL && levels[level] == 2)
	{
		drawVolumeNode(a.children[1], level+1);
	}
}


void OpenGLRenderer::onChangeTo()
{
	// Initialize some OpenGL state
	glEnable(GL_DEPTH_TEST);
    glClearColor(0.f, 0.f, 0.f, 1);
    glDisable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
}

void OpenGLRenderer::onChangeFrom()
{

}

void OpenGLRenderer::cube(BoundingBox& box)
{

	const Vector3& min = box.getMin();
	const Vector3& max = box.getMax();

	void* a = (void*)&max;

	glBegin(GL_QUADS);

	// side min.z
	glVertex3f(min.x, min.y, min.z);
	glVertex3f(min.x, max.y, min.z);
	glVertex3f(max.x, max.y, min.z);
	glVertex3f(max.x, min.y, min.z);

	// side max.z
	glVertex3f(min.x, min.y, max.z);
	glVertex3f(min.x, max.y, max.z);
	glVertex3f(max.x, max.y, max.z);
	glVertex3f(max.x, min.y, max.z);

	// Side max.x
	glVertex3f(max.x, min.y, min.z);
	glVertex3f(max.x, max.y, min.z);
	glVertex3f(max.x, max.y, max.z);
	glVertex3f(max.x, min.y, max.z);
	
	// Side min.x
	glVertex3f(min.x, min.y, min.z);
	glVertex3f(min.x, max.y, min.z);
	glVertex3f(min.x, max.y, max.z);
	glVertex3f(min.x, min.y, max.z);

	// Side max.y
	glVertex3f(min.x, max.y, min.z);
	glVertex3f(max.x, max.y, min.z);
	glVertex3f(max.x, max.y, max.z);
	glVertex3f(min.x, max.y, max.z);

	// Side min.y
	glVertex3f(min.x, min.y, min.z);
	glVertex3f(max.x, min.y, min.z);
	glVertex3f(max.x, min.y, max.z);
	glVertex3f(min.x, min.y, max.z);

	glEnd();

}

void OpenGLRenderer::axis3D ()
{
    GLUquadric* cyl = gluNewQuadric();

    gluCylinder (cyl, 0.02, 0.02, 0.8, 16, 1);        // Body of axis.

    glColor3f (1,1,1);                                // Make arrow head white.
    glPushMatrix ();
       glTranslatef (0.0, 0.0, 0.8);
       gluCylinder  (cyl, 0.04, 0.001, 0.1, 12, 1);   // Cone at end of axis.
    glPopMatrix ();
}

void OpenGLRenderer::triad3D ()
{
    glColor3f           (0.0, 0.0, 1.0);    // Z axis in blue.
    glRasterPos3f       (0.0, 0.0, 1.1);
    glutBitmapCharacter (GLUT_BITMAP_HELVETICA_18, 'z');
    axis3D ();

    glColor3f           (0.0, 1.0, 0.0);    // Y axis is green.
    glRasterPos3f       (0.0, 1.1, 0.0);
    glutBitmapCharacter (GLUT_BITMAP_HELVETICA_18, 'y');
    glPushMatrix ();
       glRotatef (-90, 1,0,0);
       axis3D ();
    glPopMatrix ();

    glColor3f           (1.0, 0.0, 0.0);    // X axis is red.
    glRasterPos3f       (1.1, 0.0, 0.0);
    glutBitmapCharacter (GLUT_BITMAP_HELVETICA_18, 'x');
    glPushMatrix ();
       glRotatef (90, 0,1,0);
       axis3D ();
    glPopMatrix ();
}
