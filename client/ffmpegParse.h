
#ifndef _FFMPEGPARSE_H
#define _FFMPEGPARSE_H

#include "AppCfg.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "libavcodec/avcodec.h"

#ifdef __cplusplus
}
#endif


struct ParseParam{
	AVCodecParserContext *parser;
	AVPacket *pkt;
	AVCodec *codec;
	AVFrame *frame;
	AVCodecContext *c;
	char *outfilename;
	void (*processFrameCallBck)(AVFrame *frame, ParseParam* param) = NULL;
	void* privDat;//这个就是source类本身
};


void ffmpegParseInit(ParseParam* param);
void ffmpegFree(ParseParam* param);
void ffmpegDecode(char* datIn,uint32_t len, ParseParam* param);
bool getYuvFromAVFrame(AVFrame *frame, char* bufOut);

int main1();

#endif
