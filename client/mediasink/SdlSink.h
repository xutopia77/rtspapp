
#ifndef _SDL_SINK_H
#define _SDL_SINK_H

#include "mediaSink.h"

class SdlSink : public MediaSink
{
private:
    int screen_w ;
    int screen_h ;
    
    unsigned int step = 4;
    unsigned int delayTime = 40;
    bool PAUSE = false;
    //使用sdl创建线程需要使用这种格式的回调函数
    static int sdlSinkRunThread(void* arg){
        MediaSink* sink = (MediaSink*)arg;
        sink->sinkRun();
        return 0;
    }
public:
    SdlSink();
    ~SdlSink();
    virtual bool start(std::shared_ptr<MediaCtrl> ctrl);
    virtual void sinkRun();
    virtual void acceptFrame(std::shared_ptr<XAvframe>& frame);
};


#endif
