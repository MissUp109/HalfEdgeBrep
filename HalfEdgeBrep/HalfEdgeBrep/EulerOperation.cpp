//
//  EulerOperation.cpp
//  HalfEdgeBrep
//
//  Created by yy on 2017/11/29.
//  Copyright © 2017年 Lynn. All rights reserved.
//

#include <iostream>
//#include <fstream>
#include <string>
#include "EulerOperation.h"

using namespace std;

Solid *EulerOperation::mvfs(double coord[3]){
    Solid *solid = new Solid();
    Face *face = new Face();
    Loop *loop = new Loop();
    Vertex *vertex = new Vertex(coord[0], coord[1], coord[2]);
    
    solid->sFace = face;
    solid->nxt = solid;
    
    face->fSolid = solid;
    face->fLoop = loop;
    face->nxt = face;
    
    loop->lFace = face;
    loop->nxt = loop;
    
    solid->sid = (int)solidList.size();
    face->fid = (int)faceList.size();
    loop->lid = (int)loopList.size();
    vertex->vid = (int)vertexList.size();
    
    solidList.push_back(solid);
    faceList.push_back(face);
    loopList.push_back(loop);
    vertexList.push_back(vertex);
    
    return solid;
}

HalfEdge *EulerOperation::mev(Vertex *v1, double coord[3], Loop *&lp){
    Edge *newEdge = new Edge();
    HalfEdge *he1 = new HalfEdge();
    HalfEdge *he2 = new HalfEdge();
    HalfEdge *he = new HalfEdge();
    Vertex *v2 = new Vertex(coord[0], coord[1], coord[2]);
    
    v2->vid = (int)vertexList.size();
    vertexList.push_back(v2);
    
    newEdge->he1 = he1;
    newEdge->he2 = he2;
    
    he1->v = v1;
    he1->edge = newEdge;
    he1->pair = he2;
    he1->wloop = lp;
    
    
    he2->v = v2;
    he2->edge = newEdge;
    he2->pair = he1;
    he2->wloop = lp;
    
    if(lp->lHalfEdge == NULL){
        he2->nxt = he1;
        he1->nxt = he2;
        lp->lHalfEdge = he1;
    }
    else{
        for(he = lp->lHalfEdge; he->nxt->v != v1; he = he->nxt);
        he2->nxt = he->nxt;
        he->nxt = he1;
        he1->nxt = he2;
        lp->lHalfEdge = he1;
    }
    
    return he1;
}

Loop *EulerOperation::mef(Loop *&lp, Vertex *v1, Vertex *v2){
    //v1是尾点，v2是首点
    Face *newFace = new Face();
    Loop *newLoop = new Loop();
    Edge *newEdge = new Edge();
    HalfEdge *he1 = new HalfEdge();
    HalfEdge *he2 = new HalfEdge();

    newEdge->he1 = he1;
    newEdge->he2 = he2;
    
    he1->v = v1;
    he1->edge = newEdge;
    he1->pair = he2;
    
    he2->v = v2;
    he2->edge = newEdge;
    he2->pair = he1;
    
    //构造两个loop
    HalfEdge *tmpHe1;
    HalfEdge *tmpHe2;
    for(tmpHe1 = lp->lHalfEdge; tmpHe1->nxt->v != v1; tmpHe1 = tmpHe1->nxt);
    for(tmpHe2 = lp->lHalfEdge; tmpHe2->nxt->v != v2; tmpHe2 = tmpHe2->nxt);
    
    he2->nxt = tmpHe1->nxt;
    he1->nxt = tmpHe2->nxt;
    tmpHe2->nxt = he2;
    tmpHe1->nxt = he1;
    
    he1->wloop = lp;
    for(tmpHe1 = he1->nxt; tmpHe1 != he1; tmpHe1 = tmpHe1->nxt)
        tmpHe1->wloop = lp;
    
    he2->wloop = newLoop;
    for(tmpHe2 = he2->nxt; tmpHe2 != he2; tmpHe2 = tmpHe2->nxt)
        tmpHe2->wloop = newLoop;
    
    //更新原来的loop属性
    //lp->lFace = newFace; //lp属于mvfs构造的face
    lp->lHalfEdge = he1;
    //lp->nxt = lp->lFace->fLoop;
    
    //构造新的loop，将其放进loopList
    newLoop->lFace = newFace;
    newLoop->lHalfEdge = he2;
    newLoop->nxt = newLoop;
    newLoop->lid = (int)loopList.size();
    loopList.push_back(newLoop);
    
    //将新的face放进solid和faceList
    Solid *solid = lp->lFace->fSolid;
    newFace->fSolid = solid;
    newFace->fLoop = newLoop;
    
    Face *tmp;//原始solid的face列表的最后一个face
    for(tmp = solid->sFace; tmp->nxt != solid->sFace; tmp = tmp->nxt);
    tmp->nxt = newFace;
    newFace->nxt = solid->sFace;
    solid->sFace = newFace;
    newFace->fid = (int)faceList.size();
    faceList.push_back(newFace);
    
    return newLoop;
}

