cmake_minimum_required(VERSION 3.0.0)
project(BSVisionTest VERSION 0.1.0)

#打开我的电脑>属性>高级系统设置>环境变量
#新建用户变量 变量名"QT5.12.5"，变量值"qt5.12.5下编译器的目录，比如我的是'C:\Qt\Qt5.12.5\5.12.5\msvc2017_64'"
#同理 新建用户变量 变量名"OrionLib"，变量值"OrionLib所在目录，比如我的是在'C:\OrionLib'"
#确定保存即可，cmake访问系统变量 $ENV{变量名},如:访问上述变量'QT5.12.5变量' $ENV{QT5.12.5}
#添加用户变量后需重启

SET(CMAKE_PREFIX_PATH $ENV{QT5.12.5})
include_directories(${PROJECT_SOURCE_DIR})
include_directories($ENV{QT5.12.5}\\include)
include_directories($ENV{QT5.12.5}\\include\\QtGui)
include_directories($ENV{QT5.12.5}\\include\\QtWidgets)
include_directories($ENV{QT5.12.5}\\include\\QtCore)
include_directories($ENV{QT5.12.5}\\include\\QtNetwork)
include_directories($ENV{QT5.12.5}\\include\\QtSerialBus)
include_directories($ENV{QT5.12.5}\\include\\QtSerialPort)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

message("Looking for Qt...")
find_package(Qt5 COMPONENTS Core Gui Network SerialBus Widgets Sql Xlsx SerialPort Qml REQUIRED)
if (${Qt5_FOUND})
    message("Found Qt " ${Qt5_VERSION})
else()
    message(FATAL_ERROR "Couldn't find Qt")
endif()
execute_process(
    COMMAND chcp 65001
    )

set(Qt5model
    Qt5::Widgets
    Qt5::Network
    Qt5::SerialBus
    Qt5::Gui
    Qt5::SerialPort
)

include_directories(include/halcon12x64 include/halcon12x64/halconcpp)

add_subdirectory(${PROJECT_SOURCE_DIR}/src/hal)

file(GLOB LIBS "lib/halcon12x64/*.lib")
set(GLOB uiFiles
    "${PROJECT_SOURCE_DIR}/src/ui/*.ui"
    "${PROJECT_SOURCE_DIR}/src/ui/EngDocument/*.ui"
    "${PROJECT_SOURCE_DIR}/src/ui/SwitchUsers/*.ui"
    "${PROJECT_SOURCE_DIR}/src/ui/Camera/*.ui"
    "${PROJECT_SOURCE_DIR}/src/ui/LightSource/*.ui"
    "${PROJECT_SOURCE_DIR}/src/ui/StationSet/*.ui"
    "${PROJECT_SOURCE_DIR}/src/ui/StationSet/roiTemplate/*.ui")


set(uiCpp
    src/ui/Mainscene.cpp
    src/ui/MyThread.cpp
    src/ui/MyThread_Discern.cpp
    src/ui/MyThread_DisOneCircle.cpp
    src/ui/StationSet/halconshare.cpp
    src/ui/StationSet/StationSet.cpp
    src/ui/StationSet/StationAttribute.cpp
    src/ui/StationSet/ModelTool.cpp
    src/ui/StationSet/MyThread_Model.cpp
    src/ui/StationSet/roiTemplate/ROITool.cpp
    src/ui/StationSet/roiTemplate/roiDraw.cpp
    src/ui/EngDocument/EngDocument.cpp
    src/ui/SwitchUsers/SwitchUsers.cpp
    src/ui/CameraPara/CameraParams.cpp
    src/ui/LightSource/LightSet.cpp)

set(sources
        ${uiFiles}
        ${uiCpp}
        main.cpp
)

qt5_add_resources(sources qml.qrc)

add_executable(${PROJECT_NAME}
    # WIN32 # If you need a terminal for debug, please comment this statement
    ${sources}
)
target_link_libraries(${PROJECT_NAME}
    ${Qt5model}
    ${LIBS}
    halLib ) # Qt5 Shared Library

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)