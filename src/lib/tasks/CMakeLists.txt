add_library(tasks STATIC
    task.hpp
    tasks_queue.hpp
)

target_link_libraries(tasks
INTERFACE
    ${Boost_LIBRARIES}

    util_lib
)

target_include_directories(tasks
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)