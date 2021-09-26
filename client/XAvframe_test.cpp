
#include "XAvframe.h"
#include "xlog.h"

using namespace std;

static void testCreateFrame(){
    shared_ptr<XAvframe> frame = gFramePool.getFreeFrame(1920*1080);
    if(frame == nullptr){
        fatal("create err\n");
    }
    char arr[100];
    for (size_t i = 0; i < 100; i++)
    {           
        frame->frame[i] = i;
    }
    for (size_t i = 0; i < 100; i++)
    {           
        LOG_INFO("%d,", frame->frame[i]);
    }
    


}



int XAvframeTest(int argc, char const *argv[]){

    testCreateFrame();


    return 0;
}

