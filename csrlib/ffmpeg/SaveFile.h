#pragma once


#include "common.h"

#include <iostream>
#include <thread>
#include <memory>

AVFormatContext* inputContext = nullptr;
AVFormatContext* outputComtext;


int OpenInput(std::string inputUrl);


std::shared_ptr<AVPacket> ReadPacketFromSource();
