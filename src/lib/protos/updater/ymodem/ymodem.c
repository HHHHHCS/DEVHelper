#include <stdlib.h>
#include <string.h>

#include "ymodem.h"


#ifndef USE_YMODEM_1K || USE_YMODEM_G
#error  Please define flag either USE_YMODEM_G or USE_YMODEM_1K first
#else

#ifdef USE_YMODEM_G
#define FRAME_HEADER_FOOTER_SIZE 3U
#elif defined USE_YMODEM_1K
#define FRAME_HEADER_FOOTER_SIZE 5U
#else
#define FRAME_HEADER_FOOTER_SIZE 5U
#endif

#define FRAME_BE_DATA_SIZE 128U
#define FRAME_BE_TOTAL_SIZE (FRAME_BE_DATA_SIZE + FRAME_HEADER_FOOTER_SIZE)
#define FRAME_D_DATA_SIZE 1024U
#define FRAME_D_TOTAL_SIZE (FRAME_D_DATA_SIZE + FRAME_HEADER_FOOTER_SIZE)

static uint8_t g_frame_seq;

static uint16_t crc16(const uint8_t *data, size_t data_size)
{
    uint16_t crc = 0;
    char i = 0;

    while (--data_size >= 0)
    {
        crc = crc ^ (int) * data++ << 8;
        i = 8;
        do
        {
            if (crc & 0x8000)
            {
                crc = crc << 1 ^ 0x1021;
            }
            else
            {
                crc = crc << 1;
            }
        }
        while (--i);
    }

    return crc;
}

static inline YModem128BytesFrame pack128BytesFrame(const char* data, size_t data_size)
{
    YModem128BytesFrame frame;
    memset(&frame, 0, sizeof(YModem128BytesFrame));

    if(data_size > FRAME_BE_DATA_SIZE)
    {
        return frame;
    }

    frame.code_type = FRAME_CODE_SOH;
    frame.seq = g_frame_seq++;
    frame.seq_inverse = ~frame.seq;
    memcpy(&frame.data[0], data, data_size);
#ifdef USE_YMODEM_1K
    frame.crc_16 = crc16(&frame.data[0], sizeof(frame.data));
#endif

    return frame;
}

static inline YModem1024BytesFrame pack1024BytesFrame(const char* data, size_t data_size)
{
    YModem1024BytesFrame frame;
    memset(&frame, 0, sizeof(YModem1024BytesFrame));

    if(data_size > FRAME_D_DATA_SIZE)
    {
        return frame;
    }

    frame.code_type = FRAME_CODE_STX;
    frame.seq = g_frame_seq++;
    frame.seq_inverse = ~frame.seq;
    memcpy(&frame.data[0], data, data_size);
#ifdef USE_YMODEM_1K
    frame.crc_16 = crc16(&frame.data[0], sizeof(frame.data));
#endif

    return frame;
}

void ymodemBeginFrame(const char *file_name, const size_t file_name_size, uint32_t file_size, char *frame_out)
{
    if(!file_name || !file_size)
    {
        return;
    }

    char file_size_str[15];
    memset(&file_size_str[0], 0, sizeof(file_size_str));
    ultoa(file_size, file_size_str, 16);

    size_t file_size_str_len = strlen(file_size_str);
    size_t frame_data_size = file_name_size + file_size_str_len + 2;

    g_frame_seq = 0;

    char frame_data[FRAME_BE_DATA_SIZE];
    memset(&frame_data[0], 0, sizeof(frame_data));
    memcpy(&frame_data[0], file_name, file_name_size);
    memcpy(&frame_data[file_name_size + 1], file_size_str, file_size_str_len);

    YModem128BytesFrame frame = pack128BytesFrame(frame_data, sizeof(frame_data));

    memcpy(frame_out, (char*)&frame, sizeof(frame));
}

void ymodemEndtFrame(char *frame_out)
{
    g_frame_seq = 0;

    char frame_data[FRAME_BE_DATA_SIZE];
    memset(&frame_data[0], 0, sizeof(frame_data));

    YModem128BytesFrame frame = pack128BytesFrame(frame_data, sizeof(frame_data));

    memcpy(frame_out, (char*)&frame, sizeof(frame));
}

void ymodemTransmitFrame(const char* data, size_t data_size, char* frame_out)
{
    if(!data)
    {
        return;
    }

    if(data_size > FRAME_D_DATA_SIZE)
    {
        return;
    }

    YModem1024BytesFrame frame = pack1024BytesFrame(data, data_size);

    memcpy(frame_out, (char*)&frame, sizeof(frame));
}

void ymodemHandshake(char* frame_out)
{
    char frame_data = FRAME_CODE_C;
    memcpy(frame_out, &frame_data, sizeof(frame_data));
}

void ymodemTransmitEndFrame(char* frame_out)
{
    char frame_data = FRAME_CODE_EOT;
    memcpy(frame_out, &frame_data, sizeof(frame_data));
}

void ymodemTransmitAckFrame(char* frame_out)
{
    char frame_data = FRAME_CODE_ACK;
    memcpy(frame_out, &frame_data, sizeof(frame_data));
}

void ymodemTransmitNAckFrame(char* frame_out)
{
    char frame_data = FRAME_CODE_NAK;
    memcpy(frame_out, &frame_data, sizeof(frame_data));
}

void ymodemCancelTransmitFrame(char* frame_out)
{
    memset(frame_out, FRAME_CODE_CAN, 5 * sizeof(FRAME_CODE_CAN));
}

void parseYModem128BytesTransmitFrame(const char* frame_in, char* data_out)
{
    if(!frame_in)
    {
        return;
    }

    YModem128BytesFrame frame;
    memcpy(&frame, frame_in, sizeof(YModem128BytesFrame));

    memcpy(data_out, &frame.data[0], sizeof(frame.data));
}

void parseYModem1024BytesTransmitFrame(const char* frame_in, char* data_out)
{
    if(!frame_in)
    {
        return;
    }

    YModem1024BytesFrame frame;
    memcpy(&frame, frame_in, sizeof(YModem1024BytesFrame));

    memcpy(data_out, &frame.data[0], sizeof(frame.data));
}

#endif // !(USE_YMODEM_G || USE_YMODEM_1K)