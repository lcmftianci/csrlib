//打开  

#include "Common.h"

#ifdef MYDECODER

#ifndef AUDIO_DECODER

#define __STDC_FORMAT_MACROS  
#ifdef _CPPRTTI   
extern "C"
{
#endif  
	#include "libavutil/imgutils.h"    //图像工具   
	#include "libavutil/samplefmt.h"  // 音频样本格式  
	#include "libavutil/timestamp.h"  //时间戳工具可以 被用于调试和日志目的   
	#include "libavcodec\avcodec.h"
	#include "libavformat/avformat.h" //Main libavformat public API header  包含了libavf I/O和   Demuxing  和Muxing 库     
	#include "SDL.h"  
#ifdef _CPPRTTI   
};
#endif  

//音视频编码器上下文  
static AVCodecContext *pVideoContext, *pAudioContext;
static FILE *fVideoFile, *fAudioFile;  //输出文件句柄  
static AVStream *pStreamVideo, *pStreamAudio; //媒体流    
static unsigned char * videoDstData[4];  //视频数据   
static int videoLineSize[4]; //   
static int videoBufferSize; //视频缓冲区大小   
static AVFormatContext *pFormatCtx = NULL; //格式上下文  
static AVFrame*pFrame = NULL; //  
static AVPacket pkt;  //解码媒体包  
static int ret = 0; //状态  
static int gotFrame; //获取到的视频流 

AVCodec *pVideoDec = NULL;
AVCodec *pAudioDec = NULL;

					 //音视频流的索引  
static int videoStreamIndex, audioStreamIndex;
//解码媒体包  
//SDL定义    
SDL_Window * pWindow = NULL;
SDL_Renderer *pRender = NULL;
SDL_Texture *pTexture = NULL;
SDL_Rect dstrect = { 0,0,800,600 };
int frame = 0;
int indexFrameVideo = 0;
static int decode_packet(int* gotFrame, int param2)
{
	int ret = 0;
	//解码数据大小  
	int decodedSize = pkt.size;
	//初始化获取的数据帧为0  
	*gotFrame = 0;
	//如果是视频流那么 解包视频流    
	if (pkt.stream_index == videoStreamIndex)
	{

		//解码数据到视频帧  
		if ((ret = avcodec_decode_video2(pVideoContext, pFrame, gotFrame, &pkt))<0)
		{
			//解码视频帧失败  
			return ret;
		}
		indexFrameVideo++;
		//copy 解压后的数据到我们分配的空间中  
		if (*gotFrame)
		{
			//拷贝数据  
			av_image_copy(videoDstData, videoLineSize, (const uint8_t **)(pFrame->data), pFrame->linesize, pVideoContext->pix_fmt, pVideoContext->width, pVideoContext->height);
			//写入数据到缓冲区  
			fwrite(videoDstData[0], 1, videoBufferSize, fVideoFile);  
			printf("输出当前第%d帧,大小:%d\n", indexFrameVideo, videoBufferSize);
			int n = SDL_BYTESPERPIXEL(pStreamVideo->codec->pix_fmt);

			//更新纹理  

			SDL_UpdateTexture(pTexture, &dstrect, (const void*)videoDstData[0], videoLineSize[0]);

			//拷贝纹理到2D模块  
			SDL_RenderCopy(pRender, pTexture, NULL, &dstrect);
			//延时 1000ms*1/25  
			SDL_Delay(1000 * 1 / frame);
			//显示Render渲染曾  
			SDL_RenderPresent(pRender);
		}
		else
		{
			printf("第%d帧,丢失\n", indexFrameVideo);
		}
	}
	//音频不管  
	else if (pkt.stream_index == audioStreamIndex)
	{
		//解码音频信息  
		if ((ret = avcodec_decode_audio4(pAudioContext, pFrame, gotFrame, &pkt)) < 0)  
		    return ret;  
		decodedSize = FFMIN(ret, pkt.size);  
		//算出当前帧的大小  
		size_t unpadded_linesize = pFrame->nb_samples * av_get_bytes_per_sample((AVSampleFormat)pFrame->format);   
		///写入数据到音频文件  
		fwrite(pFrame->extended_data[0], 1, unpadded_linesize, fAudioFile);     
	}
	//取消所有引用  并且重置frame字段  
	av_frame_unref(pFrame);
	return decodedSize;
}

