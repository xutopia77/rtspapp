
#ifndef MediaSource_H
#define MediaSource_H
#include "AppCfg.h"
#include "xlog.h"

#include "XAvframe.h"
#include "mediaSink.h"
#include <list>

#include "mediaCtrl.h"

class MediaSource
{
private:
    
protected:
    std::shared_ptr<MediaCtrl> mctrl;
public:
std::list<std::shared_ptr<MediaSink> > subscriberList;
    MediaSource(){};
    virtual ~MediaSource(){};
    
    uint32_t rate=1000/25;//25帧 40ms
    virtual bool start(std::shared_ptr<MediaCtrl> ctrl) = 0;
    virtual void sourceProduct() = 0;
    static void* sourceThread(void* arg){
        MediaSource* source = (MediaSource*)arg;
        source->sourceProduct();
        return nullptr;
    };
    bool dispatchFrame(std::shared_ptr<XAvframe>& frame);
};


#endif
