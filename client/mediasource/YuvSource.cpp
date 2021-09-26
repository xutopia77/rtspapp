
#include "YuvSource.h"
#include "ffmpegParse.h"
#include "xlog.h"

using namespace std;

bool YuvSource::start(shared_ptr<MediaCtrl> ctrl){
    int re=-1;
    pthread_t pid;
    mctrl = ctrl;
    re = pthread_create(&pid,NULL,sourceThread, this);
    if(re != 0) 
        return false;
    return true;
}

char *getFilename(char *p)
{
    int x = strlen(p);
    char ch = '/';
    char* a= strrchr(p,ch);
    if(a == NULL){
        return p;
    }
    else{
        return strrchr(p,ch) + 1;
    }
}

void  YuvSource::sourceProduct(){
    AppCfg *appcfg = gpAppCfg;
    
    int w,h;
    char strtmp[64];
    int ire = 0;

    char* fileName = getFilename((char*)gpAppCfg->inYuvFileName.c_str());
    if( (ire=sscanf(fileName, "%7s_%dx%d.yuv", strtmp, &w,&h)) != 3){
        fatal("get yuv format err, %d, %s\n", ire, fileName);
    }
    pix_fmt = strtmp;
    pix_w = w;
    pix_h = h;
    uint32_t len = pix_w * pix_h * 3 / 2;
    LOG_DBG("source yuv file format: %s, %d, %d\n", pix_fmt.c_str(), pix_w, pix_h);

    inYuvFp = fopen(gpAppCfg->inYuvFileName.c_str(), "rb+");
    if (inYuvFp == nullptr){
        fatal("open source file failed\n");
    } 
    mctrl->isReady = true;

    while (true){
        if(mctrl->isStart)
            break;
        usleep(100);
    }
    LOG_INFO("source yuv product start\n");
    while (!mctrl->isExit){
        shared_ptr<XAvframe> frame = gFramePool.getFreeFrame(len);
        ire = fread(frame->frame, 1, len, inYuvFp);
        
        if (ire == 0){
            if (mctrl->isLoop){
                // LOG_DBG("source yuv loop\n");
                fseek(inYuvFp, 0, SEEK_SET);
                ire = fread(frame->frame, 1, len, inYuvFp);            
            }else{
                LOG_INFO("read over source exit\n");
                break;
            }
        }

        if (ire == len){
            frame->width = pix_w;
            frame->height = pix_h;
            frame->pix_fmt = pix_fmt;
            dispatchFrame(frame);
        }
        else{
            LOG_ERR("arrive to the end or read failed len = %d\n", ire);
        }
        usleep(rate*1000);
    }

    LOG_OFTEN("yuvSourceRun exit\n");
}

