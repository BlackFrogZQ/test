cmake_minimum_required(VERSION 3.5) # CMake install : https://cmake.org/download/
project(Mainscene LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "C:/software/Qt/install/5.12.5/msvc2017_64") # Qt Kit Dir
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt5 COMPONENTS Widgets REQUIRED Core Gui Serialport) # Qt COMPONENTS

include_directories(include/halcon12x64 include/halcon12x64/halconcpp)
file(GLOB libs "lib/halcon12x64/*.lib")

set(srcCpp
    src/Mainscene.cpp
    src/MyThread.cpp
    src/MyThread_Discern.cpp
    src/MyThread_DisOneCircle.cpp
    src/StationSet/halconshare.cpp
    src/StationSet/StationSet.cpp
    src/StationSet/StationAttribute.cpp
    src/StationSet/MultipleNum.cpp
    src/StationSet/ModelTool.cpp
    src/StationSet/MyThread_Model.cpp
    src/StationSet/MyThread_ROI.cpp
    src/StationSet/ROITool.cpp
    src/EngDocument/EngDocument.cpp
    src/SwitchUsers/SwitchUsers.cpp
    src/Camera/CameraParams.cpp
    src/LightSource/LightSet.cpp)

# Specify MSVC UTF-8 encoding   
# add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
# add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_executable(${PROJECT_NAME}
    # WIN32 # If you need a terminal for debug, please comment this statement 
    ${srcCpp} 
    main.cpp 
) 
target_link_libraries(${PROJECT_NAME} PRIVATE ${libs} Qt5::Widgets Qt5::Core Qt5::Gui Qt5::SerialPort) # Qt5 Shared Library