
#ifndef RtspSource_H
#define RtspSource_H

#include "mediaSource.h"
#include "ffmpegParse.h"

class RtspSource : public MediaSource{
    private:
        ParseParam param;
    public:
        virtual bool start(std::shared_ptr<MediaCtrl> ctrl);
        virtual void sourceProduct();
};


extern int rtsp_client_task(AppCfg* cfg);

#endif
