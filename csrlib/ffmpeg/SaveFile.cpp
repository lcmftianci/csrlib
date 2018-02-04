#include "SaveFile.h"

using namespace std;

int OpenInput(std::string inputUrl)
{
	inputContext = avformat_alloc_context();
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

std::shared_ptr<AVPacket> ReadPacketFromSource()
{

}

