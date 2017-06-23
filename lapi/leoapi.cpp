
#include <leoapi.h>
#include <soflayer.h>
#include <mainwindow.h>
#include <vector>
#include <map>
#include <stdlib.h>

/*This function is used by _wall */
int _wall_doesHit(lapiphy *wall,lapiphy *phy){
    lapiobj obj = phyGetlapiObject(phy);
    int x = objGetY(obj);
    int y = objGetY(obj);
    if(x <= 0 || y <= 0 || x + objGetWidth(obj) >= lapiGetWidth() || y + objGetHeight(obj) >= lapiGetHeight()){
        return 1;
    }
    return 0;
}
int _wall_whenHit(lapiphy *wall, lapiphy *phy){
    return 1;
}

void _initLeoApi(){
    physicalObjects = (void *) new map<char *,vector<lapiPhysicalObject *> >;
    lapiObjects = (void *) new vector<lapiobj>;
    newFrameEvents = (void *) new std::vector<void (*)()>;
    _ball = phyInit(objInit(IMAGE_DEFAULT_BALL),LAPI_PHYNAME_BALL);
    _wall = (lapiphy *)calloc(1,sizeof(struct lapiPhysicalObject));
    _wall->flag |= FLAGS_PHYSICS_WALL;
    _wall->doesHit = _wall_doesHit;
    phySetHitListener(_wall,_wall_whenHit);
    _racket[RACKET_OPPONENT] = phyInit(objInit(IMAGE_DEFAULT_ORACKET),LAPI_PHYNAME_ORACKET);
    _racket[RACKET_PLAYER]   = phyInit(objInit(IMAGE_DEFAULT_PRACKET),LAPI_PHYNAME_PRACKET);

    _background = (lapiobj *) objInit(IMAGE_DEFAULT_BACKGROUND);
    LEOAPI_FPS = 30;
    LEOAPI_FLAGS = 0;
}

void lapiSetFps(int fps){
    LEOAPI_FPS = fps;
}
int lapiGetFps(){
    return LEOAPI_FPS;
}

lapiLayer lapiGetLayer(int index){
    return (void *) ((MainWindow *)mainWindow)->getLayer(index);
}

int _doesHitCirculer(lapiPhysicalObject *obj1, lapiPhysicalObject *obj2,int radius1,int radius2){
    register double tmp1,tmp2;
    tmp1 = phyGetXCenter(obj1) - phyGetXCenter(obj2);
    tmp1 = tmp1 * tmp1; /*even if tmp is negative, This code will make it positive*/
    tmp2 = phyGetYCenter(obj1) - phyGetYCenter(obj2);
    tmp2 = tmp2 * tmp2;
    tmp1 = sqrt(tmp1 + tmp2);
    /*now tmp has distance between center of obj1 and obj2*/
    return ( (tmp1 - (double)(radius1 + radius2) ) <= 0.0f ) ? 1 : 0;
}
int _doesHit(lapiPhysicalObject *obj1,lapiPhysicalObject *obj2){
    if(obj1->doesHit != NULL){
        return obj1->doesHit(obj1,obj2);
    }
    if(obj1->flag & FLAGS_PHYSICS_CIRCLE_HEIGHT){
        int radius1,radius2;
        radius1 = phyGetHeight(obj1) / 2;
        radius2 = phyGetHeight(obj2) / 2;
        return _doesHitCirculer(obj1,obj2,radius1,radius2);
    }else if(obj1->flag & FLAGS_PHYSICS_CIRCLE_WIDTH){
        int radius1,radius2;
        radius1 = phyGetWidth(obj1) / 2;
        radius2 = phyGetWidth(obj2) / 2;
        return _doesHitCirculer(obj1,obj2,radius1,radius2);
    }else{
        /*assume (obj1->flag & FLAGS_PHYSICS_SQUARE) is true*/
        double distance = (double)( phyGetX(obj2) - phyGetX(obj1) );
        if(distance > 0.0f){
            distance -= (double) phyGetWidth(obj1);
        }else{
            distance *= -1.0f;
            distance -= (double) phyGetWidth(obj2);
        }
        if(distance >= 0.0f)
            return 0;
        /*checking y coordinate also*/
        distance = (double)(phyGetY(obj2) - phyGetY(obj1));
        if(distance > 0.0f){
            distance -= (double) phyGetHeight(obj1);
        }else{
            distance *= -1.0f;
            distance -= (double)(phyGetHeight(obj2));
        }
        if(distance >= 0.0f)
            return 0;
        return 1;
    }
    /*never reach here*/
    return 0;
}