//lp为底面的环，e为桥边
Loop *EulerOperation::kemr(Loop *&lp, Edge *e){
    Loop *newLoop = new Loop();
    Vertex *v1 = e->he1->v;  //v1为外环的顶点
    Vertex *v2 = e->he2->v;  //v2为内环的顶点
    
    HalfEdge *tmp1 = lp->lHalfEdge;
    for(; tmp1->nxt->v != v1; tmp1 = tmp1->nxt);
    tmp1 = tmp1->nxt;
    for(; tmp1->nxt->v != v1; tmp1 = tmp1->nxt);
    
    HalfEdge *tmp2 = lp->lHalfEdge;
    for(; tmp2->nxt->v != v2; tmp2 = tmp2->nxt);
    //tmp2 = tmp2->nxt;
    //for(; tmp2->nxt->v != v2; tmp2 = tmp2->nxt);
    
    //断开内外连接循环
    tmp1->nxt = e->he2->nxt;
    tmp2->nxt = e->he1->nxt;
    
    //更新lp
    lp->lHalfEdge = tmp1;
    
    //更新lp，并构造newLoop
    if(tmp2 == e->he1){
        newLoop->lHalfEdge = nullptr;
    }
    else{//如果等于，则默认值为null
        newLoop->lHalfEdge = tmp2;
    }
    
    newLoop->lid = (int)loopList.size();
    loopList.push_back(newLoop);
    
    Face *face = lp->lFace;
    newLoop->lFace = face;
    
    Loop *tmp;//每个面的内环新loop接到外环的后面
    for(tmp = face->fLoop; tmp->nxt != face->fLoop; tmp = tmp->nxt);
    tmp->nxt = newLoop;
    newLoop->nxt = face->fLoop;
    
    //删除边
    delete e;
    
    return newLoop;
}


void EulerOperation::kfmrh(Face *&f1, Face *f2){//删除f2
    //f2的所有loop插入到f1的loop的尾部
    Loop *tmpLp1 = f1->fLoop;
    for(; tmpLp1->nxt != f1->fLoop; tmpLp1 = tmpLp1->nxt);
    Loop *f2_lp = f2->fLoop;
    f2->fLoop = nullptr;
    Loop *tmpLp2 = f2_lp;//f2应该只有一个loop
    for(; tmpLp2->nxt != f2_lp; tmpLp2 = tmpLp2->nxt);
    
    tmpLp1->nxt = f2_lp;
    tmpLp2->nxt= f1->fLoop;
    
    Loop *tmp;
    for(tmp = f1->fLoop; tmp->nxt != f1->fLoop; tmp = tmp->nxt)
        tmp->lFace = f1;
    tmp->lFace = f1;//f2会在这里发生变化
    
    //将f2从其所属的solid的sfaces链表里删除
    Face *tmpFace = f2->fSolid->sFace;
    for(;tmpFace->nxt != f2; tmpFace = tmpFace->nxt);
    tmpFace->nxt = f2->nxt;
    
    //将f2从faceList里删除
    vector<Face *>::iterator itr = find(faceList.begin(), faceList.end(), f2);
    faceList.erase(itr);
    for(int i = 0; i < faceList.size(); ++i){
        faceList[i]->fid = i;
    }

    //删除f2
    delete f2;
}

