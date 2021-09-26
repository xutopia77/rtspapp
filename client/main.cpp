

/*从rtsp服务器接收数据，经过ffmpeg解码后，送入sdl显示
export LD_LIBRARY_PATH=/usr/local/lib
*/

#include "AppCfg.h"

#include "ffmpegParse.h"
#include <string>

#include "unistd.h"
#include <pthread.h>

#include "rtspApp.h"

#include "XAvframe.h"

using namespace std;

extern int testMain();

XAvframePool gFramePool;


extern void initParam(char* cfgFileName);

extern int mainTest(int argc, char const *argv[]);

int main(int argc, char const *argv[]) {

	initParam((char*)"../dat/cfg.txt");

	mainTest(argc, argv);

	RtspApp app;
	app.start(gpAppCfg);
	while (true){
		sleep(1);
	}

	return 0;
}