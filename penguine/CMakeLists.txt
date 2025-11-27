cmake_minimum_required(VERSION 3.15)
    project(penguine_c LANGUAGES C)
    
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_C_STANDARD_REQUIRED ON)


    file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/*.c")

    add_library(penguine_c
        ${SRCS}
    )

    target_link_libraries(penguine_c
        opengl32
    )


    target_include_directories(penguine_c PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")


    if(MSVC)
        target_compile_options(penguine_c PRIVATE /W4)
    else()
        target_compile_options(penguine_c PRIVATE -Wall -Wextra -Wpedantic)
    endif()
    
    set(CMAKE_BUILD_TYPE Debug)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_definitions(penguine_c PRIVATE DEBUG)
        if(NOT MSVC)
            target_compile_options(penguine_c PRIVATE -g -O0)
        endif()
    endif()
    
    enable_testing()