
#ifndef YUVSINK_H
#define YUVSINK_H
#include "mediaSink.h"
#include "threadsafe_queue.h"

class YuvSink : public MediaSink {
private:
    threadsafe_queue<std::shared_ptr<XAvframe> > frameQueue;
public:
    YuvSink(){};
    ~YuvSink(){};
    virtual bool start(std::shared_ptr<MediaCtrl> ctrl);
    virtual void acceptFrame(std::shared_ptr<XAvframe>& frame);
    virtual void sinkRun();
};



#endif

