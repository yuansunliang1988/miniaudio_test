cmake_minimum_required(VERSION 3.16)
project(MiniaudioCrossPlatform)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)

# 添加源文件
add_executable(miniaudio_example main.cpp)

# 平台相关设置
if(WIN32)
    # Windows 特定设置
    target_link_libraries(miniaudio_example winmm ole32)
elseif(APPLE)
    # macOS 特定设置
    set(MACOSX_BUNDLE TRUE)
    set_target_properties(miniaudio_example PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/Info.plist
    )
elseif(UNIX)
    # Linux 特定设置
    target_link_libraries(miniaudio_example pthread dl)
endif()
