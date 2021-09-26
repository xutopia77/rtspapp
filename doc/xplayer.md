目前有两种播放方式

1，播放本地文件

2，播放rtsp服务器的文件

3，把接收到的H26文件进行保存

设计模式：

sdlshow中只管播放，播放的数据源有两种：

一种是sdl主动向播放源取帧，在sdlCtrl线程中按时取的，例如从文件中播放yuv文件，这种就是在sdlCtrl线程中个，以固定周期调用getYuvFrame函数，获取yuvFrame，送入sdl播放。

另外一种就是由播放源主动推帧，播放源主动把帧推送到sdl中进行播放。



void sendFrameShow(frameResultTypedef* frameRe)，调用这个函数就可以把数据帧送入sdl中进行显示。





