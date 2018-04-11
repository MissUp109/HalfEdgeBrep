//
//  maint.cpp
//  HalfEdgeBrep
//
//  Created by yy on 2017/12/2.
//  Copyright © 2017年 Lynn. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <GLUT/glut.h>
#include "EulerOperation.h"

#ifndef CALLBACK
#define CALLBACK
#endif

//顶点的回调函数
void CALLBACK vertexCallback(GLvoid *vertex)
{
    GLdouble* pt;
    pt = (GLdouble*)vertex;
    glColor3d(0.0, 0.7, 0.7);
    glVertex3dv(pt);
}

void CALLBACK beginCallback(GLenum type)
{
    glBegin(type);
}

void CALLBACK endCallback()
{
    glEnd();
}

void CALLBACK errorCallback(GLenum errorCode)
{
    const GLubyte * estring;
    //打印错误类型
    estring = gluErrorString(errorCode);
    fprintf(stderr, "Tessellation Error: %s/n", estring);
    exit(0);
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
}

void drawGraphics(GLint nproperty)
{
    int i;
    GLUtesselator * tessobj;
    tessobj = gluNewTess();
    //注册回调函数
    gluTessCallback(tessobj, GLU_TESS_VERTEX, (void (CALLBACK *)())vertexCallback);
    gluTessCallback(tessobj, GLU_TESS_BEGIN, (void (CALLBACK *)())beginCallback);
    gluTessCallback(tessobj, GLU_TESS_END, (void (CALLBACK *)())endCallback);
    gluTessCallback(tessobj, GLU_TESS_ERROR, (void (CALLBACK *)())errorCallback);
    
    //设置环绕规则
    gluTessProperty(tessobj, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD);
    
    
    gluTessBeginPolygon(tessobj, NULL);
    gluTessBeginContour(tessobj);       //定义轮廓线1 逆时针矩形
    //gluTessVertex(tessobj, graphics[0][0], graphics[0][0]);
    //gluTessVertex(tessobj, graphics[0][1], graphics[0][1]);
    //gluTessVertex(tessobj, graphics[0][2], graphics[0][2]);
    //gluTessVertex(tessobj, graphics[0][3], graphics[0][3]);
    gluTessEndContour(tessobj);
    
    gluTessBeginContour(tessobj);       //定义轮廓线2 逆时针矩形
    //gluTessVertex(tessobj, graphics[1][0], graphics[1][0]);
    //gluTessVertex(tessobj, graphics[1][1], graphics[1][1]);
    //gluTessVertex(tessobj, graphics[1][2], graphics[1][2]);
    //gluTessVertex(tessobj, graphics[1][3], graphics[1][3]);
    gluTessEndContour(tessobj);

    gluTessEndPolygon(tessobj);
}

float eye[3] = {0.0, 0.0, 20.0};
float center[3] = {0.0, 0.0, 0.0};

void reshape(GLsizei w, GLsizei h)
{
    GLfloat fovy = 45;
    GLfloat aspect = 1;
    if (0 == h) h = 1;
    
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    fovy = 45;
    aspect = (GLfloat)w/h;
    if (w < h)
        fovy = fovy * (GLfloat)h / w;
    
    gluPerspective(fovy, aspect, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt(eye[0], eye[1], eye[2],
              center[0], center[1], center[2],
              0, 1, 0);
}

void mydraw(){
    EulerOperation eulerOP;
    Solid *s = eulerOP.apply();
    
    Face *face = s->sFace;
    Loop *loop = face->fLoop;
    HalfEdge *he = loop->lHalfEdge;
    Vertex *v = he->v;
    
    while(face->nxt != s->sFace){
    
        glColor3f(0.0, 0.7, 0.7);
        glBegin(GL_POLYGON);
        while(he->nxt != loop->lHalfEdge){
            v = he->v;
            glVertex3f(v->x, v->y, v->z);
            he = he->nxt;
        }
        glEnd();
        
        face = face->nxt;
    }
}

void mydraw1(){
    glColor3f(0.0, 0.7, 0.0);
    
    glBegin(GL_POLYGON);
    glVertex3f(-10, 10, 10);
    glVertex3f(-10, -10, 10);
    glVertex3f(10, -10, 10);
    glVertex3f(10, 10, 10);
    glEnd();
    
    glBegin(GL_POLYGON);
    glVertex3f(10, 10, 10);
    glVertex3f(10, -10, 10);
    glVertex3f(10, -10, -10);
    glVertex3f(10, 10, -10);
    glEnd();
    
    glBegin(GL_POLYGON);
    glVertex3f(10,10, -10);
    glVertex3f(10, -10, -10);
    glVertex3f(-10, -10, -10);
    glVertex3f(-10, 10, -10);
    glEnd();
    
    glBegin(GL_POLYGON);
    glVertex3f(-10, 10, -10);
    glVertex3f(-10, -10, -10);
    glVertex3f(-10, -10, 10);
    glVertex3f(-10, 10, 10);
    glEnd();
    
    
    glBegin(GL_POLYGON);
    glVertex3f(-10, 10, -10);
    glVertex3f(-10, 10, 10);
    glVertex3f(10, 10, 10);
    glVertex3f(10, 10, -10);
    glEnd();
    
    glBegin(GL_POLYGON);
    glVertex3f(-10, -10, -10);
    glVertex3f(10, -10, -10);
    glVertex3f(10, -10, 10);
    glVertex3f(-10, -10, 10);
    glEnd();
    
}

static float rot = 0.0;


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glLoadIdentity();
    
    glPushMatrix();
    glTranslated(-16.0, 16.0, -90.0);
    //drawGraphics(nProperty);

    glRotatef(rot, 1, 1, 1);

    //glPolygonMode(GL_FRONT_AND_BACK   ,GL_LINE);
    //mydraw();
    mydraw1();

    glPopMatrix();
    
    glFlush();
}

void idle(){
    rot += 1;
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("test");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
