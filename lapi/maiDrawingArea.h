
#ifndef MAIDRAWINGAREA_H
#define MAIDRAWINGAREA_H

#include <QWidget>
#include <QPainter>
#include <QSizePolicy>
#include "pong.h"
#include "soflayer.h"
#include <mutex>
#include <QKeyEvent>
#include "leoapi.h"

class MaiDrawingArea : public QWidget
{
    Q_OBJECT
public:
    explicit MaiDrawingArea(QWidget *);
    ~MaiDrawingArea();
    std::vector<SofLayer *> & getLayers() {return layers;}
    void mouseDoubleClickEvent(QMouseEvent *eve);
    void mouseReleaseEvent(QMouseEvent *eve);
    void mousePressEvent(QMouseEvent *);
    void mouseEventEvolve(int x,int y,int flag);
    void keyPressEvent(QKeyEvent *eve);
    void keyReleaseEvent(QKeyEvent *eve);
    void setKeyEvent(void (*handler)(int flag,int keyType));
    void removeKeyEvent(void (*handler)(int flag,int keyType));
    void render();
private:
    void paintEvent(QPaintEvent *event);
    void setFlag(int value){flag |= value;}
    void unsetFlag(int value){flag &= ~value;}
    std::vector<SofLayer *> layers;
    int flag;
    int currentryRendering;
    std::vector<void (*)(int flag,int keyType)> keyEvents;
    std::vector<void (*)(int flag,int keyType)> mouseEvents;
    static const int FLAG_RENDERING_FROM_RENDERING_THREAD = (1 << 0);
signals:

public slots:
};

#endif // MAIDRAWINGAREA_H
