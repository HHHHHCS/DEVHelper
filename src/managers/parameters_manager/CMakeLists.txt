add_library(parameters_manager
    parameters_manager.cpp
    parameters_manager.h
    aw_dev_params_def.h
)

target_link_libraries(parameters_manager
INTERFACE
    tasks
PRIVATE
    managers
)

target_include_directories(parameters_manager
INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)