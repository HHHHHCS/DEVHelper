#ifndef __YMODEM_H__
#define __YMODEM_H__

#include "stdint.h"


// TODO(huangchsh): 后续增加升级协议配置文件
#ifndef USE_YMODEM_1K
#define USE_YMODEM_1K
#endif // !USE_YMODEM_1K

#ifndef YMODEM_WAIT_CHR_TICK
#define YMODEM_WAIT_CHR_TICK    3000
#endif // !YMODEM_WAIT_CHR_TICK

#ifndef YMODEM_WAIT_PKG_TICK
#define YMODEM_WAIT_PKG_TICK    3000
#endif // !YMODEM_WAIT_PKG_TICK

#ifndef YMODEM_CHD_INTV_TICK
#define YMODEM_CHD_INTV_TICK    3000
#endif // !YMODEM_CHD_INTV_TICK

#ifndef YMODEM_END_SESSION_SEND_CAN_NUM
#define YMODEM_END_SESSION_SEND_CAN_NUM 0x07
#endif // !YMODEM_END_SESSION_SEND_CAN_NUM

/**
 * @brief 帧头枚举
 *
 */
typedef enum
{
    FRAME_CODE_NONE = 0x00,
    FRAME_CODE_SOH  = 0x01,
    FRAME_CODE_STX  = 0x02,
    FRAME_CODE_EOT  = 0x04,
    FRAME_CODE_ACK  = 0x06,
    FRAME_CODE_NAK  = 0x15,
    FRAME_CODE_CAN  = 0x18,
    FRAME_CODE_C    = 0x43,
}YModemFrameCodeType;

/**
 * @brief 128Bytes传输帧
 *
*/
#pragma pack(push, 1)
typedef struct
{
    YModemFrameCodeType      code_type;
    uint8_t                  seq;
    uint8_t                  seq_inverse;
    char                     data[128];
    uint16_t                 crc_16;
} YModem128BytesFrame;

/**
 * @brief 1024Bytes传输帧
 *
 */
typedef struct
{
    YModemFrameCodeType      code_type;
    uint8_t                  seq;
    uint8_t                  seq_inverse;
    char                     data[1024];
    uint16_t                 crc_16;
} YModem1024BytesFrame;
#pragma pack(pop)

extern void ymodemBeginFrame(const char *file_name, const size_t file_name_size, uint32_t file_size, char *frame_out);
extern void ymodemEndtFrame(char *frame_out);
extern void ymodemTransmitFrame(const char* data, size_t data_size, char* frame_out);
extern void ymodemHandshake(char* frame_out);
extern void ymodemTransmitEndFrame(char* frame_out);
extern void ymodemTransmitAckFrame(char* frame_out);
extern void ymodemTransmitNAckFrame(char* frame_out);
extern void ymodemCancelTransmitFrame(char* frame_out);

extern void parseYModem128BytesTransmitFrame(const char* frame_in, char* data_out);
extern void parseYModem1024BytesTransmitFrame(const char* frame_in, char* data_out);

extern inline uint8_t checkYModemHandshake(const uint8_t code) { return (code == FRAME_CODE_C ? 1 : 0); }
extern inline uint8_t checkYModemAck(const uint8_t code) { return (code == FRAME_CODE_ACK ? 1 : 0); }
extern inline uint8_t checkYModemNAck(const uint8_t code) { return (code == FRAME_CODE_NAK ? 1 : 0); }
extern inline uint8_t checkYModemEot(const uint8_t code) { return (code == FRAME_CODE_EOT ? 1 : 0); }
extern inline uint8_t checkYModemCan(const uint8_t code) { return (code == FRAME_CODE_CAN ? 1 : 0); }

#endif // !__YMODEM_H__