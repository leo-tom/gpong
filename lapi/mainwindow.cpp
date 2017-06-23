#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->drawingArea = new MaiDrawingArea(this);
    this->ui->gridLayout->addWidget(this->drawingArea);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::render(){
    this->drawingArea->render();
}
SofLayer * MainWindow::getLayer(int i){
    std::vector<SofLayer *> &vec = this->drawingArea->getLayers();
    if(vec.size() <= i){
        return NULL;
    }
    return vec[i];
}
void MainWindow::setKeyListener(void (*handler)(int, int)){
    drawingArea->setKeyEvent(handler);
}
void MainWindow::removeKeyListener(void (*handler)(int, int)){
    drawingArea->removeKeyEvent(handler);
}
