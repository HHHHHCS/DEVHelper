include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}

    lib/communication/serial/q_serial
)

add_subdirectory(lib)
add_subdirectory(managers)

add_library(dev_helper_src
    dev_helper_application.cpp
    dev_helper_application.h
    main.cpp
)

target_link_libraries(dev_helper_src
PUBLIC
    Qt5::Core
    Qt5::Qml
    Qt5::Widgets

    serial_lib

    managers
    # ui_lib
)

target_include_directories(dev_helper_src
INTERFACE
    ${CMKAE_CURRENT_SOURCE_DIR}
)