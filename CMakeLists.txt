cmake_minimum_required(VERSION 3.15)
    project(a_game LANGUAGES C)
    
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_C_STANDARD_REQUIRED ON)


    add_executable(a_game
        "${CMAKE_SOURCE_DIR}/src/main.c"
    )

    add_subdirectory("${CMAKE_SOURCE_DIR}/penguine")
    
    target_link_libraries(a_game
        opengl32
        penguine_c
    )    



    if(MSVC)
        target_compile_options(a_game PRIVATE /W4)
    else()
        target_compile_options(a_game PRIVATE -Wall -Wextra -Wpedantic)
    endif()
    
    set(CMAKE_BUILD_TYPE Debug)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_definitions(a_game PRIVATE DEBUG)
        if(NOT MSVC)
            target_compile_options(a_game PRIVATE -g -O0)
        endif()
    endif()
    
    enable_testing()
    
    #install(TARGETS a_game  RUNTIME DESTINATION bin)
        