Solid *EulerOperation::sweepWithHandle(Face *f, double dir[3], double dis){
    //这里的f应该是扫成底面 f1
    //Solid *solid = new Solid();
    
    //删除多个循环扫成造成的多个面
    Face *topFace;

    //遍历每个面
    for(Loop *loop = f->fLoop; ; loop = loop->nxt){
        Loop *loop_0, *loop_1;
        
        //扫成内环时loop_0和loop_1反一下所以两种情况一样
        //if(loop == f->fLoop){
        loop_0 = loop->lHalfEdge->pair->wloop;
        loop_1 = loop;
        //}
        //else{
            //loop_0 = loop;
            //loop_1= loop->lHalfEdge->pair->wloop;
        //}
        
        HalfEdge *he1 = loop_1->lHalfEdge;
        Vertex *v1 = he1->v;
        double coord[3];
        coord[0] = v1->x + dir[0] * dis;
        coord[1] = v1->y + dir[1] * dis;
        coord[2] = v1->z + dir[2] * dis;
        HalfEdge *firstH = mev(v1, coord, loop_0)->pair;
        
        HalfEdge *he;
        Vertex *nxt_v;
        HalfEdge *prvH = firstH, *newH;
        
        for(he = he1->nxt; he != he1; he = he->nxt){
            nxt_v = he->v;
            coord[0] = nxt_v->x + dir[0] * dis;
            coord[1] = nxt_v->y + dir[1] * dis;
            coord[2] = nxt_v->z + dir[2] * dis;
            
            newH = mev(nxt_v, coord, loop_0)->pair;
            mef(loop_0, newH->v, prvH->v);
            prvH = newH;
        }
        mef(loop_0, firstH->v, prvH->v);
        
        if(loop == f->fLoop){
            topFace = loop_0->lFace;
        }
        else{
            kfmrh(topFace, loop_0->lFace);
        }
        if(loop->nxt == f->fLoop) break;
    }
    
    return solidList[0];
}

//sweep还有问题
Solid *EulerOperation::sweep(Face *f, double dir[3], double dis){
    //这里的f应该是扫成底面
    //Solid *solid = new Solid();
    Loop *loop_1 = f->fLoop;
    Loop *loop_0 = loop_1->lHalfEdge->pair->wloop;
    //间隔遍历loop，每两个半边环为一个物理环
    for(Loop *loop = loop_1; ; loop = loop->nxt){
        HalfEdge *he1 = loop_1->lHalfEdge;
        Vertex *v1 = he1->v;
        double coord[3];
        coord[0] = v1->x + dir[0] * dis;
        coord[1] = v1->y + dir[1] * dis;
        coord[2] = v1->z + dir[2] * dis;
        HalfEdge *firstH = mev(v1, coord, loop_0)->pair;
        
        HalfEdge *he;
        Vertex *nxt_v;
        HalfEdge *prvH = firstH, *newH;
        
        for(he = he1->nxt; he != he1; he = he->nxt){
            nxt_v = he->v;
            coord[0] = nxt_v->x + dir[0] * dis;
            coord[1] = nxt_v->y + dir[1] * dis;
            coord[2] = nxt_v->z + dir[2] * dis;
            
            newH = mev(nxt_v, coord, loop_0);
            newH = newH->pair;
            mef(loop_0, newH->v, prvH->v);
            prvH = newH;
        }
        Loop *topLoop = mef(loop_0, firstH->v, prvH->v);
        
        //删除多个循环扫成造成的多个面
        Face *topFace;
        if(loop == loop_1)
            topFace = topLoop->lFace;
        else{
            kfmrh(topFace, topLoop->lFace);
        }
        
        if(loop->nxt == f->fLoop) break;
    }
    
    return solidList[0];
}

Solid *EulerOperation::SolidWithoutSweep(){
    double pArray[4][3] = {
        {0, 0, 0}, {2, 0, 0}, {2, 0, -2}, {0, 0,  -2}
    };//X, Y, Z
    //申请空间
    Solid *solid = mvfs(pArray[0]);
    Loop * loop = solid->sFace->fLoop;
    //构建立方体
    for(int i = 0; i < 3; ++i){
        mev(vertexList[i], pArray[i + 1], loop);
    }
    mef(loop, vertexList[3], vertexList[0]);
    
    HalfEdge *he1 = loop->lHalfEdge->pair;
    Vertex *v1 = he1->v;
    HalfEdge *firstH;//mev生成的第一个以vup1为起点的半边
    double coord[3];
    coord[0] = v1->x;
    coord[1] = v1->y + 2;
    coord[2] = v1->z;
    firstH = mev(v1, coord, loop)->pair;
    
    HalfEdge *he;
    Vertex *nxt_v;
    HalfEdge *prvH = firstH, *newH;
    
    for(he = he1->nxt; he != he1; he = he->nxt){
        nxt_v = he->v;
        coord[0] = nxt_v->x;
        coord[1] = nxt_v->y + 2;
        coord[2] = nxt_v->z;
        //nxt_vup = new Vertex(coord[0], coord[1], coord[2]);
        newH = mev(nxt_v, coord, loop);
        newH = newH->pair;
        mef(loop, newH->v, prvH->v);//new prv顺序
        prvH = newH;
    }
    mef(loop, firstH->v, prvH->v);
    return solid;
}

