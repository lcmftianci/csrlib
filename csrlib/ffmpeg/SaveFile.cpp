#include "SaveFile.h"

using namespace std;

AVFormatContext *inputContext = nullptr;
AVFormatContext *outputContext;
int64_t lastReadPacktTime;

static int interrupt_cb(void *ctx)
{
	int timeout = 3;
	if (av_gettime() - lastReadPacktTime > timeout * 1000 * 1000)
		return -1;
	return 0;
}

//打开网络流
int OpenInput(std::string inputUrl)
{
	inputContext = avformat_alloc_context();

	//注册延时回调检测函数
	lastReadPacktTime = av_gettime();
	inputContext->interrupt_callback.callback = interrupt_cb;
	
	int ret = avformat_open_input(&inputContext, inputUrl.c_str(), nullptr, nullptr);
	if(ret < 0)
	{
		av_log(NULL, AV_LOG_ERROR, "Input File open failed\n");
		return ret;
	}

	ret = avformat_find_stream_info(inputContext, nullptr);
	if (ret < 0)
	{
		av_log(NULL, AV_LOG_ERROR, "Find input file stream information failed\n");
	}
	else
	{
		av_log(NULL, AV_LOG_FATAL, "Open input file %s success\n", inputUrl.c_str());
	}
	return ret;
}

//读取数据包
std::shared_ptr<AVPacket> ReadPacketFromSource()
{
	shared_ptr<AVPacket> packet(static_cast<AVPacket*>(av_malloc(sizeof(AVPacket))), [&](AVPacket *p){ av_packet_free(&p); av_freep(&p);});
	av_init_packet(packet.get());
	lastReadPacktTime = av_gettime();
	int ret = av_read_frame(inputContext, packet.get());
	if (ret >= 0)
		return packet;
	else
		return nullptr;
}

//初始化ffmpeg
void Init()
{
	av_register_all();
	avfilter_register_all();
	avformat_network_init();
	av_log_set_level(AV_LOG_ERROR);
}
