
#ifndef MEDIASINK_H
#define MEDIASINK_H
#include "AppCfg.h"
#include "xlog.h"
#include "XAvframe.h"
#include "mediaCtrl.h"

class MediaSink
{
private:
    
public:
    std::shared_ptr<MediaCtrl> mctrl;
    MediaSink(){};
    virtual ~MediaSink(){};
    virtual bool start(std::shared_ptr<MediaCtrl> ctrl) = 0;
    virtual void sinkRun() = 0;
    static void* sinkRunThread(void* arg){
        MediaSink* sink = (MediaSink*)arg;
        if(sink == nullptr){
            fatal("sink null\n");
        }
        sink->sinkRun();
        return nullptr;
    }
    std::shared_ptr<XAvframe> sinkFrame = nullptr;
    virtual void acceptFrame(std::shared_ptr<XAvframe>& frame) = 0;
};




#endif
