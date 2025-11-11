cmake_minimum_required(VERSION 3.15)
    project(penguine_c LANGUAGES C)
    
    # Set C standard
    set(CMAKE_C_STANDARD 17)
    set(CMAKE_C_STANDARD_REQUIRED ON)
    
    # Find vcpkg packages if available
    find_package(PkgConfig QUIET)
    
    file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.c")

    # Add executable
    add_executable(penguine_c 
        ${SRCS}
    )
    
    # Compiler-specific options
    if(MSVC)
        target_compile_options(penguine_c PRIVATE /W4)
    else()
        target_compile_options(penguine_c PRIVATE -Wall -Wextra -Wpedantic)
    endif()
    
    # Debug configuration
    set(CMAKE_BUILD_TYPE Debug)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_definitions(penguine_c PRIVATE DEBUG)
        if(NOT MSVC)
            target_compile_options(penguine_c PRIVATE -g -O0)
        endif()
    endif()
    
    # Enable testing
    enable_testing()
    
    # Install target
    #install(TARGETS penguine_c  RUNTIME DESTINATION bin)
        