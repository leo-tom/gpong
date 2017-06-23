/*
 * (C) 2017 Leo Tomura
 *
 */

#ifndef LEOAPI_H
#define LEOAPI_H

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C"{
#endif

/*declations with public are objects/functions that are meant to be used as public*/
/*if private, those funcs or objects are meant to be private.
 * usually you shouldn't use these but if you are smart enough you may.*/

/*
 * Every single allocation of memory in the code of leoapi does not check whether returned pointer is NULL.
 * Just assume that malloc never returns NULL. If your environment is shitty enough to return NULL, dont play it on your environment.
 */

#define private
#define public
/*functions with done are the ones that have already written*/
#define done

#define CLOCKS_PER_MILLI (CLOCKS_PER_SEC / 10*3)
#define Sec2Milli(val)         (val * 10*3)
#define Milli2Nano(val)        (val * 10*6)
#define Nano2Milli(val)        (val / 10*6)
#define Milli2Sec(val)         (val / 10*3)

#ifndef M_PI
#define M_PI (3.14159265358979f)
#endif
#define M_DEG2RAD(degree) ( (degree / 180.0f) * M_PI)
#define M_RAD2DEG(radian) ( (radian / M_PI) * 180.0f)

/*lapi 2 qt */
#define l2q(ptr) ((sofObject *)ptr)

#include "pictures.h"

public typedef void* lapiobj;

#define FLAGS_PHYSICS_CALCULATED                     (1 << 0)
#define FLAGS_PHYSICS_SQUARE                         (1 << 1)
#define FLAGS_PHYSICS_CIRCLE_WIDTH                   (1 << 2)
#define FLAGS_PHYSICS_CIRCLE_HEIGHT                  (1 << 3)
#define FLAGS_PHYSICS_WALL                           (1 << 4)
#define FLAGS_PHYSICS_NO_DEFAULT_COLLISION_BEHAVIOUR (1 << 5)
#define FLAGS_PHYSICS_COLLISION_DISABLED             (1 << 6)
#define FLAGS_PHYSICS_MOVEMENT_DISABLED              (1 << 7)
#define FLAGS_PHYSICS_MOVEMENT_MOVETO                (1 << 8)
#define FLAGS_PHYSICS_MOVEMENT_VECTOR                (1 << 9)
#define FLAGS_PHYSICS_TURN_TO                        (1 << 10)
#define FLAGS_PHYSICS_TURN                           (1 << 11)

public typedef struct lapiPhysicalObject{
    lapiobj objData;
    unsigned int flag;
    void *targets; /*vector<char *> * */
    char *Name;
    /*value X and Y is used as new X and Y coordinate or vector of object*/
    /*if FLAGS_PHYSICS_MOVEMENT_VECTOR is set, object moves value of X and Y per one second*/
    double X;
    double Y;
    long mvRemainingTime; /*in nanosecond*/
    double turn; /*in radian*/
    long turnRemainingTime;
    /*Those buffs are used when object is moving according to vector*/
    double buff1;
    double buff2;
    /*If this function returns non-zero. Default event listener will never be called*/
    int (*whenHit)(struct lapiPhysicalObject *,struct lapiPhysicalObject *);
    int (*doesHit)(struct lapiPhysicalObject *me,struct lapiPhysicalObject *another);

}lapiphy;

typedef void* lapiLayer;
typedef void* lapiImage;

private extern void * mainWindow;
private extern void * physicalObjects; /*map<char *,vector<lapiPhysicalObject *> *>*/
private extern void * lapiObjects;     /*vector<lapiobj >*/
private extern lapiphy * _ball;
private extern lapiphy * _racket[2];
private extern lapiphy * _wall;
private extern lapiobj  _background;
private extern void * newFrameEvents; /*vector<void (*newFrame)() > * */
#define RACKET_PLAYER   (0)
#define RACKET_OPPONENT (1)

private extern int LEOAPI_FPS;
private extern int LEOAPI_FLAGS;
#define LEOFLAGS_RENDERING_THREAD_KILL (1 << 0)


private extern void _initLeoApi();
done private extern void _doPhysics(unsigned long nano_time);

done private extern void renderingThread();

#define LAYER_BACKGROUND (0)
#define LAYER_CENTER     (1)
#define LAYER_TOP        (2)

done public extern lapiLayer lapiGetLayer(int index);
done public extern int lapiGetFps();
done public extern void lapiSetFps(int fps);
done private extern lapiImage _loadImage(const char *fileName);/*returns QImage object*/
done public extern void lapiSetKeyListener(void (*handler)(int flag,int keyType));
done public extern void lapiRemoveKeyListener(void (*handler)(int flag,int keyType));
done public extern void lapiSetNewFrameEvent(void (*newFrame)());
done public extern void lapiRemoveNewFrameEvent(void (*newFrame)());
done public extern int lapiGetWidth();
done public extern int lapiGetHeight();
done public extern void lapiSetSize(int width,int height);
done public extern int lapiGetX();
done public extern int lapiGetY();
done public extern void lapiSetLocation(int x,int y);
done public extern lapiobj lapiGetBall();
done public extern lapiobj lapiGetPRacket(); /*get player's racket*/
done public extern lapiobj lapiGetORacket(); /*get opponent's racket*/

