cmake_minimum_required(VERSION 3.16)

project(ssdaemon LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    main.cpp
    ../eth.cpp ../eth.h
    ../ip.cpp ../ip.h
    ../mac.h
    ../sutrace.cpp ../sutrace.h
    ../capPacket.cpp ../capPacket.h
)

add_executable(ssdaemon ${SOURCES})

target_include_directories(ssdaemon PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../libpcap")

if(APPLE)
    target_link_libraries(ssdaemon PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../libpcap/build/Qt_6_10_2_for_macOS-Debug/libpcap.a")
elseif(ANDROID)
    target_link_libraries(ssdaemon PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../libpcap/build/Qt_6_10_2_for_Android_arm64_v8a-Debug/libpcap.a")
endif()

if(ANDROID)
    add_custom_command(TARGET ssdaemon POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/../android/assets
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:ssdaemon> ${CMAKE_CURRENT_SOURCE_DIR}/../android/assets/ssdaemon
    )
elseif(APPLE)
    add_custom_command(TARGET ssdaemon POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:ssdaemon> ${CMAKE_CURRENT_BINARY_DIR}/../simplePcap.app/Contents/MacOS/ssdaemon
    )
endif()
