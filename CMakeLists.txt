cmake_minimum_required(VERSION 3.13.3)

project(madutils LANGUAGES CXX) 

find_package(Qt5Widgets)

set(madutils_header_files
    ${PROJECT_SOURCE_DIR}/include/mad/utils/scopedtrigger.hpp
    ${PROJECT_SOURCE_DIR}/include/mad/utils/ui/dialog.hpp)

set(madutils_source_files
    ${PROJECT_SOURCE_DIR}/src/scopedtrigger.cpp
    ${PROJECT_SOURCE_DIR}/src/ui/dialog.cpp)

add_library(madutils STATIC ${madutils_header_files} ${madutils_source_files})

target_include_directories(madutils
    PRIVATE ${PROJECT_SOURCE_DIR}/include/mad/utils
            ${Qt5Widgets_INCLUDE_DIRS}
    INTERFACE ${PROJECT_SOURCE_DIR}/include
              ${Qt5Widgets_INCLUDE_DIRS})

target_link_libraries(madutils PRIVATE Qt5::Widgets)
