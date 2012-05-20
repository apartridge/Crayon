#ifndef CRAYON_GLUT_WINDOW_H_INCLUDED
#define CRAYON_GLUT_WINDOW_H_INCLUDED

class Window
{
public:
    Window(int * argc, char* argv[]);
    
    void mainLoop();
    
    // GLUT Event handlers
    void display();
    void reshape(int x, int y);
    void keyboard(unsigned char key, int x, int y);
    void mouse(int btn, int state, int x, int y);
    void motion(int x, int y);
	void setWindowTitle(char* subtitle);
    
protected:
    float m_scaleFact;
    int   m_activeButton;
    int   m_mouseX, m_mouseY;
};

#endif // CRAYON_GLUT_WINDOW_H_INCLUDED


