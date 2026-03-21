# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\weacon_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\weacon_autogen.dir\\ParseCache.txt"
  "weacon_autogen"
  )
endif()
