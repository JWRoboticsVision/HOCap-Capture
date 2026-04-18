find_package(k4a QUIET)
find_package(RealSense2 QUIET)
option(BUILD_K4ARealSense2 "Build K4ARealSense2 Driver." ON)

if(k4a_FOUND AND RealSense2_FOUND AND BUILD_K4ARealSense2)

  message(STATUS "HAL: building 'K4ARealSense2' camera driver.")

  set(SOURCES
    K4ARealSense2Driver.h
    K4ARealSense2Driver.cpp
    K4ARealSense2Factory.cpp
  )

  add_to_hal_include_dirs(${K4ARealSense2_INCLUDE_DIRS})
  add_to_hal_libraries(${K4ARealSense2_LIBRARIES})
  add_to_hal_sources(${SOURCES})

endif()