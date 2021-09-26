
#ifndef _GLOBAL_H
#define _GLOBAL_H

#ifdef SDL_SHOW
#include <SDL2/SDL.h>
#include <SDL2/SDL_test_font.h>
#endif
#include <stdint.h>
#include <stdlib.h>

#include "stdio.h"
#include <unistd.h> 
#include <vector>
#include <queue>
#include <assert.h>
#include <string>

#define RXD_FILE_NAME	           	(char*)"../dat/test.264"
#define TXD_FFMPEG_YUV_FILE_NAME    (char*)"../dat/yuv420p_640x360.yuv"
#define RXD_FFMPEG_YUV_FILE_NAME    (char*)"../dat/out/rxyuv420p_640x360.yuv"
#define ORIGINAL_URL	            (char*)"rtsp://127.0.0.1:8554/../dat/s640x360.264"

#define AFTER_FRAME_ORIGINAL	0	//原始的
#define AFTER_FRAME_FILE		0	//可以写文件的
#define AFTER_FRAME_FFMPEG		1	//ffmpg处理的

#define VALNAME(x)	#x






typedef struct appCfg_s
{
	//cfg
	/*
	1:本地文件播放，2:rtsp接收到的文件进行播放
	3，本地h264文件播放
	*/
	int sourceType = 1;
	int sinkFileEnable = 0;//是否开通sink文件的保存 1，开通，0不开通
	int sinkSdlEnable = 1;
	//本地yuv文件播放时的文件名称
	std::string inYuvFileName = TXD_FFMPEG_YUV_FILE_NAME;
	
	std::string outYuvFileName = RXD_FFMPEG_YUV_FILE_NAME;
	std::string rtspUrl = ORIGINAL_URL;
	std::string rxdH264FileName = "../dat/out/rxh264.h264";
	bool isSaveRxH264 = false;
	void *privDat[8];//默认有8个私有数据的指针，这里的8不是固定的，有可能会被扩展，这些数据不必保存在配置文件中，他只是程序上下文而已
	void (*processRxH264Callbck)(char* dat,uint32_t len, struct appCfg_s* cfg) = NULL;
}AppCfg;

extern "C" AppCfg* gpAppCfg;

#endif // !
