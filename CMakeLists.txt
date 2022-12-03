cmake_minimum_required(VERSION 3.14)

project(HttpToWebSocketGateway LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(./src)

#set(INC_DIR "D:/Qt/Tools/OpenSSL/Win_x64/include/openssl") #“”为第三方库头文件路径
#set(LINK_DIR "D:/Qt/Tools/OpenSSL/Win_x64/bin") #为“”第三方库库文件路径（.so文件）
#include_directories(${INC_DIR})
#link_directories(${LINK_DIR})


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core WebSockets)
find_package(Qt6 REQUIRED COMPONENTS HttpServer)


add_executable(HttpToWebSocketGateway
  src/config.json
  src/config.default.json
  src/config.h
  src/websocketclient.h
  src/simplehttpserver.h
  src/websocketclient.cpp
  src/simplehttpserver.cpp
  src/config.cpp
  src/main.cpp
)
target_link_libraries(HttpToWebSocketGateway
    Qt${QT_VERSION_MAJOR}::Core
    Qt::WebSockets
    Qt::HttpServer
#    "libssl-1_1-x64.dll"
 #   "libcrypto-1_1-x64.dll"
)

install(TARGETS HttpToWebSocketGateway
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
