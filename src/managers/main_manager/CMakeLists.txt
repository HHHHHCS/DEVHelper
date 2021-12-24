add_library(main_manager
    main_manager.cpp
    main_manager.h
)

target_link_libraries(main_manager
PRIVATE
    dev_helper_src
)

target_include_directories(main_manager
INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)