
#include "XAvframe.h"

#include "xlog.h"

using namespace std;

XMempool::XMempool(){
    for(int i=0;i<defaultCount;i++){

    }
}

XMempool::~XMempool()
{
}

//===============================================================================
XAvframe::XAvframe(){
    frame = new char[1920*1080];
    // LOG_DBG("create avframe\n");
};

XAvframe::~XAvframe(){
    // LOG_DBG("free avframe\n");
    delete frame;
    frame = nullptr;
};


//===============================================================================
std::shared_ptr<XAvframe> XAvframePool::createNewFrame(){
    if (frameQueueFree.empty()){
        return nullptr;
    }

    std::shared_ptr<XAvframe> re = frameQueueFree.front();
    frameQueueFree.pop();
    return re;
}

std::shared_ptr<XAvframe> XAvframePool::getFreeFrame(size_t len){
    //[todo] 应该根据len来申请内存
    shared_ptr<XAvframe> frame = make_shared<XAvframe>();
    return frame;
}

bool XAvframePool::push_back(std::shared_ptr<XAvframe>& frame){
    frameQueueFill.push(frame);
    return true;
}

std::shared_ptr<XAvframe> XAvframePool::getFrame(){
    if(frameQueueFill.empty())
        return nullptr;

    shared_ptr<XAvframe> frame = frameQueueFill.front();
    frameQueueFill.pop();
    return frame;
}



