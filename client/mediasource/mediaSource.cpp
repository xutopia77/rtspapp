#include "mediaSource.h"
#include "ffmpegParse.h"
#include "XAvframe.h"

#include "XAvframe.h"

using namespace std;


bool MediaSource::dispatchFrame(std::shared_ptr<XAvframe>& frame){
    for(auto& x: subscriberList){
        if (x != nullptr){
            x->acceptFrame(frame);
        }
    }
    return true;
}


