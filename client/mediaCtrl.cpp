
#include "mediaCtrl.h"
#include "AppCfg.h"


#include <time.h>
#include <pthread.h>

#include "xlog.h"

using namespace std;

void MediaCtrl::ctrlRun(){
    while (true){
        usleep(1000);
        if(isReady)  { 
            isStart = true;
            LOG_INFO("ctrl start\n");
            break;
        }
        // LOG_OFTEN("ctrl heart\n");
    }
    while (true){
        usleep(1000*1000);
        // LOG_OFTEN("ctrl heart\n");
    }
}

void* MediaCtrl::mediaCtrlRunThread(void* arg){
    MediaCtrl* ctrl = (MediaCtrl*)arg;
    if(nullptr == ctrl) fatal("ullptr == ctrl");
    
    ctrl->ctrlRun();
    
    return NULL;
}

bool MediaCtrl::start(){
    int re=-1;

    //运行控制线程
    pthread_t pid;
    re = pthread_create(&pid,NULL,mediaCtrlRunThread,this);
    if(re != 0) fatal("create ctrl thread err\n");
    return true;
}


