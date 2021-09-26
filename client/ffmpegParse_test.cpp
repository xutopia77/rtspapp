
/*解码h264并保存为yuv*/

#include "XAvframe.h"
#include "ffmpegParse.h"
#include "xlog.h"
using namespace std;

#define INBUF_SIZE 4096


static void processFrameCallbck(AVFrame* frame, ParseParam* param){
	shared_ptr<XAvframe> frameRe = gFramePool.getFreeFrame(frame->width * frame->height * (1 + 0.5));
	if(frameRe == nullptr){
		fatal("get free frame err\n");
	}
	getYuvFromAVFrame(frame, frameRe->frame);
    frameRe->height = frame->height;
    frameRe->width = frame->width;
	if(frame->format == AV_PIX_FMT_YUV420P){
		frameRe->pix_fmt = "yuv420p";
	}else{
		frameRe->pix_fmt = "unknow";
	}

	FILE *f;
	f = fopen(param->outfilename, "a+");
	fwrite(frameRe->frame, 1, frame->width * frame->height * (1 + 0.5), f);

	fclose(f);
}


int ffmpegParseTest(int argc, char const *argv[]){
	
	static ParseParam param;

	const char *filename;

	FILE *f;
	uint8_t inbuf[INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
	uint8_t *data;
	size_t data_size;
	int ret;
	filename = "./../dat/s640x360.264";
	param.outfilename = (char*)"../dat/out/o420p_640x360.yuv";
	param.processFrameCallBck = processFrameCallbck;
	ffmpegParseInit(&param);

	f = fopen(filename, "rb");
	if (!f){
		fprintf(stderr, "Could not open %s\n", filename);
		exit(1);
	}

	while (!feof(f))
	{
		/* read raw data from the input file */
		data_size = fread(inbuf, 1, INBUF_SIZE, f);
		if (!data_size)
			break;
		/* use the parser to split the data into frames */
		data = inbuf;
		
		ffmpegDecode((char*)data,data_size,&param);
	}

	/* flush the decoder */
//	decode(param.c, param.frame, NULL, outfilename);

	fclose(f);

	ffmpegFree(&param);

	return 0;
}



