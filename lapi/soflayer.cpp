#include "soflayer.h"

SofLayer::SofLayer()
{

}
SofLayer::~SofLayer(){
    for(auto it = map.begin();it != map.end();++it){
        sofObject *obj = (*it).second;
        if(obj != nullptr)
            delete obj;
    }
    map.clear();
}

void SofLayer::remove(sofObject *obj){
    for(auto it = map.begin();it != map.end();++it){
        sofObject *element = (*it).second;
        if(element == obj){
            map.erase((*it).first);
            // return;
            /*one layer may contain same object with different key*/
        }
    }
}
void SofLayer::remove(std::string &key){
    map.erase(key);
}
void SofLayer::remove(const char *ptr){
    std::string str(ptr);
    remove(str);
}

void SofLayer::startRendering(QPainter &painter,const QSize& size,int flag){
    int sx,sy,sw,sh;
    for(auto it = map.begin() ; it != map.end();++it){
        sofObject *obj = (*it).second;
        sx = obj->getSX();
        sy = obj->getSY();
        sw = obj->getSW();
        sh = obj->getSH();

        painter.drawImage(obj->getX()
                          ,obj->getY()
                          ,*(obj->getQImage())
                          ,sx,sy
                          ,sw,sh,Qt::ImageConversionFlag::AutoColor);
    }
}

void SofLayer::add(std::string &key,sofObject *obj)
{
    map[key] = obj;
}
void SofLayer::add(const char *key, sofObject *obj){
    std::string str(key);
    map[key] = obj;
}
