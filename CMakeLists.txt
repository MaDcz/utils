cmake_minimum_required(VERSION 3.13.3)

project(madutils LANGUAGES CXX) 

find_package(Qt5Widgets)

set(CMAKE_AUTOMOC ON)

set(madutils_header_files
    ${PROJECT_SOURCE_DIR}/include/mad/utils/scopedtrigger.hpp
    ${PROJECT_SOURCE_DIR}/include/mad/utils/gui/dialog.hpp
    ${PROJECT_SOURCE_DIR}/include/mad/utils/gui/events/event.hpp
    ${PROJECT_SOURCE_DIR}/include/mad/utils/gui/events/inputevent.hpp
    ${PROJECT_SOURCE_DIR}/include/mad/utils/gui/events/mouseevent.hpp
    ${PROJECT_SOURCE_DIR}/include/mad/utils/gui/events/mousewheelevent.hpp
    ${PROJECT_SOURCE_DIR}/include/mad/utils/gui/features/cursorguidelinesfeature.hpp
    ${PROJECT_SOURCE_DIR}/include/mad/utils/gui/features/feature.hpp
    ${PROJECT_SOURCE_DIR}/include/mad/utils/gui/features/featuresawarewidget.hpp
    ${PROJECT_SOURCE_DIR}/include/mad/utils/gui/features/selectionboxfeature.hpp
    ${PROJECT_SOURCE_DIR}/include/mad/utils/gui/features/zoomfeature.hpp)

set(madutils_source_files
    ${PROJECT_SOURCE_DIR}/src/scopedtrigger.cpp
    ${PROJECT_SOURCE_DIR}/src/gui/dialog.cpp
    ${PROJECT_SOURCE_DIR}/src/gui/events/event.cpp
    ${PROJECT_SOURCE_DIR}/src/gui/events/mouseevent.cpp
    ${PROJECT_SOURCE_DIR}/src/gui/events/mousewheelevent.cpp
    ${PROJECT_SOURCE_DIR}/src/gui/features/cursorguidelinesfeature.cpp
    ${PROJECT_SOURCE_DIR}/src/gui/features/feature.cpp
    ${PROJECT_SOURCE_DIR}/src/gui/features/selectionboxfeature.cpp
    ${PROJECT_SOURCE_DIR}/src/gui/features/zoomfeature.cpp)

add_library(madutils STATIC ${madutils_header_files} ${madutils_source_files})

target_include_directories(madutils
    PRIVATE ${PROJECT_SOURCE_DIR}/include/mad/utils
            ${Qt5Widgets_INCLUDE_DIRS}
    INTERFACE ${PROJECT_SOURCE_DIR}/include
              ${Qt5Widgets_INCLUDE_DIRS})

target_link_libraries(madutils PRIVATE Qt5::Widgets)
