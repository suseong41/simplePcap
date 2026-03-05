# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/simplePcap_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/simplePcap_autogen.dir/ParseCache.txt"
  "simplePcap_autogen"
  )
endif()