void _collide(lapiphy *obj1,lapiphy *obj2){
    /*if whenHit returns non-zero, default collision behaviour will never be executed.*/
    if(obj1->whenHit != NULL)
        if(obj1->whenHit(obj1,obj2)){
            if(obj2->whenHit != NULL)
                obj2->whenHit(obj2,obj1);
            return;
        }
    if(obj1->flag & FLAGS_PHYSICS_NO_DEFAULT_COLLISION_BEHAVIOUR)
        return;
    /*Default collision behaviour*/
    phyMoveStop(obj1);
}

/*nano_time == (10^9 / LEOAPI_FPS) */
void _doPhysics(unsigned long nano_time){
    std::map<char *,vector <lapiphy *> > *map = (std::map<char *,vector <lapiphy *> > *)physicalObjects;
    for(std::map<char *,vector <lapiphy *> >::iterator itMap = map->begin();
        itMap != map->end();
        ++itMap)
        {
        std::vector<lapiphy *> vec = (*itMap).second;
        for(std::vector<lapiphy *>::iterator itVec = vec.begin();
            itVec != vec.end();
            ++itVec)
            {
            lapiphy *obj = *itVec;
            vector<char *> *target = (vector<char *> *)obj->targets;
            if(target == NULL)
                continue;
            for(auto itTarget = target->begin();itTarget != ((vector<char *> *)obj->targets)->end();++itTarget){
                const char *key = *itTarget;
                vector<lapiphy *> &targetVec = (*map)[(char *)key];
                for(vector<lapiphy *>::iterator it = targetVec.begin();
                    it != targetVec.end();
                    ++it)
                {
                    lapiphy *hittingWith = *it;
                    if(hittingWith == _wall){
                        /*When hitting with wall, execute wall's doesHit*/
                        if(_wall_doesHit(_wall,obj)){
                            _collide(obj,_wall);
                        }
                    }else if(obj->doesHit){
                        if(obj->doesHit(obj,hittingWith)){
                            _collide(obj,hittingWith);
                        }
                    }else{
                        if (_doesHit(obj,hittingWith) ){
                            _collide(obj,hittingWith);
                        }
                    }

                }
            }
        }
    }
    for(auto itMap = map->begin();itMap != map->end();++itMap){
        std::vector<lapiphy *> &vec = (*itMap).second;
        for(auto itVec = vec.begin();itVec != vec.end();++itVec){
            lapiphy *phy = *itVec;
            lapiobj obj = phy->objData;
            if( ! (phy->flag & FLAGS_PHYSICS_MOVEMENT_DISABLED) ){
                if(obj== NULL){
                    /*does it even happen?*/
                    continue;
                }
                if(phy->flag & FLAGS_PHYSICS_MOVEMENT_VECTOR){
                    double move,tmp;
                    tmp = (phy->X * (double) nano_time / 1000000000.0f ) + phy->buff1;
                    move = (double)((long)tmp);
                    phy->buff1 = tmp - move; /*only values in decimal point*/
                    objSetX(obj,objGetX(obj) + (int)move);

                    tmp = (phy->Y * (double) nano_time / 1000000000.0f ) + phy->buff2;
                    move = (double)((long)tmp);
                    phy->buff2 = tmp - move;
                    objSetY(obj,objGetY(obj) + (int)move);
                }else if(phy->flag & FLAGS_PHYSICS_MOVEMENT_MOVETO){
                    double div;
                    div = (double) nano_time / (double)phy->mvRemainingTime;
                    objSetX(obj, phyGetX(phy) + (phy->X - phyGetX(phy) ) * div);
                    objSetY(obj, phyGetY(phy) + (phy->Y - phyGetY(phy) ) * div);
                    phy->mvRemainingTime -= nano_time;
                }
            }
            if( phy->flag & FLAGS_PHYSICS_TURN_TO){
                double div,turning;
                div = (double) nano_time / (double)phy->turnRemainingTime;
                /* value of phy->turn is in radian*/
                turning = phy->turn * div;
                phy->turn -= turning;
                phy->turnRemainingTime -= nano_time;
                objTurnRadian(obj,turning);
            }else if(phy->flag & FLAGS_PHYSICS_TURN){
                objTurnRadian(obj,objGetTurnRadian(phy->objData) + ( (double) nano_time / 1000000000.0f * phy->turn ) );
            }
        }
    }
}

