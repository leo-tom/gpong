#ifndef SOFLAYER_H
#define SOFLAYER_H

#include <map>
#include "sofobject.h"
#include "pong.h"
#include <QPainter>

class SofLayer
{
public:
    SofLayer();
    ~SofLayer();
    void startRendering(QPainter &painter,const QSize &size,int flag);
    void add(std::string &key,sofObject *obj);
    void add(const char *key,sofObject *obj);
    void remove(sofObject *obj);
    void remove(std::string &str);
    void remove(const char *);
    sofObject * get(const std::string &key){return map[key];}
    sofObject * get(const char *key){std::string str(key);return map[str];}
    std::map<std::string,sofObject *> * getObjects(){return &map;}

    static const int FLAG_CALLED_BY_RENDERING_THREAD = 1 << 0;

private:
    std::map<std::string,sofObject *> map;
};

#endif // SOFLAYER_H
