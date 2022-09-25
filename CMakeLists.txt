cmake_minimum_required(VERSION 3.23)
project(binance VERSION 1.0.0 DESCRIPTION "binance library")
set(CMAKE_CXX_STANDARD 17)
include(GNUInstallDirs)
find_package(PkgConfig)
pkg_check_modules(LIB_WEBSOCKETS REQUIRED libwebsockets)
pkg_check_modules(LIB_CURL REQUIRED libcurl)
set(CMAKE_INSTALL_DEFAULT_DIRECTORY_PERMISSIONS
        OWNER_READ
        OWNER_WRITE
        OWNER_EXECUTE
        GROUP_READ
        )
get_filename_component(
        LIB_WEBSOCKETS_INSTALL_DIR
        ${LIB_WEBSOCKETS_LIBRARY_DIRS}
        DIRECTORY
)
get_filename_component(
        LIB_CURL_INSTALL_DIR
        ${LIB_CURL_LIBRARY_DIRS}
        DIRECTORY
)
add_library(binance SHARED src/binance_websocket.cpp include/binance_websocket.h src/binance_logger.cpp include/binance_logger.h src/binance.cpp include/binance.h src/binance_utils.cpp include/binance_utils.h)
target_link_libraries(
        binance
        ${LIB_WEBSOCKETS_LIBRARIES}
        ${LIB_CURL_LIBRARIES}
)
set_target_properties(binance PROPERTIES
        VERSION 1.0.1
        SOVERSION 1
        PUBLIC_HEADER api/binance.h)
configure_file(binance.pc.in binance.pc @ONLY)
target_include_directories(binance PRIVATE ./src INTERFACE ./include)
install(TARGETS binance
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/binance.pc
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig
        PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
        )



