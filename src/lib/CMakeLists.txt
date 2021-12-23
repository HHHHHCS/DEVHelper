find_package(Qt5
REQUIRED COMPONENTS
  Core
  SerialBus
  SerialPort
)

aux_source_directory(./serial SERIAL_SRC_LIST)

add_library(dev_helper_lib STATIC
    ${SERIAL_SRC_LIST}
)

target_link_libraries(dev_helper_lib
PRIVATE
  Qt5::Core
  Qt5::SerialBus
  Qt5::SerialPort
)