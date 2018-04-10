还是参照雷大神的例子改写得，但是改写过程中碰到几个问题，纠结的比较久的问题是解码MP4，
avi文件，播放出来的音频不仅很多杂音而且断断续续。因为是刚接触这块，没什么音频方面得知识，
出了问题也找不到问题点，只能百度到处看别人的帖子。一开始我觉得是SDL播放的问题，于是用雷神的程序，把同样的源文件先解码保存成PCM纯文件，然后下载了一个工具去播放，发现效果和SDL代码实现一致，还是有杂音。由此把问题确定是在解码的过程中。网上帖子大部分是都是说是由于avcodec_decode_audio4解码得到的是plane格式，需要用swr_convert进行格式转化。（最后确实发现是这个转化中采样率导致的）
但是我代码中已经加入了这个转化，并且解码aac文件并没有问题。所以一直想不通，期间陆陆续续看了很多帖子，慢慢就稍稍了解了一些音频的基本知识。
swr_alloc_set_opts(struct SwrContext *s,

                                      int64_t out_ch_layout, enum AVSampleFormat out_sample_fmt, int out_sample_rate,

                                      int64_t  in_ch_layout, enum AVSampleFormat  in_sample_fmt, int  in_sample_rate,

                                      int log_offset, void *log_ctx);



如果out_sample_rate低于in_sample_rate，则会出现杂音，猜测是因为采样率低导致数据丢失得原因。


再具体的见代码

// Audio_Player_demon_1.cpp : 定义控制台应用程序的入口点。

//



#include "stdafx.h"



#pragma comment(lib, "avcodec.lib")

#pragma comment(lib, "avformat.lib")

#pragma comment(lib, "avutil.lib")

#pragma comment(lib, "SDL2.lib")

#pragma comment(lib, "swscale.lib")

#pragma comment(lib, "swresample.lib")



#define MAX_AUDIO_FRAME_SIZE 192000 // 1 second of 48khz 32bit audio



static  Uint8  *audio_chunk; 

static  Uint32  audio_len; 

static  Uint8  *audio_pos; 



/* The audio function callback takes the following parameters: 

* stream: A pointer to the audio buffer to be filled 

* len: The length (in bytes) of the audio buffer 

* 回调函数

*/ 

void  fill_audio(void *udata,Uint8 *stream,int len);



int _tmain(int argc, _TCHAR* argv[])

{

av_register_all();



AVFormatContext *pAvFmtCtx;

AVCodecContext *pAvCodCtx;



pAvFmtCtx = avformat_alloc_context();



//char fileName[] = "WavinFlag.aac";

char fileName[] = "test.avi";


avformat_open_input(&pAvFmtCtx, fileName, NULL, NULL);



av_find_stream_info(pAvFmtCtx);



for (int i=0; i<pAvFmtCtx->nb_streams; ++i)

{

if (AVMEDIA_TYPE_AUDIO == pAvFmtCtx->streams[i]->codec->codec_type)

{

pAvCodCtx = pAvFmtCtx->streams[i]->codec;

break;

}


}



AVCodec *pAvcode;

pAvcode = avcodec_find_decoder(pAvCodCtx->codec_id);

avcodec_open2(pAvCodCtx, pAvcode, NULL);


AVPacket *pAvPct;

AVFrame *pAvFrame;



pAvPct = (AVPacket *)malloc(sizeof(AVPacket));

av_init_packet(pAvPct);



//AVFrame结构体不能使用下面方式初始化，主要原因猜测应该是av_malloc只能初始化实例，不能初始化buff

//pAvFrame = (pAvFrame *)av_malloc(sizeof(pAvFrame));



pAvFrame = av_frame_alloc();



uint8_t *uBuff;

//Out Audio Param

uint64_t out_channel_layout=AV_CH_LAYOUT_STEREO;

int out_nb_samples=1024;

AVSampleFormat out_sample_fmt=AV_SAMPLE_FMT_S16;



       //雷神这边是固定44100，主要杂音问题就是在这里，因为有些文件是48000

int out_sample_rate=pAvCodCtx->sample_rate;

        

int out_channels=av_get_channel_layout_nb_channels(out_channel_layout);



int iBufSize;

iBufSize = av_samples_get_buffer_size(NULL, out_channels, out_nb_samples, out_sample_fmt, 1);



uBuff = (uint8_t *)av_malloc(MAX_AUDIO_FRAME_SIZE*2);;



//SDL

SDL_Init(SDL_INIT_AUDIO|SDL_INIT_TIMER);

//SDL_AudioSpec *pOutSpec;



//SDL_AudioSpec

SDL_AudioSpec pOutSpec;

pOutSpec.freq = pAvCodCtx->sample_rate; 

//pOutSpec.freq = 48000; 

pOutSpec.format = AUDIO_S16SYS; 

pOutSpec.channels = out_channels; 

pOutSpec.silence = 0; 

pOutSpec.samples = out_nb_samples; 

pOutSpec.callback = fill_audio; 

pOutSpec.userdata = pAvCodCtx; 



SwrContext *pSwCtx;

pSwCtx = swr_alloc();

pSwCtx = swr_alloc_set_opts(pSwCtx, out_channel_layout, out_sample_fmt, out_sample_rate, pAvCodCtx->channel_layout, pAvCodCtx->sample_fmt, pAvCodCtx->sample_rate, 0, NULL);

swr_init(pSwCtx);



int iGetFrame;

SDL_OpenAudio(&pOutSpec, NULL);



while(av_read_frame(pAvFmtCtx, pAvPct)>= 0)

{



avcodec_decode_audio4(pAvCodCtx, pAvFrame, &iGetFrame, pAvPct);



if (iGetFrame)

{

int data_size;

data_size = av_samples_get_buffer_size(nullptr, pAvCodCtx->channels, pAvFrame->nb_samples, pAvCodCtx->sample_fmt, 1);

//assert(data_size <= buf_size);

memcpy(uBuff, pAvFrame->data[0], data_size);





int dst_nb_samples = av_rescale_rnd(swr_get_delay(pSwCtx, pAvFrame->sample_rate) + pAvFrame->nb_samples,

pOutSpec.freq, pOutSpec.format, AVRounding(1));



swr_convert(pSwCtx, &uBuff, MAX_AUDIO_FRAME_SIZE, (const uint8_t **)pAvFrame->data, pAvFrame->nb_samples);



if(pOutSpec.samples!=pAvFrame->nb_samples)

{

SDL_CloseAudio();

out_nb_samples=pAvFrame->nb_samples;

iBufSize=av_samples_get_buffer_size(NULL,out_channels ,out_nb_samples,out_sample_fmt, 1);

pOutSpec.samples=out_nb_samples;

SDL_OpenAudio(&pOutSpec, NULL);





}



audio_chunk = (Uint8 *) uBuff; 

//Audio buffer length

audio_len =iBufSize;



audio_pos = audio_chunk;

//Play

SDL_PauseAudio(0);

while(audio_len>0)//Wait until finish

SDL_Delay(1); 



}



/*audio_chunk = (Uint8 *) uBuff; 

//Audio buffer length

audio_len =iBufSize;



audio_pos = audio_chunk;

//Play

SDL_PauseAudio(0);

while(audio_len>0)//Wait until finish

SDL_Delay(1); 



av_free_packet(pAvPct);*/

}




return 0;

}



