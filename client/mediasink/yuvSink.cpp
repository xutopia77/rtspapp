
#include "yuvSink.h"
#include <pthread.h>

#include "xlog.h"

#include "XAvframe.h"
#include "ffmpegParse.h"

using namespace std;

bool YuvSink::start(shared_ptr<MediaCtrl> ctrl){
    int re=-1;
    mctrl = ctrl;
    pthread_t pid;
    re = pthread_create(&pid,NULL,sinkRunThread, this);
    if(re != 0) return false;   

    return true;
}


void YuvSink::acceptFrame(std::shared_ptr<XAvframe>& frame){
    frameQueue.push(frame);
}

void YuvSink::sinkRun(){
    LOG_INFO("sink yuv start\n");
    FILE *outYuvFp = NULL;
    
    uint32_t frameCnt = 0;
    while(!mctrl->isExit){
        shared_ptr<XAvframe> frame = *(frameQueue.wait_and_pop());
        if(frame != nullptr){
            // LOG_DBG("get yuv frame %d\n", ++frameCnt);
            if(NULL == outYuvFp){
                if("yuv420p" != frame->pix_fmt){
                    fatal("unknow pix fmt %s\n", frame->pix_fmt.c_str());
                }
                string outfileName = gpAppCfg->outYuvFileName + frame->pix_fmt+"_"+to_string(frame->width) + "x"+to_string(frame->height)+".yuv";
                outYuvFp = fopen(gpAppCfg->outYuvFileName.c_str(), "wb+");
                if(outYuvFp == NULL){
                    fatal("outYuvFp == NULL\n");
                }
            }

            fwrite((char*)frame->frame, frame->height*frame->width * 3 / 2,1, outYuvFp);
        }
        usleep(100);
    }
    fclose(outYuvFp);
}