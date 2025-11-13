cmake_minimum_required(VERSION 3.15)
    project(penguine_c LANGUAGES C)
    
    # Set C standard
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_C_STANDARD_REQUIRED ON)

    if(EMSCRIPTEN)
        set(CMAKE_C_COMPILER "emcc")
    endif()
    
    # Find vcpkg packages if available
    find_package(PkgConfig QUIET)
    
    file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.c")
    # Add executable
    add_executable(penguine_c 
        ${SRCS}
    )

    if(EMSCRIPTEN)
        target_link_options(penguine_c PRIVATE
            -sWASM=1
            -sUSE_WEBGL2=1
            -sALLOW_MEMORY_GROWTH=1
            -sEXPORTED_RUNTIME_METHODS=ccall,cwrap
        )
        set_target_properties(penguine_c PROPERTIES SUFFIX ".js")
    else()
        target_link_libraries(penguine_c
            opengl32
        )
    endif()


    target_include_directories(penguine_c PRIVATE "${CMAKE_SOURCE_DIR}/src")


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
        