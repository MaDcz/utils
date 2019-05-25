cmake_minimum_required(VERSION 3.13.3)

project(madutils LANGUAGES CXX) 

find_package(Qt5Widgets)

set(CMAKE_AUTOMOC ON)

set(madutils_header_files
    ${PROJECT_SOURCE_DIR}/include/mad/utils/ui/dialog.hpp
    ${PROJECT_SOURCE_DIR}/include/mad/utils/ui/dialogbuttonboxfactory.hpp
    ${PROJECT_SOURCE_DIR}/include/mad/utils/ui/events/event.hpp
    ${PROJECT_SOURCE_DIR}/include/mad/utils/ui/events/inputevent.hpp
    ${PROJECT_SOURCE_DIR}/include/mad/utils/ui/events/mouseevent.hpp
    ${PROJECT_SOURCE_DIR}/include/mad/utils/ui/events/mousewheelevent.hpp
    ${PROJECT_SOURCE_DIR}/include/mad/utils/ui/events/resizeevent.hpp
    ${PROJECT_SOURCE_DIR}/include/mad/utils/ui/features/cursorguidelinesfeature.hpp
    ${PROJECT_SOURCE_DIR}/include/mad/utils/ui/features/feature.hpp
    ${PROJECT_SOURCE_DIR}/include/mad/utils/ui/features/featuresawareqwidget.hpp
    ${PROJECT_SOURCE_DIR}/include/mad/utils/ui/features/selectionboxfeature.hpp
    ${PROJECT_SOURCE_DIR}/include/mad/utils/ui/features/zoomfeature.hpp
    ${PROJECT_SOURCE_DIR}/include/mad/utils/ui/filebrowsewidgetfactory.hpp
    ${PROJECT_SOURCE_DIR}/include/mad/utils/ui/layoutarranger.hpp
    ${PROJECT_SOURCE_DIR}/include/mad/utils/ui/singlepartwidgetfactory.hpp
    ${PROJECT_SOURCE_DIR}/include/mad/utils/ui/widgetpartscollection.hpp
    ${PROJECT_SOURCE_DIR}/include/mad/utils/ui/widgetfactory.hpp
    ${PROJECT_SOURCE_DIR}/include/mad/utils/flags.hpp
    ${PROJECT_SOURCE_DIR}/include/mad/utils/scopedtrigger.hpp)

set(madutils_source_files
    ${PROJECT_SOURCE_DIR}/src/ui/dialog.cpp
    ${PROJECT_SOURCE_DIR}/src/ui/dialogbuttonboxfactory.cpp
    ${PROJECT_SOURCE_DIR}/src/ui/events/event.cpp
    ${PROJECT_SOURCE_DIR}/src/ui/events/mouseevent.cpp
    ${PROJECT_SOURCE_DIR}/src/ui/events/mousewheelevent.cpp
    ${PROJECT_SOURCE_DIR}/src/ui/events/resizeevent.cpp
    ${PROJECT_SOURCE_DIR}/src/ui/features/cursorguidelinesfeature.cpp
    ${PROJECT_SOURCE_DIR}/src/ui/features/feature.cpp
    ${PROJECT_SOURCE_DIR}/src/ui/features/selectionboxfeature.cpp
    ${PROJECT_SOURCE_DIR}/src/ui/features/zoomfeature.cpp
    ${PROJECT_SOURCE_DIR}/src/ui/filebrowsewidgetfactory.cpp
    ${PROJECT_SOURCE_DIR}/src/ui/layoutarranger.cpp
    ${PROJECT_SOURCE_DIR}/src/scopedtrigger.cpp)

add_library(madutils STATIC ${madutils_header_files} ${madutils_source_files})

target_include_directories(madutils
    PRIVATE ${PROJECT_SOURCE_DIR}/include/mad/utils
            ${Qt5Widgets_INCLUDE_DIRS}
    INTERFACE ${PROJECT_SOURCE_DIR}/include
              ${Qt5Widgets_INCLUDE_DIRS})

target_link_libraries(madutils PRIVATE Qt5::Widgets)
