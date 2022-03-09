include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
)

add_subdirectory(lib)
add_subdirectory(managers)

add_library(dev_helper_src STATIC
    dev_helper_application.cpp
    dev_helper_application.h
    main.cpp
)

target_link_libraries(dev_helper_src
PUBLIC
    managers
)

target_include_directories(dev_helper_src
INTERFACE
    ${CMKAE_CURRENT_SOURCE_DIR}
)