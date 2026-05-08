cmake_minimum_required(VERSION 3.20)
project(WinFspDemo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# WinFsp detection
if(NOT DEFINED WINFSP_DIR)
    if(DEFINED ENV{WINFSP_DIR})
        set(WINFSP_DIR "$ENV{WINFSP_DIR}")
    else()
        set(WINFSP_DIR "C:/Program Files (x86)/WinFsp")
    endif()
endif()

message(STATUS "Using WinFsp from: ${WINFSP_DIR}")

include_directories("${WINFSP_DIR}/inc")
link_directories("${WINFSP_DIR}/lib")

add_executable(WinFspDemo src/main.cpp src/DemoFileSystem.cpp)

# Link against WinFsp library
# Note: Usually it's winfsp-x64.lib for 64-bit builds
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    target_link_libraries(WinFspDemo winfsp-x64)
else()
    target_link_libraries(WinFspDemo winfsp-x86)
endif()

# Output settings
set_target_properties(WinFspDemo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Auto-copy WinFsp DLL to output directory
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(WINFSP_DLL "${WINFSP_DIR}/bin/winfsp-x64.dll")
else()
    set(WINFSP_DLL "${WINFSP_DIR}/bin/winfsp-x86.dll")
endif()

add_custom_command(TARGET WinFspDemo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${WINFSP_DLL}"
    $<TARGET_FILE_DIR:WinFspDemo>
)
