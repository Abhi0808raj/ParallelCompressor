cmake_minimum_required(VERSION 3.16)
project(ParallelCompressor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6 Widgets
find_package(Qt6 COMPONENTS Widgets REQUIRED)

# Find Zstd (Zstandard)
find_package(PkgConfig REQUIRED)
pkg_check_modules(ZSTD REQUIRED IMPORTED_TARGET libzstd)

# Source files
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/CompressionWorker.cpp
    src/ThreadSafeQueue.cpp 
)

set(HEADERS
    src/MainWindow.h
    src/CompressionWorker.h
    src/ThreadSafeQueue.h
)

# Add executable
add_executable(ParallelCompressor ${SOURCES} ${HEADERS})


# Include directories (if you have extra includes)
target_include_directories(ParallelCompressor PRIVATE src)

# Link Qt and Zstd
target_link_libraries(ParallelCompressor
    Qt6::Widgets
    PkgConfig::ZSTD
    pthread
)

message(STATUS "Zstd include dirs: ${ZSTD_INCLUDE_DIRS}")
message(STATUS "Zstd libraries: ${ZSTD_LIBRARIES}")
