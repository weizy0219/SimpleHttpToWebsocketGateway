cmake_minimum_required(VERSION 3.14)

project(HttpToWebSocketGateway LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core WebSockets)
find_package(Qt6 REQUIRED COMPONENTS HttpServer)


add_executable(HttpToWebSocketGateway
  config.json config.default.json config.h websocketclient.h simplehttpserver.h  websocketclient.cpp simplehttpserver.cpp config.cpp main.cpp
)
target_link_libraries(HttpToWebSocketGateway
    Qt${QT_VERSION_MAJOR}::Core
    Qt::WebSockets
    Qt::HttpServer
)

install(TARGETS HttpToWebSocketGateway
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
