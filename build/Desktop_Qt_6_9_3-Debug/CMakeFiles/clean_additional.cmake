# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/SoundRelay_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/SoundRelay_autogen.dir/ParseCache.txt"
  "SoundRelay_autogen"
  )
endif()
