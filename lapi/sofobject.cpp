#include "sofobject.h"
void sofObject::init(){
    this->x = 0;
    this->y = 0;
    this->turn = 0.0f;
    this->flag = 0;
    this->size = 0.0f;
}

sofObject::sofObject(const string &str)
{
    QString qstr = str.c_str();
    if(str.empty()){
        this->originalImage = this->image = nullptr;
    }else{
        this->image = this->originalImage = new QImage(qstr,nullptr);
    }
    init();
    this->deleteOriginalImageOnFinalize(true);
}
sofObject::sofObject(QImage *pic){
    init();
    this->originalImage = this->image = pic;
}
sofObject::sofObject(QImage &pic){
    init();
    deleteOriginalImageOnFinalize(true);
    this->image = this->originalImage = new QImage(pic);
}

sofObject::~sofObject(){
    if(isDeleteOnFinallize()){
        if(this->originalImage)
            delete this->originalImage;
    }
    if(this->image)
        delete this->image;
}

void sofObject::setImage(const string &str){
    QString qstr = str.c_str();
    this->setImage(new QImage(qstr,nullptr));
}
void sofObject::setImage(QImage *pic){
    if(this->image != nullptr && this->image != this->originalImage)
        delete this->image;
    this->image = pic;
}
void sofObject::setImage(QPixmap *pic){
    if(this->image != nullptr && this->image != this->originalImage)
        delete this->image;
    this->image = new QImage(pic->toImage());
}
void sofObject::setX(int x){
    this->x = x;
}
void sofObject::setY(int y){
    this->y = y;
}
void sofObject::Turn(double radian){
    QTransform transform;
    transform.rotateRadians(radian);
    this->turn = radian;
    this->setImage(new QImage(this->originalImage->transformed(transform)));
}
void sofObject::setSize(double val){
    this->setImage(new QImage(this->originalImage->scaled(
                                  this->getWidth() * val,
                                  this->getHeight() * val,
                                  Qt::KeepAspectRatio,Qt::FastTransformation)));
    this->sh *= val;
    this->sw *= val;
    this->sx *= val;
    this->sy *= val;
    this->size = val;
}

void sofObject::setWidth(int w){
    if(w == this->getWidth())
        return;
    this->setImage(new QImage(this->originalImage->scaledToWidth(w,Qt::FastTransformation)));
}
void sofObject::setHeight(int h){
    if(this->getHeight() == h)
        return;
    this->setImage(new QImage(this->originalImage->scaledToHeight(h,Qt::FastTransformation)));
}