//生成没有通孔的实体
Solid *EulerOperation::SolidWithSweep(){
    double pArray[4][3] = {
        {0, 0, 0}, {2, 0, 0}, {2, 0, -2}, {0, 0,  -2}
    };//X, Y, Z
    
    //申请空间
    Solid * solid = mvfs(pArray[0]);
    Loop *outLoop = solid->sFace->fLoop;
    
    //构建底面外环
    for(int i = 0; i < 3; ++i){
        mev(vertexList[i], pArray[i + 1], outLoop);
    }
    mef(outLoop, vertexList[3], vertexList[0]);
    
    double *dir = new double[3];
    dir[0] = 0;
    dir[1] = 1;
    dir[2] = 0;
    double dis = 2;
    
    return sweep(solid->sFace, dir, dis);
}

//生成一个通孔的实体
Solid *EulerOperation::SolidWithOneHandle(){
    double pArray[4][3] = {
        {0, 0, 0}, {2, 0, 0}, {2, 0, -2}, {0, 0,  -2}
    };//X, Y, Z
    
    //申请空间
    mvfs(pArray[0]);
    Loop *loop_0 = solidList[0]->sFace->fLoop;
    
    //构建底面外环
    for(int i = 0; i < 3; ++i){
        mev(vertexList[i], pArray[i + 1], loop_0);
    }
    mef(loop_0, vertexList[3], vertexList[0]);
    
    Loop *loop_1 = loopList[1];
    //构建底面内环
    double inArray[4][3]={
        {0.5, 0, -0.5}, {1.5, 0, -0.5}, {1.5, 0, -1.5}, {0.5, 0, -1.5}
    };
     //构建底面内环
    HalfEdge *he1 = mev(vertexList[0], inArray[0], loop_1);
    Loop *inLoop = kemr(loop_1, he1->edge);
    for(int i = 0; i < 3; ++i){
        mev(vertexList[4 + i], inArray[1 + i], inLoop);
    }
    //下面mef生成的face放在facelist[2]里
    mef(inLoop, vertexList[7], vertexList[4]);
    
    //扫成方向和距离
    double *dir = new double[3];
    dir[0] = 0;
    dir[1] = 1;
    dir[2] = 0;
    double dis = 2;
    
    return sweepWithHandle(faceList[1], dir, dis);
}

//生成两个通孔的实体
Solid *EulerOperation::SolidWithTwoHandle(){
    double pArray[4][3] = {
        {0, 0, 0}, {5, 0, 0}, {5, 0, -5}, {0, 0,  -5}
    };//X, Y, Z
    
    //申请空间
    mvfs(pArray[0]);
    Loop *loop_0 = solidList[0]->sFace->fLoop;
    
    //构建底面外环
    for(int i = 0; i < 3; ++i){
        mev(vertexList[i], pArray[i + 1], loop_0);
    }
    mef(loop_0, vertexList[3], vertexList[0]);
    
    Loop *loop_1 = loopList[1];
    
    //构建第1个底面内环
    double inArray1[4][3]={
        {1, 0, -1}, {2, 0, -1}, {2, 0, -4}, {1, 0, -4}
    };
    HalfEdge *he1 = mev(vertexList[0], inArray1[0], loop_1);
    Loop *inLoop = kemr(loop_1, he1->edge);
    for(int i = 0; i < 3; ++i){
        mev(vertexList[4 + i], inArray1[1 + i], inLoop);
    }
    //下面mef生成的face放在facelist[2]里
    mef(inLoop, vertexList[7], vertexList[4]);
    
    double inArray2[4][3]={
        {3, 0, -1}, {4, 0, -1}, {4, 0, -4}, {3, 0, -4}
    };
    
    //构建第2个底面内环
    he1 = mev(vertexList[0], inArray2[0], loop_1);
    inLoop = kemr(loop_1, he1->edge);
    for(int i = 0; i < 3; ++i){
        mev(vertexList[8 + i], inArray2[1 + i], inLoop);
    }
    //下面mef生成的face放在facelist[3]里
    mef(inLoop, vertexList[11], vertexList[8]);
    
    //扫成方向和距离
    double *dir = new double[3];
    dir[0] = 0;
    dir[1] = 1;
    dir[2] = 0;
    double dis = 2;
    
    return sweepWithHandle(faceList[1], dir, dis);
}


