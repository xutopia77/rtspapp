
#include "RtspSource.h"
#include "ffmpegParse.h"
#include "AppCfg.h"

using namespace std;



int RtspSourceTest(int argc, char const *argv[]){
    LOG_INFO("rtsp source test\n");

    if(0){

    
    ParseParam param;
    AppCfg* cfg = gpAppCfg;

    cfg->privDat[0] = &param;
    rtsp_client_task(cfg);
    LOG_INFO("rtsp source stop111\n");
}

if(1){

    RtspSource source;
    // source->mctrl->isStart = true;
    source.start(nullptr);

     LOG_INFO("rtsp source stop\n");
    while (1)
    {
        sleep(1);
    }
    
}


    return 0;
}
