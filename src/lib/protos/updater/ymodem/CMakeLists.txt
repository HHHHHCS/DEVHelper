add_library(ymodem
    ymodem.c
    ymodem.h
)

target_include_directories(ymodem
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)