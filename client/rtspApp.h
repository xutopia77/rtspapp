
#ifndef RTSPAPP_H
#define  RTSPAPP_H
#include "mediaSource.h"
#include "mediaCtrl.h"

#include "mediaSink.h"

#include "AppCfg.h"
#include <list>

class RtspApp
{
private:
    std::shared_ptr<MediaCtrl>      mctrl;
    std::list<std::shared_ptr<MediaSink> >      msinkList;
    std::shared_ptr<MediaSource>    msource;
    AppCfg*                  mcfg;

    bool createSource();
    bool createSink();

public:
    RtspApp(){};
    ~RtspApp(){};
    void start(AppCfg* cfg);


    
};





#endif
