#pragma once


#include <cstdint>

#ifdef __cplusplus
namespace aw_parameters_define {
#endif // __cplusplus

    typedef struct __attribute__((packed))
    {

    } UAVParams;

    typedef struct __attribute__((packed))
    {

    } UAPParams;

    typedef struct __attribute__((packed))
    {

    } UGVParams;

    typedef struct __attribute__((packed))
    {

    } PX4Params;

    typedef struct __attribute__((packed))
    {
        uint32_t id;

        uint32_t log_max_num : 16;
        uint32_t default_language : 8;

        uint32_t default_dtu_type : 8;

        uint32_t used_gps : 4;
        uint32_t outside_gps_baudrate;
        uint32_t inside_gps_baudrate;

        uint32_t bat_maxium_vol : 16;
        uint32_t bat_minium_vol : 16;
    } RCXParams;

    typedef struct __attribute__((packed))
    {

    } RPXParams;

    typedef struct __attribute__((packed))
    {

    } BAXParams;

#ifdef __cplusplus
} // namespace parameters_define
#endif // __cplusplus