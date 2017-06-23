#include "leoapi.h"
#include "string.h"
#include <vector>
#include <map>

lapiphy* phyInit(lapiobj obj, const char *name){
    lapiphy *ptr = (lapiphy *)calloc(1,sizeof(struct lapiPhysicalObject));
    if(name == NULL || name == nullptr){
        return NULL;
    }
    ptr->Name = strdup(name);
    ptr->objData = obj;
    std::map<const char *,std::vector<lapiPhysicalObject *> > *map = (std::map<const char *,std::vector<lapiPhysicalObject *> > *)physicalObjects;
    (*map)[(char *)name].push_back(ptr);
    return ptr;
}
unsigned int phyGetFlag(lapiphy *phy){
    return phy->flag;
}
void phyMoveVector(lapiphy *phy, double x, double y){
    phy->flag |= FLAGS_PHYSICS_MOVEMENT_VECTOR;
    phy->X = x;
    phy->Y = y;
}
void phyMoveTo(lapiphy *phy, int x, int y, double second){
    phy->flag |= FLAGS_PHYSICS_MOVEMENT_MOVETO;
    phy->X = x;
    phy->Y = y;
    phy->mvRemainingTime = second;
}
void phyTurnToRadian(lapiphy *obj, double radian,double second){
    obj->turn = radian;
    obj->turnRemainingTime = second;
    obj->flag |= FLAGS_PHYSICS_TURN_TO;
}
void phyMoveStop(lapiphy *obj){
    obj->mvRemainingTime = 0.0f;
    obj->X = 0.0f;
    obj->Y = 0.0f;
}

void phyTurnToDegree(lapiphy *obj, double degree,double second){
    phyTurnToRadian(obj,M_DEG2RAD(degree),second);
}
void phyTurnRadian(lapiphy *obj, double radian){
    obj->turn = radian;
    obj->flag |= FLAGS_PHYSICS_TURN;
}
void phyTurnDegree(lapiphy *obj, double degree){
    phyTurnRadian(obj,M_DEG2RAD(degree));
}

void phySetHitListener(lapiphy *obj, int (*whenHit)(lapiPhysicalObject *, lapiPhysicalObject *)){
    obj->whenHit = whenHit;
}
void * phyGetHitListener(lapiphy *obj){
    return (void *)obj->whenHit;
}
void phySetName(lapiphy *obj, const char *name){
    obj->Name = strdup(name);
}
void phyAddTarget(lapiphy *obj, const char *target){
    if(!obj->targets){
        obj->targets = new std::vector<char *>();
    }
    std::vector<char *> *vec = (std::vector<char *> *)obj->targets;
    vec->push_back(strdup(target));
}
int phyGetX(lapiphy *obj){
    return objGetX(obj->objData);
}
int phyGetY(lapiphy *obj){
    return objGetY(obj->objData);
}
int phyGetXCenter(lapiphy *obj){
    return objGetX(obj->objData) + objGetWidth(obj->objData) / 2;
}
int phyGetYCenter(lapiphy *obj){
    return objGetY(obj->objData) + objGetHeight(obj->objData) / 2;
}
int phyGetWidth(lapiphy *obj){
    return objGetWidth(obj->objData);
}
int phyGetHeight(lapiphy *obj){
    return objGetHeight(obj->objData);
}
const char * phyGetName(lapiphy *obj){
    return obj->Name;
}
char ** phyGetTargets(lapiphy *obj){
    std::vector<char *> *vec = (std::vector<char *> *)obj->targets;
    if(vec == NULL)
        return NULL;
    char **ptr = (char **)malloc(vec->size() + 1);
    int i;
    for(i = 0;i < vec->size();i++){
        ptr[i] = strdup(vec->at(i));
    }
    ptr[i] = NULL;
    return ptr;
}
int phyGetTargetsSize(lapiphy *obj){
    std::vector<char *> *vec = (std::vector<char *> *)obj->targets;
    if(vec == NULL)
        return NULL;
    return vec->size();
}
lapiobj phyGetlapiObject(lapiphy *obj){
    return obj->objData;
}
void phyDisableDefaultCollisionBehaviour(lapiphy *obj, bool disable){
    if(disable)
        obj->flag |= FLAGS_PHYSICS_NO_DEFAULT_COLLISION_BEHAVIOUR;
    else
        obj->flag &= ~ FLAGS_PHYSICS_NO_DEFAULT_COLLISION_BEHAVIOUR;
}
void phyDisableCollision(lapiphy *obj, bool disable){
    if(disable)
        obj->flag |= FLAGS_PHYSICS_COLLISION_DISABLED;
    else
        obj->flag &= ~ FLAGS_PHYSICS_COLLISION_DISABLED;
}
void phyDisableMovement(lapiphy *obj, bool disable){
    if(disable)
        obj->flag |= FLAGS_PHYSICS_MOVEMENT_DISABLED;
    else
        obj->flag &= ~ FLAGS_PHYSICS_MOVEMENT_DISABLED;
}
lapiobj phyFinalize(lapiPhysicalObject *obj){
    lapiobj tmp = obj->objData;
    if(obj->targets){
        std::vector<char *> *vec = (std::vector<char *> *)obj->targets;
        for(auto it = vec->begin();it!= vec->end();++it){
            char *str = *it;
            free(str);
        }
        delete vec;
    }
    std::map<char *,std::vector<lapiPhysicalObject *> > *map = (std::map<char *,std::vector<lapiPhysicalObject *> > *)physicalObjects;
    std::vector<lapiPhysicalObject *> &vec = map->at(obj->Name);
    for(auto it = vec.begin();it != vec.end();++it){
        if(*it == obj){
            vec.erase(it);
            break;
        }
    }
    free(obj);
    return tmp;
}
