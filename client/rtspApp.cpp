
#include "rtspApp.h"

#include <pthread.h>
#include <stdlib.h>

#include "H264Source.h"
#include "RtspSource.h"
#include "YuvSource.h"

#include "yuvSink.h"
#include "SdlSink.h"

using namespace std;

void RtspApp::start(AppCfg* cfg)
{
    mcfg = cfg;

    //==========
    mctrl = make_shared<MediaCtrl>();

    if(!mctrl->start()) {
        fatal("pMediaCtrl->start() err\n");
    }
     
    //==========
    createSink();
    for(auto& x: msinkList){
        if(!x->start(mctrl)){
            fatal("msink->start err\n");
        }
    }
    

    //==========
    createSource();
    //添加订阅
    for(auto& x: msinkList){
        msource->subscriberList.push_back(x);
    }
    if(!msource->start(mctrl)){
        fatal("msink->start err\n");
    }
}

bool RtspApp::createSink(){
    if(mcfg->sinkFileEnable){
        msinkList.push_back(make_shared<YuvSink>());
    }
    if(mcfg->sinkSdlEnable){
        msinkList.push_back(make_shared<SdlSink>());
    }
    return true;
}

bool RtspApp::createSource(){
    //initSource
	pthread_t pid;
	int re=-1;
	switch (mcfg->sourceType){
	case 1:
        msource = make_shared<YuvSource>();
		break;
	case 2:
		msource = make_shared<RtspSource>();	
		break;
    case 3:
        msource = make_shared<H264Source>();
		break;
	default:
		LOG_ERR("sourceType[%d] err\n exit\n",mcfg->sourceType);
		exit(-1);
		break;
	}
    return true;
}



