find_package(k4a QUIET)
option(BUILD_K4A "Build K4A Driver." ON)

if(k4a_FOUND AND BUILD_K4A)

  message(STATUS "HAL: building 'K4A' camera driver.")

  set(SOURCES
    K4ADriver.h
    K4ADriver.cpp
    K4AFactory.cpp
  )

  add_to_hal_include_dirs(${k4a_INCLUDE_DIRS})
  add_to_hal_libraries(${k4a_LIBRARIES})
  add_to_hal_sources(${SOURCES})

endif()