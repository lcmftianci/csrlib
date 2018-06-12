#ifndef PACKET_QUEUE_H_
#define PACKET_QUEUE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
#include <SDL.h>

	typedef struct PacketQueue
	{
		AVPacketList    *first_pkt;     //��ͷ��һ��packet, ע�����Ͳ���AVPacket
		AVPacketList    *last_pkt;      //��βpacket
		int             nb_packets;     // paket����
		int             size;           //
		SDL_mutex       *mutex;         //
		SDL_cond        *cond;          // ��������
	}PacketQueue;

	void packet_queue_init(PacketQueue *queue);
	int packet_queue_put(PacketQueue *queue, AVPacket *packet);
	int packet_queue_get(PacketQueue *queue, AVPacket *pakcet, int block);

#ifdef __cplusplus
}
#endif
#endif