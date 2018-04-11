//
//  main.cpp
//  ShowFlag
//
//  Created by yy on 16/3/23.
//  Copyright (c) 2016年 Lynn. All rights reserved.
//

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "EulerOperation.h"

void Render(){
    EulerOperation eulerOp;
    eulerOp.apply();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    {
        glColor3f(1.0,0.0,0.0);
        glVertex2f(0, .5);
        glColor3f(0.0,1.0,0.0);
        glVertex2f(-.5,-.5);
        glColor3f(0.0, 0.0, 1.0);
        glVertex2f(.5, -.5);
    }
    glEnd();
}

/*
int main(int argc, char *argv[]){
    EulerOperation eulerOp;
    eulerOp.apply();
    return 0;
}
*/

int main(int argc, char *argv[]){
    GLFWwindow* win;
    
    //Initialize the library.
    if(!glfwInit()){
        return -1;
    }
    
    //Create a windowed mode window and its OpenGL context.
    win = glfwCreateWindow(640, 480, "OpenGL Base Project", NULL, NULL);
    if(!win)
    {
        glfwTerminate();
        return -1;
    }
    
    //Make the window's context current.
    glfwMakeContextCurrent(win);
    
    //Loop until the user closes the window
    while(!glfwWindowShouldClose(win)){
        Render();
        
        //Swap front and back buffers.
        glfwSwapBuffers(win);
        
        //Poll for and process events.
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}