done public extern lapiobj objGetFromLayer(lapiLayer layer,const char *key);
done public extern lapiobj objInit(const char *fileName);/*Use macro when passing fileName*/
done public extern void objPutOnLayer(lapiLayer layer,const char *key,lapiobj obj);
done public extern void objRemoveKeyFromLayer(lapiLayer layer,const char *key);
done public extern void objRemoveThisFromLayer(lapiLayer layer,lapiobj obj);
done public extern int objGetWidth(lapiobj);
done public extern int objGetHeight(lapiobj);
done public extern int objGetSW(lapiobj);
done public extern int objGetSH(lapiobj);
done public extern int objGetSX(lapiobj);
done public extern int objGetSY(lapiobj);
done public extern int objGetX(lapiobj);
done public extern int objGetY(lapiobj);
done public extern double objGetTurnRadian(lapiobj);
done public extern double objGetTurnDegree(lapiobj);
done public extern double objGetTurn(lapiobj);
done public extern void objTurnRadian(lapiobj,double radian);
done public extern void objTurnDegree(lapiobj,double degree);
done public extern void objSetSize(lapiobj,double newSize); /* 0 <= newSize <= 1 */
done public extern void objSetSW(lapiobj,int val);
done public extern void objSetSH(lapiobj,int val);
done public extern void objSetSX(lapiobj,int val);
done public extern void objSetSY(lapiobj,int val);
done public extern void objSetX(lapiobj,int x);
done public extern void objSetY(lapiobj,int y);
done public extern void objSetImageFromFile(lapiobj,const char *fileName);/*Use macro when passing fileName*/
done public extern void objSetImage(lapiobj,lapiImage);
done public extern void objSetMouseListener(lapiobj,void (*listener)(lapiobj,int flag));
done public extern void objRemoveMouseListener(lapiobj,void (*listener)(lapiobj,int flag));
done public extern void objFinallize(lapiobj);

done public extern struct lapiPhysicalObject * phyInit(lapiobj,const char *name);
done public extern unsigned int phyGetFlag(lapiphy *);
done public extern void phyMoveVector(lapiphy *,double x,double y); /*per second*/
done public extern void phyMoveTo(lapiphy *,int x,int y,double second);
done public extern void phyTurnToRadian(lapiphy *,double radian,double second); /*takes "second" to turn "radian" */
done public extern void phyTurnToDegree(lapiphy *,double degree,double second);
done public extern void phyTurnRadian(lapiphy *obj,double radian);
done public extern void phyTurnDegree(lapiphy *obj,double radian);
done public extern void phyMoveStop(lapiphy *);
done public extern void phySetHitListener(lapiphy *obj,int (*whenHit)(lapiphy *,lapiphy *));
done public extern void * phyGetHitListener(lapiphy *obj);
done public extern void phySetName(lapiphy *obj,const char *name);
done public extern void phyAddTarget(lapiphy *obj,const char *target);
done public extern int phyGetX(lapiphy *obj);
done public extern int phyGetY(lapiphy *obj);
done public extern int phyGetXCenter(lapiphy *obj);
done public extern int phyGetYCenter(lapiphy *obj);
done public extern int phyGetWidth(lapiphy *obj);
done public extern int phyGetHeight(lapiphy *obj);
done public extern const char * phyGetName(lapiphy *obj);
done public extern char ** phyGetTargets(lapiphy *obj); /*Free return value for sure*/ /*It is not recommended to use those*/
done public extern int phyGetTargetsCount(lapiphy *obj);
done public extern lapiobj phyGetlapiObject(lapiphy *obj);
done public extern void phyDisableDefaultCollisionBehaviour(lapiphy *obj,bool disable);
done public extern void phyDisableCollision(lapiphy *obj,bool disable);
done public extern void phyDisableMovement(lapiphy *obj,bool disable);
public extern lapiobj phyFinalize(lapiPhysicalObject *obj);

#define KEY_PRESSED (1 << 0)
#define KEY_RELEASED (1 << 1)

#define MOUSE_RELEASED (1 << 0)
#define MOUSE_PRESSED  (1 << 1)
#define MOUSE_DOUBLE_CLICKED (1 << 2)

const extern int KEY_UP;
const extern int KEY_DOWN;
const extern int KEY_LEFT;
const extern int KEY_RIGHT;
const extern int KEY_SHIFT;
const extern int KEY_TAB;
const extern int KEY_ENTER;
const extern int KEY_CTRL;
const extern int KEY_ALT;
const extern int KEY_BACKSPACE;
const extern int KEY_ESCAPE;
const extern int KEY_A;
const extern int KEY_B;
const extern int KEY_C;
const extern int KEY_D;
const extern int KEY_E;
const extern int KEY_F;
const extern int KEY_G;
const extern int KEY_H;
const extern int KEY_I;
const extern int KEY_J;
const extern int KEY_K;
const extern int KEY_L;
const extern int KEY_M;
const extern int KEY_N;
const extern int KEY_O;
const extern int KEY_P;
const extern int KEY_Q;
const extern int KEY_R;
const extern int KEY_S;
const extern int KEY_T;
const extern int KEY_U;
const extern int KEY_V;
const extern int KEY_W;
const extern int KEY_X;
const extern int KEY_Y;
const extern int KEY_Z;

public extern const char LAPI_PHYNAME_BALL[];
public extern const char LAPI_PHYNAME_PRACKET[];
public extern const char LAPI_PHYNAME_ORACKET[];
public extern const char LAPI_PHYNAME_WALL[];


#undef private
#undef public
#undef done

#ifdef __cplusplus
}
#endif

#endif // LEOAPI_H
