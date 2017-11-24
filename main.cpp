#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>
#include <GLUT/glut.h>
#include <vector>

#include "ResourcePath.hpp"

using namespace std;
#define PI 3.14159


void gluPerspective( GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar )
{
    GLdouble xmin, xmax, ymin, ymax;
    
    ymax = zNear * tan( fovy * M_PI / 360.0 );
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;
    
    glFrustum( xmin, xmax, ymin, ymax, zNear, zFar );
}


float distance(float *a, float *b){
    return sqrt((a[0]-b[0])*(a[0]-b[0]) + (a[1]-b[1])*(a[1]-b[1]) + (a[2]-b[2])*(a[2]-b[2]));
}


void drawSphere(float x, float y, float z, float radius){
    
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texcoords;
    std::vector<GLushort> indices;
    
    int rings = 128;
    int sectors = 256;
    
    float const R = 1./(float)(rings-1);
    float const S = 1./(float)(sectors-1);
    int r, s;
    
    
    
    vertices.resize(rings * sectors * 3);
    normals.resize(rings * sectors * 3);
    texcoords.resize(rings * sectors * 2);
    std::vector<GLfloat>::iterator v = vertices.begin();
    std::vector<GLfloat>::iterator n = normals.begin();
    std::vector<GLfloat>::iterator t = texcoords.begin();
    for(r = 0; r < rings; r++) for(s = 0; s < sectors; s++) {
        float const y = sin( -M_PI_2 + M_PI * r * R );
        float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
        float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );
        
        *t++ = s*S;
        *t++ = r*R;
        
        *v++ = x * radius;
        *v++ = y * radius;
        *v++ = z * radius;
        
        *n++ = x;
        *n++ = y;
        *n++ = z;
    }
    
    indices.resize(rings * sectors * 4);
    std::vector<GLushort>::iterator i = indices.begin();
    for(r = 0; r < rings-1; r++) for(s = 0; s < sectors-1; s++) {
        *i++ = r * sectors + s;
        *i++ = r * sectors + (s+1);
        *i++ = (r+1) * sectors + (s+1);
        *i++ = (r+1) * sectors + s;
    }
    
    glPushMatrix();
    glTranslatef(0,0,0);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
    glNormalPointer(GL_FLOAT, 0, &normals[0]);
    glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);
    glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
    glPopMatrix();
    
}

