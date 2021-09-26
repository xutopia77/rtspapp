
#ifndef YuvSource_H
#define YuvSource_H

#include "mediaSource.h"

#include "mediaCtrl.h"

class YuvSource : public MediaSource{
    private:
    FILE *inYuvFp;
    public:
    int32_t pix_w = 0;
	int32_t pix_h = 0;
    std::string pix_fmt;
    virtual bool start(std::shared_ptr<MediaCtrl> ctrl);
    virtual void sourceProduct();
};


#endif
