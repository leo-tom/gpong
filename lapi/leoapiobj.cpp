#include <leoapi.h>
#include <mainwindow.h>
#include <sofobject.h>
#include <soflayer.h>
#include <string>
#include <math.h>

#ifndef M_PI
#define M_PI (3.141592653589793)
#endif

lapiobj objGetFromLayer(lapiLayer layer, const char *key){
    sofObject *ptr = ((SofLayer *)layer)->get(key);
    if(ptr == nullptr){
        return NULL;
    }
    return (lapiobj)ptr;
}
void objRemoveKeyFromLayer(lapiLayer layer, const char *key){
    ((SofLayer *)layer)->remove(key);
}
void objRemoveThisFromLayer(lapiLayer layer, lapiobj obj){
    ((SofLayer *)layer)->remove( l2q(obj) );
}

lapiobj objInit(const char *fileName){
    std::string str = fileName;
    sofObject *obj = new sofObject(str);
    return obj;
}
void objPutOnLayer(lapiLayer _layer,const char *key, lapiobj obj){
    SofLayer *layer = (SofLayer *)_layer;
    if(!key){
        std::string str;
        str = (long long)clock();
        str += (unsigned long long) obj;
        layer->add(str.c_str(),l2q(obj));
        return;
    }
    layer->add(key,l2q(obj));
}
int objGetWidth(lapiobj obj){
    return l2q(obj)->getWidth();
}
int objGetHeight(lapiobj obj){
    return l2q(obj)->getHeight();
}
int objGetSH(lapiobj obj){
    return l2q(obj)->getSH();
}
int objGetSW(lapiobj obj){
    return l2q(obj)->getSW();
}
int objGetSX(lapiobj obj){
    return l2q(obj)->getSX();
}
int objGetSY(lapiobj obj){
    return l2q(obj)->getSY();
}
int objGetX(lapiobj obj){
    return l2q(obj)->getX();
}
int objGetY(lapiobj obj){
    return l2q(obj)->getY();
}
double objGetTurnDegree(lapiobj obj){
    return l2q(obj)->getTurn() * 180.0f / M_PI;
}
double objGetTurnRadian(lapiobj obj){
    return l2q(obj)->getTurn();
}
double objGetTurn(lapiobj obj){
    return objGetTurnRadian(obj);
}
void objTurnDegree(lapiobj obj, double degree){
    l2q(obj)->Turn(degree / 180.0f * M_PI);
}
void objTurnRadian(lapiobj obj, double radian){
    l2q(obj)->Turn(radian);
}
void objSetSize(lapiobj obj, double newSize){
    l2q(obj)->setSize(newSize);
}
void objSetSH(lapiobj obj, int val){
    l2q(obj)->setSH(val);
}
void objSetSW(lapiobj obj, int val){
    l2q(obj)->setSW(val);
}
void objSetSX(lapiobj obj, int val){
    l2q(obj)->setSX(val);
}
void objSetSY(lapiobj obj, int val){
    l2q(obj)->setSY(val);
}
void objSetX(lapiobj obj, int x){
    l2q(obj)->setX(x);
}
void objSetY(lapiobj obj, int y){
    l2q(obj)->setY(y);
}

void objSetImage(lapiobj obj, lapiImage img){
    l2q(obj)->setImage((QImage *) img);
}
void objSetImageFromFile(lapiobj obj, const char *fileName){
    QString str(fileName);
    objSetImage(obj,new QImage(str));
}
void objSetMouseListener(lapiobj obj,void (*listener)(lapiobj, int)){
    l2q(obj)->mouseEvents.push_back(listener);
}
void objFinallize(lapiobj obj){
    delete l2q(obj);
}