int main(int, char const**)
{
    
    
    sf::RenderWindow window(sf::VideoMode(800, 600), "Solar System",sf::Style::Default,sf::ContextSettings(32));
    window.setVerticalSyncEnabled(true);
        sf::Image icon;
    if (!icon.loadFromFile(resourcePath() + "icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    
    
    

    
    
    sf::Texture texEarth;
    if (!texEarth.loadFromFile(resourcePath() + "texEarth.jpg")) {
        return EXIT_FAILURE;
    }
    
    sf::Texture texMoon;
    if (!texMoon.loadFromFile(resourcePath() + "texMoon.jpg")) {
        return EXIT_FAILURE;
    }
    
    sf::Texture texVenus;
    if (!texVenus.loadFromFile(resourcePath() + "texVenus.jpg")) {
        return EXIT_FAILURE;
    }
    
    sf::Texture texMercury;
    if (!texMercury.loadFromFile(resourcePath() + "texMercury.jpg")) {
        return EXIT_FAILURE;
    }
    
    sf::Texture texSun;
    if (!texSun.loadFromFile(resourcePath() + "texSun.jpg")) {
        return EXIT_FAILURE;
    }
    
    sf::Texture texUniverse;
    if (!texUniverse.loadFromFile(resourcePath() + "texUniverse.jpg")) {
        return EXIT_FAILURE;
    }
    
    sf::Texture texMars;
    if (!texMars.loadFromFile(resourcePath() + "texMars.jpg")) {
        return EXIT_FAILURE;
    }
    

    float rotate_x = 0;
    float rotate_y = 0;
    
    bool running = true;
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


    float width = 800;
    float height = 600;
    
    float speed = 0.1;
    
    
    const GLfloat LIGHT_AMBIENT[]       = { 1, 1, 1, 100};
    float position[] = {0.0f, 0.0f, 500.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT,  LIGHT_AMBIENT);
    
    bool z = false, q = false, s = false, d = false, space = false, c = false;
    
    
    float camX = 0, camY = 0, camZ = 0;
    float rCamX = 0, rCamY = 180, rCamZ = 0;
    
    
    int lastX = -1, lastY = -1;
    float i = 0;
    float multi = 10;
    
    const GLfloat LIGHT_0_POSITION[]    = { 0.0, 0.0, 500.0, 0.0};
    const GLfloat LIGHT_MODEL_AMBIENT[] = { 0.8, 0.8, 0.8, 1.0};
    
    vector<float*> positions;
    float pos1[] = {0,0,500,109};
    float pos2[] = {0,0,500-280,15};
    float pos3[] = {0,0,(500-280)-30,8};
    float pos4[] = {0,0,500-239,14};
    positions.push_back(pos1);
    positions.push_back(pos2);
    positions.push_back(pos3);
    positions.push_back(pos4);
    
    
    bool inEarth = false;
    
    
    while (running && window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::Resized)
            {
                glViewport(0, 0, event.size.width, event.size.height);
                width = event.size.width;
                height = event.size.height;
                
            }
            if(event.type == sf::Event::KeyPressed){
                if(event.key.code == sf::Keyboard::Z){
                    z = true;
                }
                if(event.key.code == sf::Keyboard::S){
                    s = true;
                }
                if(event.key.code == sf::Keyboard::Q){
                    q = true;
                }
                if(event.key.code == sf::Keyboard::D){
                    d = true;
                }
                if(event.key.code == sf::Keyboard::Space){
                    space = true;
                }
                if(event.key.code == sf::Keyboard::C){
                    c = true;
                }
                if(event.key.code == sf::Keyboard::M){
                    speed+=0.1;
                }
                if(event.key.code == sf::Keyboard::L){
                    speed-=0.1;
                }
                if(event.key.code == sf::Keyboard::T){
                    inEarth = !inEarth;
                }
                
            }else if(event.type == sf::Event::KeyReleased){
                if(event.key.code == sf::Keyboard::Z){
                    z = false;
                }
                if(event.key.code == sf::Keyboard::S){
                    s = false;
                }
                if(event.key.code == sf::Keyboard::Q){
                    q = false;
                }
                if(event.key.code == sf::Keyboard::D){
                    d = false;
                }
                if(event.key.code == sf::Keyboard::Space){
                    space = false;
                }
                if(event.key.code == sf::Keyboard::C){
                    c = false;
                }
                
            }else if(event.type == sf::Event::MouseMoved){
                if(lastX != -1 && lastY != -1){
                    int vx = event.mouseMove.x - lastX;
                    int vy = event.mouseMove.y - lastY;
                    
                    rCamY += vx;
                    if(rCamY < 0){
                        rCamY += 360;
                    }
                    if(rCamY > 360){
                        rCamY -= 360;
                    }
                    rCamX += vy;
                }
                
                lastX = event.mouseMove.x;
                lastY = event.mouseMove.y;
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                running = false;
            }
        }
        
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LIGHT_MODEL_AMBIENT);
        glEnable(GL_LIGHTING);
        
        
        
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45, (float)(width)/height, 0.01, 100000);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT| GL_STENCIL_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        
        glLoadIdentity();
        glPushMatrix();
            glLightfv(GL_LIGHT0, GL_POSITION, LIGHT_0_POSITION);
        glPopMatrix();
        
        glPushMatrix();
        
        if(!inEarth){
            
            glRotatef(rCamX, 1, 0, 0);
            glRotatef(rCamY, 0, 1, 0);
            glRotatef(rCamZ, 0, 0, 1);
            
            glTranslatef(camX, camY, camZ);
        }
        else{
            //Not working for now
        }
        
        
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        
        
        
        
        
        if(z){
            camZ += multi * cos(rCamY/180*PI);
            camX += multi * -sin(rCamY/180*PI);
            camY += multi * sin(rCamX/180*PI);
            
        }
        
        if(s){
            camZ -= multi * cos(rCamY/180*PI);
            camX -= multi * -sin(rCamY/180*PI);
            camY -= multi * sin(rCamX/180*PI);
        }
        
        if(q){
            camX += multi * cos(rCamY/180*PI);
            camZ += multi * sin(rCamY/180*PI);
        }
        
        if(d){
            camX -= multi * cos(rCamY/180*PI);
            camZ -= multi * sin(rCamY/180*PI);
        }
        if(space){
            camY -= multi;
        }
        
        if(c){
            camY += multi;
        }
        
        i+=speed;

        
        sf::Texture::bind(&texUniverse);
        
        glPushMatrix();
            glRotatef((float)i/32, 1, 1, 1);
            glTranslatef(0, 0, 0);
        
            glColor3f(1, 1, 1);
            drawSphere(0, 0, 0, 10000);
        
        glPopMatrix();

        
        //SUN
        glPushMatrix();
            sf::Texture::bind(&texSun);
            if(inEarth)
                glTranslatef(0, 0,  (500-358)-camZ);
            else
                glTranslatef(0, 0, 500);
            glRotatef(i/16, 1, 1, 1);
            glColor3f(1, 1, 1);
            drawSphere(0, 0, 0, 109);
        
        
        //MERCURY
        sf::Texture::bind(&texMercury);
        
        glPushMatrix();
            glRotatef(i*17, 1, 1, 0);
            glTranslatef(0, 0, -166);
            glColor3f(1, 1, 1);
            drawSphere(0, 0, 0, 8.8);
        glPopMatrix();
        
        //VENUS
        sf::Texture::bind(&texVenus);
        
        glPushMatrix();
            glRotatef(i*12, 1, 1, 0);
            glTranslatef(0, 0, -217);
            glColor3f(1, 1, 1);
            drawSphere(0, 0, 0, 14.4);
        glPopMatrix();
        
        
        //MARS
        sf::Texture::bind(&texMars);
        glPushMatrix();
            glRotatef(i*8, 1, 1, 0);
            glTranslatef(0, 0, -486);
            glColor3f(1, 1, 1);
            drawSphere(0, 0, 0, 15);
        glPopMatrix();

        
            //EARTH
            sf::Texture::bind(&texEarth);
        
            glPushMatrix();
                glRotatef(i*10, 1, 0, 0);
                glTranslatef(0, 0, -358);
      
                glColor3f(1, 1, 1);
                drawSphere(0, 0, 0, 15);
        
        
        
                glPushMatrix();
        
                    //MOON
        
                    sf::Texture::bind(&texMoon);
        
                    glRotatef(i*100, 0, 1, 0);
                    glTranslatef(0, 0, -21);
        
                    glColor3f(1, 1, 1);
                    drawSphere(0, 0, 0, 4);
                glPopMatrix();

        
            glPopMatrix();
        
        
        glPopMatrix();
        
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
        glDisable(GL_LIGHTING);
        glFlush();
        window.display();

    }
    
    return EXIT_SUCCESS;
}