int Demuxing(int argc, char** argv)
{
	if (argc < 4)
	{
		printf("Parameter Error!\n");
		return 0;
	}

	//注册所有混流器 过滤器  
	av_register_all();
	//注册所有编码器  
	avcodec_register_all();
	//媒体输入源头  
	char*pInputFile = argv[1];
	strcpy(pInputFile, argv[1]);
	//视频输出文件  
	char*pOutputVideoFile = argv[2];
	//音频输出文件  
	char*pOutputAudioFile = argv[3];
	//分配环境上下文  
	pFormatCtx = avformat_alloc_context();
	//打开输入源  并且读取输入源的头部  
	if (avformat_open_input(&pFormatCtx, pInputFile, NULL, NULL) < 0)
	{
		printf("Open Input Error!\n");
		return 0;
	}
	//获取流媒体信息  
	if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
	{
		printf("获取流媒体信息失败!\n");
		return 0;
	}
	//打印媒体信息  
	av_dump_format(pFormatCtx, 0, pInputFile, 0);
	for (unsigned i = 0; i < pFormatCtx->nb_streams; i++)
	{
		AVStream *pStream = pFormatCtx->streams[i];
		AVMediaType mediaType = pStream->codec->codec_type;
		//提取不同的编解码器  
		if (mediaType == AVMEDIA_TYPE_VIDEO)
		{
			videoStreamIndex = i;
			pVideoContext = pStream->codec;
			pStreamVideo = pStream;
			fVideoFile = fopen(pOutputVideoFile, "wb");
			frame = pVideoContext->framerate.num;
			if (!fVideoFile)
			{
				printf("con't open file!\n");
				goto end;
			}
			//计算解码后一帧图像的大小  
			//int nsize = avpicture_get_size(PIX_FMT_YUV420P, 1280, 720);  
			//分配计算初始化 图像缓冲区 调色板数据  
			int ret = av_image_alloc(videoDstData, videoLineSize, pVideoContext->width, pVideoContext->height, pVideoContext->pix_fmt, 1);
			if (ret < 0)
			{
				printf("Alloc video buffer error!\n");
				goto end;
			}
			//avpicture_fill((AVPicture *)pFrame, videoDstData[0], PIX_FMT_YUV420P, 1280, 720);  
			videoBufferSize = ret;

			//根据编码器id查找编码器  
			pVideoDec = avcodec_find_decoder(pStream->codec->codec_id);
			if (pVideoDec == NULL)
			{
				printf("查找编码器失败!\n");
				goto end;
			}
			if (avcodec_open2(pStream->codec, pVideoDec, nullptr) != 0)
			{
				printf("打开编码器失败!\n");
				goto end;
			}
		}
		else if (mediaType == AVMEDIA_TYPE_AUDIO)
		{
			audioStreamIndex = i;
			pAudioContext = pStream->codec;
			pStreamAudio = pStream;
			fAudioFile = fopen(pOutputAudioFile, "wb");
			if (!fAudioFile)
			{
				printf("con't open file!\n");
				goto end;
			}
			//分配视频帧  
			pFrame = av_frame_alloc();
			if (pFrame == NULL)
			{
				av_freep(&videoDstData[0]);
				printf("alloc audio frame error\n");
				goto end;
			}
			//根据编码器id查找编码器  
			pAudioDec = avcodec_find_decoder(pStream->codec->codec_id);
			if (pAudioDec == NULL)
			{
				printf("查找编码器失败!\n");
				goto end;
			}
			if (avcodec_open2(pStream->codec, pAudioDec, nullptr) != 0)
			{
				printf("打开编码器失败!\n");
				goto end;
			}
		}
	}
	av_init_packet(&pkt);
	pkt.data = NULL;
	pkt.size = 0;

	//读取媒体数据包  数据要大于等于0  
	while (av_read_frame(pFormatCtx, &pkt) >= 0)
	{
		AVPacket oriPkt = pkt;
		do
		{
			//返回每个包解码的数据  
			ret = decode_packet(&gotFrame, 0);
			if (ret < 0)
				break;
			//指针后移  空闲内存减少  
			pkt.data += ret;
			pkt.size -= ret;
			//  
		} while (pkt.size > 0);
		//释放之前分配的空间  读取完毕必须释放包  
		av_free_packet(&oriPkt);
	}

end:
	//关闭视频编码器  
	avcodec_close(pVideoContext);
	//关闭音频编码器  
	avcodec_close(pAudioContext);
	avformat_close_input(&pFormatCtx);
	if (fVideoFile != NULL)
	{
		fclose(fVideoFile);
		fVideoFile = NULL;
	}
	
	if(fAudioFile != NULL)
	{
		fclose(fAudioFile);
		fAudioFile = NULL;
	}
	
	//释放编码帧  
	//avcodec_free_frame(&pFrame);
	av_free(&pFrame);
	//释放视频数据区  
	av_free(videoDstData[0]);
	return 0;
}

int main(int argc, char*argv[])
{
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER))
	{
		printf("Could not initialize SDL - %s\n", SDL_GetError());
		return -1;
	}

#if 0
	//SDL_AudioSpec  
	SDL_AudioSpec wanted_spec;
	wanted_spec.freq = out_sample_rate;
	wanted_spec.format = AUDIO_S16SYS;
	wanted_spec.channels = out_channels;
	wanted_spec.silence = 0;
	wanted_spec.samples = out_nb_samples;
	wanted_spec.callback = fill_audio;
	wanted_spec.userdata = pCodecCtx;

	if (SDL_OpenAudio(&wanted_spec, NULL)<0) {
		printf("can't open audio.\n");
		return -1;
	}
#endif
	//创建窗口  
	pWindow = SDL_CreateWindow("YUV420P", 200, 100, 800, 600, 0);
	//启用硬件加速   
	pRender = SDL_CreateRenderer(pWindow, -1, 0);
	dstrect.x = 0;
	dstrect.y = 0;
	dstrect.w = 1280;
	dstrect.h = 720;
	//创建一个纹理  设置可以Lock  YUV420P 格式 1280*720   
	pTexture = SDL_CreateTexture(pRender, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, 1280, 720);
	Demuxing(argc, argv);
	//释放  
	SDL_RenderClear(pRender);
	SDL_DestroyTexture(pTexture);
	SDL_DestroyRenderer(pRender);
	SDL_DestroyWindow(pWindow);
	SDL_Quit();
	return  0;
}

#endif

#endif