//
//  main.cpp
//  HalfEdgeBrep
//
//  Created by yy on 2017/12/3.
//  Copyright © 2017年 Lynn. All rights reserved.
//

#include <stdlib.h>
#include <GLUT/GLUT.h>
#include <stdio.h>
#include <string.h>
#include "EulerOperation.h"

#ifndef CALLBACK
#define CALLBACK
#endif

float eye[] = {0, 4, 6};
float center[] = {0, 0, 0};
float fDistance = 0.2f;
float fRotate = 0;
bool bAnim = false;
bool isLineMode = false;

void DrawWithoutSweep(){
    EulerOperation eulerOp;
    Solid *s = eulerOp.SolidWithoutSweep();
    Face *f = s->sFace;
    Loop *lp;
    HalfEdge *he;
    Vertex *v;
    for(; f->nxt != s->sFace; f = f->nxt ){
        lp = f->fLoop;
        glBegin(GL_POLYGON);
        for(he = lp->lHalfEdge; he->nxt != lp->lHalfEdge; he = he->nxt){
            v = he->v;
            glVertex3f(v->x, v->y, v->z);
        }
        v = he->v;
        glVertex3f(v->x, v->y, v->z);
        glEnd();
    }
    
    lp = f->fLoop;
    glBegin(GL_POLYGON);
    for(he = lp->lHalfEdge; he->nxt != lp->lHalfEdge; he = he->nxt){
        v = he->v;
        glVertex3f(v->x, v->y, v->z);
    }
    v = he->v;
    glVertex3f(v->x, v->y, v->z);
    glEnd();
    
}

void DrawWithSweep(){
    EulerOperation eulerOp;
    Solid *s = eulerOp.SolidWithSweep();
    
    Face *f = s->sFace;
    Loop *lp;
    HalfEdge *he;
    Vertex *v;
    for(; f->nxt != s->sFace; f = f->nxt ){
        lp = f->fLoop;
        glBegin(GL_POLYGON);
        for(he = lp->lHalfEdge; he->nxt != lp->lHalfEdge; he = he->nxt){
            v = he->v;
            glVertex3f(v->x, v->y, v->z);
        }
        v = he->v;
        glVertex3f(v->x, v->y, v->z);
        glEnd();
    }
    
    lp = f->fLoop;
    glBegin(GL_POLYGON);
    for(he = lp->lHalfEdge; he->nxt != lp->lHalfEdge; he = he->nxt){
        v = he->v;
        glVertex3f(v->x, v->y, v->z);
    }
    v = he->v;
    glVertex3f(v->x, v->y, v->z);
    glEnd();
}

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

void DrawWithHandle(){
    EulerOperation eulerOp;
    //Solid *s = eulerOp.SolidWithSweep();
    //Solid *s = eulerOp.SolidWithHandle();
    Solid *s = eulerOp.SolidWithTwoHandle();
    Face *f;
    Loop *lp;
    HalfEdge *he;
    Vertex *v;
    
    GLUtesselator * tessobj;
    tessobj = gluNewTess();
    //注册回调函数
    gluTessCallback(tessobj, GLU_TESS_VERTEX, (void (CALLBACK *)())vertexCallback);
    gluTessCallback(tessobj, GLU_TESS_BEGIN, (void (CALLBACK *)())beginCallback);
    gluTessCallback(tessobj, GLU_TESS_END, (void (CALLBACK *)())endCallback);
    gluTessCallback(tessobj, GLU_TESS_ERROR, (void (CALLBACK *)())errorCallback);
    
    //设置环绕规则
    gluTessProperty(tessobj, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_POSITIVE);//negative

    for(f = s->sFace; f->nxt != s->sFace; f = f->nxt){
        gluTessBeginPolygon(tessobj, NULL);
        for(lp = f->fLoop; ; lp = lp->nxt){
            gluTessBeginContour(tessobj);//定义轮廓线1 逆时针矩形
            for(he = lp->lHalfEdge; ; he = he->nxt){
                v = he->v;
                double *coord = new double[3];
                coord[0] = v->x;
                coord[1] = v->y;
                coord[2] = v->z;
                gluTessVertex(tessobj, coord, coord);
                if(he->nxt == lp->lHalfEdge) break;
            }
            gluTessEndContour(tessobj);
            if(lp->nxt == f->fLoop) break;
        }
        
        gluTessEndPolygon(tessobj);
    }

}


void reshape(int width, int height)
{
    if (height==0)                                        // Prevent A Divide By Zero By
    {
        height=1;                                        // Making Height Equal One
    }
    
    glViewport(0,0,width,height);                        // Reset The Current Viewport
    
    glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
    glLoadIdentity();                                    // Reset The Projection Matrix
    
    float whRatio = (GLfloat)width/(GLfloat)height;
    gluPerspective(45, whRatio, 1, 1000);
    
    glMatrixMode(GL_MODELVIEW);                            // Select The Modelview Matrix
}

void idle()
{
    glutPostRedisplay();
}

void key(unsigned char k, int x, int y)
{
    switch(k)
    {
        case 27:
        case 'q': {exit(0); break; }
        case 'l':
        {
            isLineMode = !isLineMode;
            break;
        }
        case 'a':
        {
            eye[0] += fDistance;
            center[0] += fDistance;
            break;
        }
        case 'd':
        {
            eye[0] -= fDistance;
            center[0] -= fDistance;
            break;
        }
        case 'w':
        {
            eye[1] -= fDistance;
            center[1] -= fDistance;
            break;
        }
        case 's':
        {
            eye[1] += fDistance;
            center[1] += fDistance;
            break;
        }
        case 'z':
        {
            eye[2] *= 0.95;
            break;
        }
        case 'c':
        {
            eye[2] *= 1.05;
            break;
        }
        case ' ':
        {
            bAnim = !bAnim;
            break;
        }
        default: break;
    }
}

void redraw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.9, 0.9, 0.9, 1);
    glLoadIdentity();                                    // Reset The Current Modelview Matrix
    
    gluLookAt(eye[0], eye[1], eye[2],
              center[0], center[1], center[2],
              0, 1, 0);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    GLfloat gray[] = { 0.4, 0.4, 0.4, 1.0 };
    GLfloat pink[] = {0.9, 0.5, 0.8, 1.0};
    GLfloat light_pos[] = {10, 10, 10, 1};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,pink);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, gray);
    glEnable(GL_LIGHT0);
    
    if (bAnim) fRotate += 0.5f;
    glRotatef(fRotate, 1.0f, 0.0f, 0);            // Rotate around Y axis
    glScalef(0.4, 0.4, 0.4);
    
    if(isLineMode){
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else{
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    
    //DrawWithSweep();
    //DrawWithoutSweep();
    DrawWithHandle();
 
    glutSwapBuffers();
}



int main (int argc,  char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(480,480);
    int windowHandle = glutCreateWindow("test");
    
    glutDisplayFunc(redraw);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(key);
    //glutMouseFunc(MousePress);
    //glutMotionFunc(MouseMotion);
    glutIdleFunc(idle);
    
    glutMainLoop();
    return 0;
}



