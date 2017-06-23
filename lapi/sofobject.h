#ifndef SOFOBJECT_H
#define SOFOBJECT_H

#include "pong.h"
#include <QPixmap>
#include <QImage>
#include <QString>
#include "leoapi.h"

using namespace std;
class sofObject
{
public:
    sofObject(const string &str);
    sofObject(QImage *pic);
    sofObject(QImage &pic);

    ~sofObject();

    std::vector<void (*)(lapiobj,int flag)> mouseEvents;

    void setWidth(int w);
    void setHeight(int h);
    int getWidth(){return this->image->width();}
    int getHeight(){return this->image->height();}
    void setSX(int val){this->sx = val;}
    void setSY(int val){this->sy = val;}
    void setSW(int val){this->sw = val;}
    void setSH(int val){this->sh = val;}
    int getSX(){return sx;}
    int getSY(){return sy;}
    int getSW(){return sw;}
    int getSH(){return sh;}
    void setImage(const string &str);
    void setImage(QPixmap *pic);
    void setImage(QImage *pic);
    QImage * getQImage(){return image;}
    QImage * getOriginalQImage(){return originalImage;}
    void setX(int x);
    void setY(int y);
    int getX(){return x;}
    int getY(){return y;}
    void Turn(double radian);
    double getTurn(){return turn;}
    void setSize(double val);
    double getSize(){return size;}

    void deleteOriginalImageOnFinalize(bool b)
        { if(b) flag |= FLAG_DELETE_ORIGINAL_IMAGE_ONFINALIZE;else flag &= ~FLAG_DELETE_ORIGINAL_IMAGE_ONFINALIZE;}
    int isDeleteOnFinallize(){return flag & FLAG_DELETE_ORIGINAL_IMAGE_ONFINALIZE;}
private:
    void init();
    unsigned int flag;
    int x;
    int y;
    int sx;
    int sy;
    int sw;
    int sh;
    double size;
    double turn;/*radian*/
    QImage *image;
    QImage *originalImage;

    static const int FLAG_DELETE_ORIGINAL_IMAGE_ONFINALIZE = 1 << 0;
    static const int FLAG_SIZE_CHAMGED = 1 << 1;
    static const int FLAG_TURN_CHANGED = 1 << 2;
};

#endif // SOFOBJECT_H
