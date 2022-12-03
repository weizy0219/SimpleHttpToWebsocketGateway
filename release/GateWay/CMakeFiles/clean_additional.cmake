# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\HttpToWebSocketGateway_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\HttpToWebSocketGateway_autogen.dir\\ParseCache.txt"
  "HttpToWebSocketGateway_autogen"
  )
endif()
