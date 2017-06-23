#include "lapi/leoapi.h"

#include "lapi/mainwindow.h"
#include <QApplication>
#include <thread>

void renderingThread(){
    /*Assuming that time_t is second*/
    clock_t startTime,currentTime;
    struct timespec sleepTime;
    vector<void (*)() > *newFrameVec = (vector<void (*)() > *) newFrameEvents;
    long tmp;
    double dtmp;
    unsigned long nanoSleepTime; /*millisecond per one frame*/
    sleepTime.tv_sec = 0;
    while(! (LEOAPI_FLAGS & LEOFLAGS_RENDERING_THREAD_KILL) ){
        /*main loop*/
\
        nanoSleepTime = 1000000000 / LEOAPI_FPS;
        if( (startTime = clock()) < 0) goto timeError;
        for(std::vector<void (*)() >::iterator it = newFrameVec->begin();it!=newFrameVec->end();++it){
            void (*eventHandler)() = *it;
            if(eventHandler != NULL){
                eventHandler();
            }
        }
        _doPhysics(nanoSleepTime);
        ((MainWindow *) mainWindow)->render();
        if( (currentTime = clock()) < 0) goto timeError;
        tmp = currentTime - startTime;
        dtmp = ((double)tmp / (double)CLOCKS_PER_SEC) / 1000000.0f;
        tmp = (long)dtmp;
        /*tmp now have how long nanosecond have passed*/
        tmp = nanoSleepTime - tmp;
        printf("nanoSleeping:%ld\n",tmp);
        if(tmp > 0){
            /*if there is few time left, sleep for a moment*/
            sleepTime.tv_nsec = tmp;
            nanosleep(&sleepTime,NULL);
        }
    }
    return;
    timeError:
            std::cerr << "Failed to fetch current time" << std::endl;
            exit(1);
}


int main(int argc,char *argv[]){
    LEOAPI_FLAGS = 0;
    QApplication a(argc, argv);
    MainWindow w;
    mainWindow = (void *)&w;
    w.show();
    _initLeoApi();
    std::thread thread(renderingThread);
    int returnValue = a.exec();
    LEOAPI_FLAGS |= LEOFLAGS_RENDERING_THREAD_KILL;
    thread.join();
    return returnValue;
}
