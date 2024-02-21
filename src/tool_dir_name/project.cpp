
#include "project.h"

#include <libavcodec/avcodec.h>
#include "libavutil/imgutils.h"
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>


// 拉取视频流并保存到文件
void tool_class::pullVideoFromCamera(const char* outputFileName, int width, int height)
{
    avdevice_register_all();

    AVInputFormat* inputFormat = const_cast<AVInputFormat*>(av_find_input_format("dshow"));
    if (!inputFormat) {
        std::cout << "Cannot find input format" << std::endl;
        return;
    }

    AVFormatContext* formatContext = nullptr;
    AVDictionary* options = nullptr;

    // 设置摄像头参数
    av_dict_set(&options, "video_size", std::to_string(width).c_str(), 0);
    av_dict_set(&options, "framerate", "30", 0);
    av_dict_set(&options, "pixel_format", "yuyv422", 0);

    // 打开摄像头
    if (avformat_open_input(&formatContext, "video=Integrated Camera", inputFormat, &options) != 0) {
        std::cout << "Failed to open camera" << std::endl;
        return;
    }

    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
        std::cout << "Failed to find stream information" << std::endl;
        return;
    }

    // 查找视频流
    int videoStreamIndex = -1;
    for (unsigned int i = 0; i < formatContext->nb_streams; ++i) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            break;
        }
    }

    if (videoStreamIndex == -1) {
        std::cout << "Failed to find video stream" << std::endl;
        return;
    }

    AVCodecParameters* codecParameters = formatContext->streams[videoStreamIndex]->codecpar;
    AVCodec* codec = const_cast<AVCodec*>(avcodec_find_decoder(codecParameters->codec_id));
    if (!codec) {
        std::cout << "Unsupported codec" << std::endl;
        return;
    }

    AVCodecContext* codecContext = avcodec_alloc_context3(codec);
    if (!codecContext) {
        std::cout << "Failed to allocate codec context" << std::endl;
        return;
    }

    if (avcodec_parameters_to_context(codecContext, codecParameters) < 0) {
        std::cout << "Failed to copy codec parameters to context" << std::endl;
        return;
    }

    if (avcodec_open2(codecContext, codec, nullptr) < 0) {
        std::cout << "Failed to open codec" << std::endl;
        return;
    }

    AVFrame* frame = av_frame_alloc();
    if (!frame) {
        std::cout << "Failed to allocate frame" << std::endl;
        return;
    }

    AVPacket* packet = av_packet_alloc();
    if (!packet) {
        std::cout << "Failed to allocate packet" << std::endl;
        return;
    }

    SwsContext* swsContext = sws_getContext(codecContext->width, codecContext->height, codecContext->pix_fmt,
        width, height, AV_PIX_FMT_BGR24, SWS_BILINEAR, nullptr, nullptr, nullptr);
    if (!swsContext) {
        std::cout << "Failed to allocate SwsContext" << std::endl;
        return;
    }

    FILE* outputFile = fopen(outputFileName, "wb");
    if (!outputFile) {
        std::cout << "Failed to open output file" << std::endl;
        return;
    }

    while (av_read_frame(formatContext, packet) >= 0) {
        if (packet->stream_index == videoStreamIndex) {
            avcodec_send_packet(codecContext, packet);
            int ret = avcodec_receive_frame(codecContext, frame);
            if (ret == 0) {
                // 转换像素格式
                AVFrame* rgbFrame = av_frame_alloc();
                uint8_t* buffer = (uint8_t*)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_BGR24, width, height, 1));
                av_image_fill_arrays(rgbFrame->data, rgbFrame->linesize, buffer, AV_PIX_FMT_BGR24, width, height, 1);

                sws_scale(swsContext, (const uint8_t* const*)frame->data, frame->linesize, 0, codecContext->height,
                    rgbFrame->data, rgbFrame->linesize);

                // 写入文件
                fwrite(rgbFrame->data[0], 1, width * height * 3, outputFile);

                av_frame_free(&rgbFrame);
                av_free(buffer);
            }
        }
        av_packet_unref(packet);
    }

    fclose(outputFile);
    av_frame_free(&frame);
    av_packet_free(&packet);
    avcodec_free_context(&codecContext);
    avformat_close_input(&formatContext);
    sws_freeContext(swsContext);
}


tool_class::tool_class()
{
	std::cout << "Start tool_class" << std::endl;

}
tool_class::~tool_class()
{
	std::cout << "End tool_class" << std::endl;

}