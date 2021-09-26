
#ifndef _AVFRAME_H
#define _AVFRAME_H

#include <stdint.h>
#include <memory>
#include <queue>


#define SHITMEM 1

class XMempool
{
private:
    size_t defaultCache = 1920*1080;
    size_t defaultCount=25;

    
    
    // std::queue<std::shared_ptr<XMem> > memQueueFill;
public:
    struct XMem{
        int refCnt=0;//引用计数
        char* frame;
    };
    std::queue<std::shared_ptr<XMem> > memQueueFree;
    XMempool();
    ~XMempool();
    std::shared_ptr<XMem> getFreeMem(){
        if(memQueueFree.empty()){
            return nullptr;
        }
        std::shared_ptr<XMem> re = memQueueFree.front();
        memQueueFree.pop();
        return re;
    }
};





//应该避免avframe的直接赋值，把构造函数隐藏掉
class XAvframe
{
private:
    uint32_t _cacheBytes = 1920*1080; 
public:
    XAvframe();
    ~XAvframe();
    char* frame;
	std::string pix_fmt;
	uint32_t width,height;
    bool isUsed = false;

};

class XAvframePool
{
private:
    XMempool mempool;
    int _cacheNum = 25;
    std::queue<std::shared_ptr<XAvframe> > frameQueueFree;
    std::queue<std::shared_ptr<XAvframe> > frameQueueFill;
public:
    XAvframePool(){
        #ifndef SHITMEM
        for(int i=0;i<_cacheNum;i++){
            std::shared_ptr<XAvframe> frame = std::make_shared<XAvframe>();
            frameQueueFree.push(frame);
        }
        #endif
    };
    ~XAvframePool(){};
    std::shared_ptr<XAvframe> getFrame();             //获取存在的帧
    std::shared_ptr<XAvframe> createNewFrame();
    std::shared_ptr<XAvframe> getFreeFrame(size_t len);           //从内存池中申请空闲帧
    bool push_back(std::shared_ptr<XAvframe>& frame); //追加帧

    size_t getFillFrameCount(){return frameQueueFill.size();}
};



extern XAvframePool gFramePool;

#endif
