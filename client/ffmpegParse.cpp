
/*解码h264并保存为yuv*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ffmpegParse.h"

#include "XAvframe.h"
#include "xlog.h"
using namespace std;

#define INBUF_SIZE 4096

void process_decode_frame(std::shared_ptr<XAvframe> frameRe)
{
	// FILE *f;
	// f = fopen(cfg.outYuvFileName, "a+");
	// fwrite(frameRe->frame, 1, frameRe->width * frameRe->height * (1 + 0.5), f);

	// fclose(f);

	// sendFrameShow(frameRe);

}

void defaultProcessFrameCallBck(AVFrame* frame){

}

static void get_decode_frame(AVCodecContext *dec_ctx,AVFrame *frame, ParseParam* param)
{

#if 0
	char video_decode_buf[1080*1920];
	int i,j,k;
	if (dec_ctx->pix_fmt == AV_PIX_FMT_YUV420P)
	{
		for (i = 0; i < frame->height; i++)
		{
			memcpy(video_decode_buf + frame->width * i,
				   frame->data[0] + frame->linesize[0] * i,
				   frame->width);
		}
		for (j = 0; j < frame->height / 2; j++)
		{
			memcpy(video_decode_buf + frame->width * i + frame->width / 2 * j,
				   frame->data[1] + frame->linesize[1] * j,
				   frame->width / 2);
		}
		for (k = 0; k < frame->height / 2; k++)
		{
			memcpy(video_decode_buf + frame->width * i + frame->width / 2 * j + frame->width / 2 * k,
				   frame->data[2] + frame->linesize[2] * k,
				   frame->width / 2);
		}
	}
	else if (dec_ctx->pix_fmt == AV_PIX_FMT_YUV422P)
	{
		for (i = 0; i < dec_ctx->height; i++)
		{
			memcpy(video_decode_buf + dec_ctx->width * i,
				   frame->data[0] + frame->linesize[0] * i,
				   dec_ctx->width);
		}
		for (j = 0; j < dec_ctx->height; j++)
		{
			memcpy(video_decode_buf + dec_ctx->width * i + dec_ctx->width / 2 * j,
				   frame->data[1] + frame->linesize[1] * j,
				   dec_ctx->width / 2);
		}
		for (k = 0; k < dec_ctx->height; k++)
		{
			memcpy(video_decode_buf + dec_ctx->width * i + dec_ctx->width / 2 * j + dec_ctx->width / 2 * k,
				   frame->data[2] + frame->linesize[2] * k,
				   dec_ctx->width / 2);
		}
	}
	else
	{
		return ;
	}

#endif 

	if(param->processFrameCallBck != NULL){
		param->processFrameCallBck(frame, param);
	}else{
		LOG_WARN("processFrameCallBck null\n");
	}

	// process_decode_frame(frameRe);
}

static void decode(AVCodecContext *dec_ctx, AVFrame *frame, AVPacket *pkt, ParseParam* param)
{
	int ret;

	ret = avcodec_send_packet(dec_ctx, pkt);
	if (ret < 0)
	{
		fprintf(stderr, "Error sending a packet for decoding\n");
		exit(1);
	}

	while (ret >= 0)
	{
		ret = avcodec_receive_frame(dec_ctx, frame);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
			return;
		else if (ret < 0)
		{
			fprintf(stderr, "Error during decoding\n");
			exit(1);
		}

		get_decode_frame(dec_ctx, frame, param);

		// LOG_INFO("ffmpeg decode linesize=%d\n", frame->linesize[0]);
	}
}


//=========================================================================
//api

void ffmpegParseInit(ParseParam* param)
{
		char *filename, *outfilename;
		AVCodec *codec;
		AVCodecParserContext *parser;
		AVCodecContext *c = NULL;
		FILE *f;
		AVFrame *frame;
		uint8_t inbuf[INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
		uint8_t *data;
		size_t data_size;
		int ret;
		AVPacket *pkt;

	pkt = av_packet_alloc();
	if (!pkt)
	{
		printf("pkt == null,exit\n");
		exit(1);
	}

	/* set end of buffer to 0 (this ensures that no overreading happens for damaged MPEG streams) */
	memset(inbuf + INBUF_SIZE, 0, AV_INPUT_BUFFER_PADDING_SIZE);

	/* find the MPEG-1 video decoder */
	// codec = avcodec_find_decoder(AV_CODEC_ID_MPEG1VIDEO);
	codec = avcodec_find_decoder(AV_CODEC_ID_H264);
	
	if (!codec)
	{
		fprintf(stderr, "Codec not found\n");
		exit(1);
	}

	parser = av_parser_init(codec->id);
	if (!parser)
	{
		fprintf(stderr, "parser not found\n");
		exit(1);
	}

	c = avcodec_alloc_context3(codec);
	if (!c)
	{
		fprintf(stderr, "Could not allocate video codec context\n");
		exit(1);
	}

	/* For some codecs, such as msmpeg4 and mpeg4, width and height
       MUST be initialized there because this information is not
       available in the bitstream. */

	/* open it */
	if (avcodec_open2(c, codec, NULL) < 0)
	{
		fprintf(stderr, "Could not open codec\n");
		exit(1);
	}

	frame = av_frame_alloc();
	if (!frame)
	{
		fprintf(stderr, "Could not allocate video frame\n");
		exit(1);
	}

	param->pkt = pkt;
	param->codec = codec;
	param->parser = parser;
	param->c = c;
	param->frame = frame;
}

