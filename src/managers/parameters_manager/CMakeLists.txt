add_library(parameters_manager
    parameters_manager.cpp
    parameters_manager.h
)

target_link_libraries(parameters_manager
PRIVATE
    dev_helper_src
)

target_include_directories(parameters_manager
INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)