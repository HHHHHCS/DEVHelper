add_subdirectory(link_manager)
add_subdirectory(main_manager)
add_subdirectory(parameters_manager)
add_subdirectory(upgrade_manager)

add_library(managers
    manager_collection.cpp
    manager_collection.h
)

target_link_libraries(managers
PUBLIC
    Qt5::Core
    Qt5::Qml
    Qt5::Widgets

    link_manager
    main_manager
    parameters_manager
    upgrade_manager
)

target_include_directories(managers
INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)