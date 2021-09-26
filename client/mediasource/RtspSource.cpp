
#include "RtspSource.h"
#include "ffmpegParse.h"

using namespace std;


static void processFrameCallbck(AVFrame* frame, ParseParam* param){
	shared_ptr<XAvframe> frameRe = gFramePool.getFreeFrame(frame->width * frame->height * (1 + 0.5));
	if(frameRe == nullptr){
		fatal("get free frame err\n");
	}
	if(!getYuvFromAVFrame(frame, frameRe->frame)){
        return ;
    }

    frameRe->height = frame->height;
    frameRe->width = frame->width;
	if(frame->format == AV_PIX_FMT_YUV420P){
		frameRe->pix_fmt = "yuv420p";
	}else{
		frameRe->pix_fmt = "unknow";
	}

    if(1){
        FILE *f;
        // f = fopen(param->outfilename, "a+");
        f = fopen("../dat/out/rxh264.h264", "a+");
        fwrite(frameRe->frame, 1, frame->width * frame->height * (1 + 0.5), f);
        fclose(f);
    }

    if(1){
        RtspSource* source = (RtspSource*)param->privDat;
        if(source == nullptr){
            LOG_ERR("source null err\n");
            return;
        }
        // LOG_DBG("start dispatchFrame\n");
        source->dispatchFrame(frameRe);
    }
	//AVPixelFormat
}

extern void processRtspCliH264Default(char* dat,uint32_t len, AppCfg* cfg);

bool RtspSource::start(shared_ptr<MediaCtrl> ctrl){
    mctrl = ctrl;
    int re=-1;
    pthread_t pid;
    re = pthread_create(&pid,NULL,sourceThread, this);
    if(re != 0) 
        return false;
    return true;
}

void  RtspSource::sourceProduct(){
    //1,初始化
    ParseParam param;
    param.processFrameCallBck = processFrameCallbck;
    param.privDat = this;

    AppCfg* cfg = gpAppCfg;
    cfg->processRxH264Callbck = processRtspCliH264Default;
    cfg->privDat[0] = &param;

    //2,通知资源已经准备好//[todo]是否需要这样做？
    mctrl->isReady = true;

    //3，等待开始
    while (true){
        if(mctrl->isStart)
            break;
        usleep(100);
    }

    //4开始
    LOG_DBG("rtsp client start\n");
    rtsp_client_task(cfg);

	/* flush the decoder */
    //	decode(param.c, param.frame, NULL, outfilename);


    LOG_OFTEN("yuvSourceRun exit\n");
}

