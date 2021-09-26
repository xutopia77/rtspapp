
#ifndef H264Source_H
#define H264Source_H

#include "mediaSource.h"
#include "ffmpegParse.h"

class H264Source : public MediaSource{
    private:
        ParseParam param;
    public:
        virtual bool start(std::shared_ptr<MediaCtrl> ctrl);
        virtual void sourceProduct();
};


#endif
