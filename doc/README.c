
支持两种播放模式，打开本地的yuv文件进行播放，接收rtsp客户端的数据进行播放

API介绍
sdlshow.cpp
    把数据送进sdl进行显示
    void sendFrameShow(char* buf,uint32_t len,AppCfg *cfg)





sdl显示线程设置
    sdlRunlThread
        在这个线程中等待sdl事件
        EVENT_FREASH： 这个是进行画面的显示，显示是会从sdl的缓存中得到一个帧进行显示




设计流程

开启控制线程

创建sink
开启sink

创建source
添加订阅
启动source