lapiImage _loadImage(const char *fileName){
    QString str = fileName;
    QImage *img = new QImage(str,nullptr);
    if(!img->isNull())
        return img;
    delete img;
    return NULL;
}

void lapiSetKeyListener(void (*handler)(int, int)){
    MainWindow *w = (MainWindow *) mainWindow;
    w->setKeyListener(handler);
}
void lapiRemoveKeyListener(void (*handler)(int, int)){
    MainWindow *w = (MainWindow *) mainWindow;
    w->removeKeyListener(handler);
}

void lapiSetNewFrameEvent(void (*newFrame)()){
    vector<void (*)() > *vec = (vector<void (*)() > *)newFrameEvents;
    vec->push_back(newFrame);
}
void lapiRemoveNewFrameEvent(void (*newFrame)()){
    vector<void (*)() > *vec = (vector<void (*)() > *)newFrameEvents;
    for(auto it = vec->begin();it != vec->end();++it){
        void (*func)() = *it;
        if(func == newFrame){
            vec->erase(it);
            return;
        }
    }
}
int lapiGetWidth(){
    return ((MainWindow *)mainWindow)->width();
}
int lapiGetHeight(){
    return ((MainWindow *)mainWindow)->height();
}
void lapiSetSize(int w, int h){
    ((MainWindow *)mainWindow )->setFixedSize(w,h);
}
int lapiGetX(){
    return ((MainWindow *)mainWindow)->x();
}
int lapiGetY(){
    return ((MainWindow *)mainWindow)->y();
}
void lapiSetLocation(int x, int y){
    ((MainWindow *)mainWindow)->setGeometry(x,y,lapiGetWidth(),lapiGetHeight());
}

lapiobj lapiGetBall(){
    return _ball;
}
lapiobj lapiGetORacket(){
    return _racket[RACKET_OPPONENT];
}
lapiobj lapiGetPRacket(){
    return _racket[RACKET_PLAYER];
}



/*Define Keys*/
const int KEY_UP = Qt::Key_Up;
const int KEY_DOWN = Qt::Key_Down;
const int KEY_LEFT = Qt::Key_Left;
const int KEY_RIGHT = Qt::Key_Right;
const int KEY_ALT = Qt::Key_Alt;
const int KEY_BACKSPACE = Qt::Key_Backspace;
const int KEY_CTRL = Qt::Key_Control;
const int KEY_ENTER = Qt::Key_Enter;
const int KEY_ESCAPE = Qt::Key_Escape;
const int KEY_TAB = Qt::Key_Tab;
const int KEY_SHIFT = Qt::Key_Shift;
const int KEY_A = Qt::Key_A;
const int KEY_B = Qt::Key_B;
const int KEY_C = Qt::Key_C;
const int KEY_D = Qt::Key_D;
const int KEY_E = Qt::Key_E;
const int KEY_F = Qt::Key_F;
const int KEY_G = Qt::Key_G;
const int KEY_H = Qt::Key_H;
const int KEY_I = Qt::Key_I;
const int KEY_J = Qt::Key_J;
const int KEY_K = Qt::Key_K;
const int KEY_L = Qt::Key_L;
const int KEY_M = Qt::Key_M;
const int KEY_N = Qt::Key_N;
const int KEY_O = Qt::Key_O;
const int KEY_P = Qt::Key_P;
const int KEY_Q = Qt::Key_Q;
const int KEY_R = Qt::Key_R;
const int KEY_S = Qt::Key_S;
const int KEY_T = Qt::Key_T;
const int KEY_U = Qt::Key_U;
const int KEY_V = Qt::Key_V;
const int KEY_W = Qt::Key_W;
const int KEY_X = Qt::Key_X;
const int KEY_Y = Qt::Key_Y;
const int KEY_Z = Qt::Key_Z;
/*End of definition of keys*/

const char LAPI_PHYNAME_BALL[] = "_$NameOfBall";
const char LAPI_PHYNAME_PRACKET[] = "_$NameOfPRacket";
const char LAPI_PHYNAME_ORACKET[] = "_$NameOfORacket";
const char LAPI_PHYNAME_WALL[] = "_$NameOfWall";

int LEOAPI_FLAGS;
int LEOAPI_FPS;

void * mainWindow;
void * physicalObjects; /*map<char *,vector<lapiPhysicalObject *> >*/
void * lapiObjects;     /*vector<lapiobj >*/
lapiphy * _ball;
lapiphy * _racket[2];
lapiphy * _wall;
lapiobj  _background;
void * newFrameEvents; /*vector<void (*newFrame)() > * */