void fill_audio(void *udata, Uint8 *stream, int len)

{

SDL_memset(stream, 0, len);

if (audio_len == 0)

{

printf("audio_len=0\n");

return;

}



len = (len>audio_len?audio_len:len);

SDL_MixAudio(stream,audio_pos,len,SDL_MIX_MAXVOLUME);//对音频数据进行混音,感觉这边混音的用处就是为了填充缓存

//memcpy(stream, audio_pos, len);

audio_pos+=len;//更新播放位置

audio_len-=len;//更新数据长度

}











PS：整个代码基本流程如下
视频、音频解码过程都是一致的，首先
av_register_all()  注册所需要的函数
avformat_open_input打开文件
av_find_stream_info 取流文件，这时候只有文件的头部信息，比如文件长度、码率之类。
avcodec_find_decoder 通过流文件信息获取解码器信息
avcodec_open2 打开解码器
av_read_frame  取信息包。
最后解码。


https://github.com/brookicv/FSplayer.git


			int pkt_size = packet->size;
			for(;;)//解码，必须解码完，因为可能不止一帧数据。改变packet->data的指针
			{
				int got_picture=0;
				Lock.Lock();
				int ret = avcodec_decode_audio4(pCodecCtx_A, pFrame_A, &got_picture, packet);
				Lock.Unlock();
				if(ret<0)
					break;
				if(got_picture<=0)
					break;
				if(got_picture&&ret>=0)//这里是resample
				{  
					int bytes_per_sample=Getbytes_per_sample(out_sample_fmt);
					int in_samples_per_channel=pFrame_A->nb_samples;
					int out_samples_per_channel= av_rescale_rnd(128 +in_samples_per_channel , out_sample_rate, pFrame_A->sample_rate, AV_ROUND_UP);
					int size_per_sample_with_channels = out_channels*bytes_per_sample;
					int out_size = out_samples_per_channel*size_per_sample_with_channels;
					unsigned char *out[] = {(unsigned char*)audio_out_buffer};
					int converted_samplers_per_channel=swr_convert(audio_convert_ctx,out, out_samples_per_channel,(const uint8_t **)pFrame_A->extended_data , in_samples_per_channel);  
					if(converted_samplers_per_channel>0)
					{
						fifo.push((BYTE*)audio_out_buffer,converted_samplers_per_channel*size_per_sample_with_channels);
					}
				}
				Lock.Lock();
				packet->data+=ret;	//重要！！，必须改变输入数据的指针
				Lock.Unlock();
				pkt_size-=ret;
				if(pkt_size<=0)
					break;
			}
			
			需要注意的是，一般网上的代码只有int ret = avcodec_decode_audio4(pCodecCtx_A, pFrame_A, &got_picture, packet);然后调用swr_convert进行转换成输出的PCM格式，这样的方法对于ADPCM压缩的音频文件或其他某些文件不奏效，声音会出现断裂从而听上去像加速播放的效果。这种方法实际上是理想化了，int nRead=av_read_frame(pFormatCtx, packet)分离（demux）出来的音频帧有可能并不止1帧，所以解码那里必须有个死循环，通过int ret = avcodec_decode_audio4(pCodecCtx_A, pFrame_A, &got_picture, packet);解码的返回值判断解码了多少数据，然后改变解码数据的指针继续解码剩下的音频帧，直至解码完成！

记录下，以备后用，也希望能帮助到遇到同样问题的您！



http://dranger.com/ffmpeg/tutorial03.html

https://blog.csdn.net/beichen2012/article/details/61194285	openal

https://www.cnblogs.com/CoderTian/p/6791638.html





