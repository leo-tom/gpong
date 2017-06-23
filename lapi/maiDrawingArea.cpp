#include "maiDrawingArea.h"
#include <stdlib.h>

MaiDrawingArea::MaiDrawingArea(QWidget *parent) : QWidget(parent)
{
    this->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
    this->setMinimumSize(640,360);
    layers.push_back(new SofLayer);
    layers.push_back(new SofLayer);
    layers.push_back(new SofLayer);
}
MaiDrawingArea::~MaiDrawingArea(){
    for(auto it = layers.begin();it!=layers.end();++it){
        SofLayer *layer = *it;
        delete layer;
    }
}

void MaiDrawingArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    int flag = 0;
    SofLayer *layer = layers.at(0);
    QSize size;
    size.setHeight(this->height());
    size.setWidth(this->width());
    for(auto it = layers.begin() ; it != layers.end();++it){
        SofLayer *layer = *it;
        layer->startRendering(painter,size,flag);
    }
}
void MaiDrawingArea::render()
{
    if(this->isVisible() && this->isActiveWindow()){
        setFlag(FLAG_RENDERING_FROM_RENDERING_THREAD);
        this->update();
    }
}

void MaiDrawingArea::mouseEventEvolve(int x, int y, int flag){
    for(auto it = layers.rbegin();it != layers.rend();++it){
        SofLayer *layer = *it;
        std::map<std::string,sofObject *> *map = layer->getObjects();
        for(auto mapIt = map->begin();mapIt != map->end();++mapIt){
            sofObject *obj = mapIt->second;
            if(obj->getX() <= x && obj->getY() <= y && obj->getX() + obj->getWidth() >= x && obj->getY() + obj->getHeight() >= y){
                /*clicked*/
                for(auto itEvent = obj->mouseEvents.begin();itEvent != obj->mouseEvents.end();++itEvent){
                    void (*handler)(lapiobj,int flag) = *itEvent;
                    handler(obj,flag);
                }
            }
        }
    }
}

void MaiDrawingArea::mousePressEvent(QMouseEvent *event){
    mouseEventEvolve(event->x(),event->y(),MOUSE_PRESSED);
}
void MaiDrawingArea::mouseDoubleClickEvent(QMouseEvent *event)
{
    mouseEventEvolve(event->x(),event->y(),MOUSE_DOUBLE_CLICKED);
}
void MaiDrawingArea::mouseReleaseEvent(QMouseEvent *event){
    mouseEventEvolve(event->x(),event->y(),MOUSE_RELEASED);
}
void MaiDrawingArea::keyPressEvent(QKeyEvent *eve){
    for(auto it = keyEvents.begin();it!= keyEvents.end();++it){
        void (*func)(int flag,int keyType) = *it;
        if(func!=nullptr){
            func(KEY_PRESSED,eve->key());
        }
    }
}
void MaiDrawingArea::keyReleaseEvent(QKeyEvent *eve){
    for(auto it = keyEvents.begin();it!=keyEvents.end();++it){
        void (*func)(int flag,int keyType) = *it;
        if(func!=nullptr){
            func(KEY_RELEASED,eve->key());
        }
    }
}

void MaiDrawingArea::setKeyEvent(void (*handler)(int, int)){
    keyEvents.push_back(handler);
}
void MaiDrawingArea::removeKeyEvent(void (*handler)(int, int)){
    for(auto it = keyEvents.begin();it != keyEvents.end();++it){
        void (*func)(int, int) = *it;
        if(func == handler){
            keyEvents.erase(it);
            return;
        }
    }
}



