set(QSERIAL_DIR q_serial)

add_library(serial_lib
    ${QSERIAL_DIR}/q_serial.cpp
    ${QSERIAL_DIR}/q_serial.h
)

target_link_libraries(serial_lib
PRIVATE
    Qt5::SerialBus
    Qt5::SerialPort
)

target_include_directories(serial_lib
INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)