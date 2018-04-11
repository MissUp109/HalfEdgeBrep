//
//  StructDefine.h
//  HalfEdgeBrep
//
//  Created by yy on 2017/11/29.
//  Copyright © 2017年 Lynn. All rights reserved.
//

#ifndef StructDefine_h
#define StructDefine_h

struct Solid;
struct Face;
struct Loop;
struct Edge;
struct HalfEdge;
struct Vertex;

struct Solid {
    int sid;            //体的编号
    Face *sFace;        //第一个face
    //Solid *prv;         //前一个体
    Solid *nxt;         //后一个体
    
    Solid():sid(0), sFace(nullptr), nxt(nullptr){}
    ~Solid(){}
};

struct Face {
    int fid;            //面的编号
    Solid *fSolid;      //所属的体
    Loop *fLoop;        //形成face的loop
    //Loop *inLoop;     //内部的第一个loop
    //int lpCount;      //内部loop个数
    //Face *prv;          //前一个face
    Face *nxt;          //下一个face
    
    Face():fid(0), fSolid(nullptr), fLoop(nullptr), nxt(nullptr){}
    ~Face(){}
};

struct Loop {
    int lid;            //环的编号
    Face *lFace;        //所属的face
    HalfEdge *lHalfEdge;//第一个edge
    //Loop *prv;          //前一个loop
    Loop *nxt;          //后一个loop
    
    Loop():lid(0), lFace(nullptr), lHalfEdge(nullptr), nxt(nullptr){}
    ~Loop(){}
};

struct Edge {
    //int eid;            //边的编号
    HalfEdge *he1;      //第一个半边
    HalfEdge *he2;      //第二个半边
    
    Edge():he1(nullptr), he2(nullptr){}
    ~Edge(){}
};

struct HalfEdge {
    Vertex *v;          //始点
    Edge *edge;         //所属物理边
    HalfEdge *pair;     //对应的半边
    //HalfEdge *prv;      //前一个半边
    HalfEdge *nxt;      //下一个半边
    Loop *wloop;        //所属环
    
    HalfEdge():v(nullptr), edge(nullptr), nxt(nullptr), wloop(nullptr){}
    ~HalfEdge(){}
};

struct Vertex {
    int vid;            //顶点编号
    double x;           //顶点坐标
    double y;
    double z;
    //Vertex *prv;        //前一个顶点
    //Vertex *nxt;        //后一个顶点
    //HalfEdge *vhe;    //所属的半边
    
    Vertex(double vx, double vy, double vz):vid(0){
        x = vx;
        y = vy;
        z = vz;
    }
    ~Vertex(){}
};

struct Point {
    double x;
    double y;
    double z;
    /*
    Point(double nx, double ny, double nz){
        x = nx;
        y = ny;
        z = nz;
    }
     */
};

#endif /* StructDefine_h */
