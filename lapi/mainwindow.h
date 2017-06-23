#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QResizeEvent>
#include <iostream>
#include "maiDrawingArea.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void render();
    void setKeyListener(void (*handler)(int flag,int keyType));
    void removeKeyListener(void (*handler)(int flag,int keyType));
    void setObjectClickedListener();
    SofLayer * getLayer(int i);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    MaiDrawingArea *drawingArea;
protected:
};

#endif // MAINWINDOW_H
