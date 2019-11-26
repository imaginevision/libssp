#define _GLIBCXX_USE_CXX11_ABI 1
#include <functional>
#include <memory>
#include <thread>

#include <stdlib.h>

#include "imf/net/loop.h"
#include "imf/net/threadloop.h"
#include "imf/ssp/sspclient.h"

using namespace std::placeholders;

#ifdef _DEBUG
#pragma comment (lib, "libsspd.lib")
#else
#pragma comment (lib, "libssp.lib")
#endif



static void on_264_1(struct imf::SspH264Data * h264)
{
	printf("on 1 264 [%d] [%lld] [%d] [%d]\n", h264->frm_no, h264->pts, h264->type, h264->len);
}

static void on_264_2(struct imf::SspH264Data * h264)
{
	printf("on 2 264 [%d] [%lld] [%d] [%d]\n", h264->frm_no, h264->pts, h264->type, h264->len);
}

static void on_audio_data_1(struct imf::SspAudioData * audio)
{

}

static void on_audio_data_2(struct imf::SspAudioData * audio)
{

}

static void on_meta_1(struct imf::SspVideoMeta *v, struct imf::SspAudioMeta *a, struct imf::SspMeta * m)
{
	printf("on meta 1 wall clock %d", m->pts_is_wall_clock);
	printf("              video %dx%d %d/%d\n", v->width, v->height, v->unit, v->timescale);
	printf("              audio %d\n", a->sample_rate);
}

static void on_meta_2(struct imf::SspVideoMeta *v, struct imf::SspAudioMeta *a, struct imf::SspMeta * m)
{

}

static void on_disconnect()
{
	printf("on disconnect\n");
}

static void setup(imf::Loop *loop)
{
	std::string ip = "192.168.9.86";
	imf::SspClient * client = new imf::SspClient(ip, loop, 0x400000);
	client->init();

	client->setOnH264DataCallback(std::bind(on_264_1, _1));
	client->setOnMetaCallback(std::bind(on_meta_1, _1, _2, _3));
	client->setOnDisconnectedCallback(std::bind(on_disconnect));
	client->setOnAudioDataCallback(std::bind(on_audio_data_1, _1));

	client->start();

	////////////////////
	//client = new imf::SspClient(std::string("10.98.32.2"), loop, 0x400000);
	//client->init();

	//client->setOnH264DataCallback(std::bind(on_264_2, _1));
	//client->setOnMetaCallback(std::bind(on_meta_2, _1, _2, _3));
	//client->setOnDisconnectedCallback(std::bind(on_disconnect));
	//client->setOnAudioDataCallback(std::bind(on_audio_data_2, _1));

	//client->start();

	// add more SspClient if you need
}

int main(int argc, char ** argv)
{
	std::unique_ptr<imf::ThreadLoop> threadLooper(new imf::ThreadLoop(std::bind(setup, _1)));
	threadLooper->start();

	while (1) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	threadLooper->stop();
	return 0;
}

