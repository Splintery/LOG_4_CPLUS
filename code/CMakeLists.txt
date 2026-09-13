cmake_minimum_required(VERSION 3.28)
project(LOG_4_CPLUS LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_compile_definitions(DEBUG_VERSION)

include_directories(
    inc/
    inc/logger
)

add_executable(
    main src/main.cpp
    inc/logger/LogLevel.hpp
    src/logger/Appender.cpp
    src/logger/Logger.cpp
)

target_compile_features(main PRIVATE cxx_std_17)