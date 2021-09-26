
#include "H264Source.h"
#include "ffmpegParse.h"

using namespace std;


static void processFrameCallbck(AVFrame* frame, ParseParam* param){
	shared_ptr<XAvframe> frameRe = gFramePool.getFreeFrame(frame->width * frame->height * (1 + 0.5));
	if(frameRe == nullptr){
		fatal("get free frame err\n");
	}
	getYuvFromAVFrame(frame, frameRe->frame);
    frameRe->height = frame->height;
    frameRe->width = frame->width;
    if(AV_PIX_FMT_YUV420P == frame->format)
        frameRe->pix_fmt = "yuv420p";
    else
        frameRe->pix_fmt = "unknow";

    if(0){
        FILE *f;
        f = fopen(param->outfilename, "a+");
        fwrite(frameRe->frame, 1, frame->width * frame->height * (1 + 0.5), f);
        fclose(f);
    }

    if(1){
        H264Source* source = (H264Source*)param->privDat;
        if(source == nullptr){
            return;
        }
        source->dispatchFrame(frameRe);
    }
	//AVPixelFormat
	//LOG_INFO("width[%d, %d], pix_fmt %d\n", frame->width, frame->height, frame->format);
}


bool H264Source::start(shared_ptr<MediaCtrl> ctrl){
    int re=-1;
    mctrl = ctrl;
    pthread_t pid;
    re = pthread_create(&pid,NULL,sourceThread, this);
    if(re != 0) 
        return false;
    return true;
}

void  H264Source::sourceProduct(){

    LOG_INFO("source h264 start\n");

	#define INBUF_SIZE 4096
	
	const char *filename;

	FILE *f;
	uint8_t inbuf[INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
	uint8_t *data;
	size_t data_size;
	int ret;
	filename = "./../dat/s640x360.264";
	param.outfilename = (char*)"../dat/out/o420p_640x360.yuv";
	param.processFrameCallBck = processFrameCallbck;
    param.privDat = this;
	ffmpegParseInit(&param);

	f = fopen(filename, "rb");
	if (!f){
		fprintf(stderr, "Could not open %s\n", filename);
		exit(1);
	}
    //通知资源已经准备好//[todo]是否需要这样做？
    mctrl->isReady = true;

    while (true){
        if(mctrl->isStart)
            break;
        usleep(100);
    }

    while (!mctrl->isExit){
        data_size = fread(inbuf, 1, INBUF_SIZE, f);
        if(data_size == 0){
            //重新开始？
        }
		data = inbuf;
		ffmpegDecode((char*)data, data_size, &param);
        usleep(1000*40);
    }

	/* flush the decoder */
    //	decode(param.c, param.frame, NULL, outfilename);

	fclose(f);
	ffmpegFree(&param);

    LOG_OFTEN("yuvSourceRun exit\n");
}

