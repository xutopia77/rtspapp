

/*从rtsp服务器接收数据，经过ffmpeg解码后，送入sdl显示
export LD_LIBRARY_PATH=/usr/local/lib
*/

#include <time.h>
#include <stdio.h>
#include <unistd.h>

extern void runSrv();
extern void runClient();

int main(int argc, const char** argv) {

	if(argc != 2){
		printf("argv err\n");
	}

	if(*argv[1]=='3'){
		runSrv();
		sleep(1);
	}

	if(*argv[1]=='1'){
		runClient();
		sleep(1);
	}


	if(*argv[1]=='2'){
		runSrv();
		sleep(1);
		runClient();
		sleep(1);
	}

	while(1);

}