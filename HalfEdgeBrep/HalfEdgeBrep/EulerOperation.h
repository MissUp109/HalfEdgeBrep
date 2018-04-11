//
//  EulerOperation.h
//  HalfEdgeBrep
//
//  Created by yy on 2017/11/29.
//  Copyright © 2017年 Lynn. All rights reserved.
//

#ifndef EulerOperation_h
#define EulerOperation_h
#include "StructDefine.h"
#include <vector>
using namespace std;
class EulerOperation {
private:
    vector<Solid *> solidList;
    vector<Face *> faceList;
    vector<Loop *> loopList;
    vector<Vertex *> vertexList;
public:
    Solid *mvfs(double coord[3]);
    
    HalfEdge *mev(Vertex *v1, double coord[3], Loop *&lp);
    
    Loop *mef(Loop *&lp, Vertex *v1, Vertex *v2);
    
    Loop *kemr(Loop *&lp, Edge *e);
    
    void kfmrh(Face *&f1, Face *f2);
    
    Solid *sweep(Face *f, double dir[3], double dis);
    
    Solid *sweepWithHandle(Face *f, double dir[3], double dis);
    
    Solid *SolidWithoutSweep();
    
    //Solid *SolidWithHandleWithourSweep();
    
    Solid *SolidWithSweep();
    
    Solid *SolidWithOneHandle();
    
    Solid *SolidWithTwoHandle();
};

#endif /* EulerOperation_h */
