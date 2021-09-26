
#ifndef MEDIACTRL_H
#define MEDIACTRL_H

#include "AppCfg.h"

class MediaCtrl
{
private:
    AppCfg* mcfg;
    static void* mediaCtrlRunThread(void* arg);
    void ctrlRun();
public:
    bool isExit = false;
    bool isReady = false;
    bool isStart = false;
    bool isLoop     = true;
    

    MediaCtrl(){mcfg = gpAppCfg;}
    ~MediaCtrl(){};
    bool start();

};




#endif