void ffmpegFree(ParseParam* param)
{
	av_parser_close(param->parser);
	avcodec_free_context(&param->c);
	av_frame_free(&param->frame);
	av_packet_free(&param->pkt);
}

void ffmpegDecode(char* datIn,uint32_t len, ParseParam* param)
{

	uint32_t data_size = len;
	uint8_t *data = (u_int8_t*)datIn;
	int ret;
		while (data_size > 0)
		{
			ret = av_parser_parse2(param->parser, param->c, &param->pkt->data, &param->pkt->size,
								   data, data_size, AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
			if (ret < 0)
			{
				fprintf(stderr, "Error while parsing\n");
				exit(1);
			}
			data += ret;
			data_size -= ret;
			// pkt->stream_index
			if (param->pkt->size)
				decode(param->c, param->frame, param->pkt, param);
			else
			{
				// LOG_RUNLINE;
			}
		}
}


bool getYuvFromAVFrame(AVFrame *frame, char* bufOut){
	if(bufOut == nullptr){
		return false;
	}

	int i,j,k;
	if (frame->format == AV_PIX_FMT_YUV420P)
	{
		for (i = 0; i < frame->height; i++)
		{
			memcpy(bufOut + frame->width * i,
				   frame->data[0] + frame->linesize[0] * i,
				   frame->width);
		}
		for (j = 0; j < frame->height / 2; j++)
		{
			memcpy(bufOut + frame->width * i + frame->width / 2 * j,
				   frame->data[1] + frame->linesize[1] * j,
				   frame->width / 2);
		}
		for (k = 0; k < frame->height / 2; k++)
		{
			memcpy(bufOut + frame->width * i + frame->width / 2 * j + frame->width / 2 * k,
				   frame->data[2] + frame->linesize[2] * k,
				   frame->width / 2);
		}
	}
	else if (frame->format == AV_PIX_FMT_YUV422P)
	{
		for (i = 0; i < frame->height; i++)
		{
			memcpy(bufOut + frame->width * i,
				   frame->data[0] + frame->linesize[0] * i,
				   frame->width);
		}
		for (j = 0; j < frame->height; j++)
		{
			memcpy(bufOut + frame->width * i + frame->width / 2 * j,
				   frame->data[1] + frame->linesize[1] * j,
				   frame->width / 2);
		}
		for (k = 0; k < frame->height; k++)
		{
			memcpy(bufOut + frame->width * i + frame->width / 2 * j + frame->width / 2 * k,
				   frame->data[2] + frame->linesize[2] * k,
				   frame->width / 2);
		}
	}
	else
	{
		return false;
	}
	return true;
}


int main1()
{
	ParseParam param;
	
	const char *filename, *outfilename;

	FILE *f;
	uint8_t inbuf[INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
	uint8_t *data;
	size_t data_size;
	int ret;
	filename = "./test.264";
	outfilename = "./o.dat";
	param.outfilename = (char*)outfilename;
	ffmpegParseInit(&param);

	f = fopen(filename, "rb");
	if (!f){fprintf(stderr, "Could not open %s\n", filename);exit(1);}

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



