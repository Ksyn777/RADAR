# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Radar_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Radar_autogen.dir\\ParseCache.txt"
  "Radar_autogen"
  )
endif()
