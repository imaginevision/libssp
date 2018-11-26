#pragma once

#include <functional>

#ifdef _WIN32
#ifdef FULL_SOURCE
#define LIBSSP_API
#else
#ifdef LIBSSP_EXPORTS
#define LIBSSP_API __declspec(dllexport)
#else
#define LIBSSP_API __declspec(dllimport)
#endif
#endif
#else
#define LIBSSP_API
#endif

#define ERROR_SSP_PROTOCOL_VERSION_GT_SERVER        (-1000)
#define ERROR_SSP_PROTOCOL_VERSION_LT_SERVER        (-1001)
#define ERROR_SSP_CONNECTION_FAILED                 (-1002)
#define ERROR_SSP_CONNECTION_EXIST                  (-1003)

namespace imf
{

enum {
	STREAM_DEFAULT = 0,
	STREAM_MAIN = 1,
	STREAM_SEC = 2
};

#define VIDEO_ENCODER_UNKNOWN	(0)
#define VIDEO_ENCODER_H264	(96)
#define VIDEO_ENCODER_H265	(265)

#define AUDIO_ENCODER_UNKNOWN	(0)
#define AUDIO_ENCODER_AAC	(37)
#define AUDIO_ENCODER_PCM	(23)

struct LIBSSP_API SspVideoMeta {
	uint32_t width;
	uint32_t height;
	uint32_t timescale;
	uint32_t unit;
	uint32_t gop;
	uint32_t encoder;
};

struct LIBSSP_API SspAudioMeta {
	uint32_t timescale;
	uint32_t unit;
	uint32_t sample_rate;
	uint32_t sample_size;
	uint32_t channel;
	uint32_t bitrate;
	uint32_t encoder;
};

struct LIBSSP_API SspMeta {
	bool pts_is_wall_clock;
};

struct LIBSSP_API SspH264Data {
	uint8_t * data;
	size_t len;
	uint64_t pts;
	uint64_t ntp_timestamp;
	uint32_t frm_no;
	uint32_t type;			// I or P
};

struct LIBSSP_API SspAudioData {
	uint8_t * data;
	size_t len;
	uint64_t pts;
	uint64_t ntp_timestamp;
};

typedef std::function <void(void)> OnRecvBufferFullCallback;					// called when the recv buffer is full
typedef std::function <void(void)> OnDisconnectedCallback;						// called when the session is closed
typedef std::function <void(void)> OnConnectionConnectedCallback;				// called when the session is est
typedef std::function <void(struct SspH264Data * h264)> OnH264DataCallback;		// called every video frame is ready. Actually, it's a video callback, no matter it's compression format
typedef std::function <void(struct SspAudioData * audio)> OnAudioDataCallback;	// called every audio frame is ready
typedef std::function <void(struct SspVideoMeta*, struct SspAudioMeta*, struct SspMeta *)> OnMetaCallback;	// meta data callback
typedef std::function <void(int code, const char* description)> OnExceptionCallback;	// exception

class Loop;
class SspClientImp;
class LIBSSP_API SspClient
{
public:
	SspClient(const std::string & ip, Loop *loop, size_t bufSize, unsigned short port = 9999, uint32_t streamStyle = STREAM_DEFAULT);
	~SspClient();

	int init(void);
	int start(void);
	int stop(void);

	void setOnRecvBufferFullCallback(const OnRecvBufferFullCallback & cb);
	void setOnH264DataCallback(const OnH264DataCallback & cb);
	void setOnAudioDataCallback(const OnAudioDataCallback & cb);
	void setOnMetaCallback(const OnMetaCallback & cb);
	void setOnDisconnectedCallback(const OnDisconnectedCallback & cb);
	void setOnConnectionConnectedCallback(const OnConnectionConnectedCallback & cb);
	void setOnExceptionCallback(const OnExceptionCallback & cb);
private:
	SspClientImp *imp_;
};
}