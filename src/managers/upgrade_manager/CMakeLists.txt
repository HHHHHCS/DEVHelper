add_library(upgrade_manager
    upgrade_manager.cpp
    upgrade_manager.h
)

target_link_libraries(upgrade_manager
PRIVATE
    dev_helper_src
)

target_include_directories(upgrade_manager
INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)