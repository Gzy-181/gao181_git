cmake_minimum_required(VERSION 3.16)

project(weacon VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 Qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
# 如果使用 Qt6，查找 SerialPort
if(${QT_VERSION_MAJOR} EQUAL 6)
    find_package(Qt6 REQUIRED COMPONENTS SerialPort)
endif()

set(PROJECT_SOURCES
    main.cpp
    widget.cpp
    widget.h
    widget.ui
    res.cpp
    res.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    # 使用 qt_add_executable
    qt_add_executable(weacon
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(weacon SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(weacon
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# 链接库必须在 add_executable 之后
target_link_libraries(weacon PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
)

# 如果是 Qt6，链接 SerialPort
if(${QT_VERSION_MAJOR} EQUAL 6)
    target_link_libraries(weacon PRIVATE Qt6::SerialPort)
endif()

# 设置目标属性
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.weacon)
endif()

set_target_properties(weacon PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS weacon
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(weacon)
endif()
