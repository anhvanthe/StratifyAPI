

set(SRC_SOURCES_PREFIX ${SOURCES_PREFIX})

if( ${SOS_BUILD_CONFIG} STREQUAL arm )
  set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/draw)
  add_subdirectory(draw)
  list(APPEND SOURCELIST ${SOURCES})

	set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/dsp)
	add_subdirectory(dsp)
	list(APPEND SOURCELIST ${SOURCES})

  set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/sgfx)
  add_subdirectory(sgfx)
  list(APPEND SOURCELIST ${SOURCES})

  set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/ui)
  add_subdirectory(ui)
  list(APPEND SOURCELIST ${SOURCES})

	set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/test)
	add_subdirectory(test)
	list(APPEND SOURCELIST ${SOURCES})

endif()

set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/api)
add_subdirectory(api)
list(APPEND SOURCELIST ${SOURCES})

set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/chrono)
add_subdirectory(chrono)
list(APPEND SOURCELIST ${SOURCES})

set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/ev)
add_subdirectory(ev)
list(APPEND SOURCELIST ${SOURCES})

set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/sm)
add_subdirectory(sm)
list(APPEND SOURCELIST ${SOURCES})

set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/calc)
add_subdirectory(calc)
list(APPEND SOURCELIST ${SOURCES})

set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/fmt)
add_subdirectory(fmt)
list(APPEND SOURCELIST ${SOURCES})

set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/hal)
add_subdirectory(hal)
list(APPEND SOURCELIST ${SOURCES})

set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/sys)
add_subdirectory(sys)
list(APPEND SOURCELIST ${SOURCES})

set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/var)
add_subdirectory(var)
list(APPEND SOURCELIST ${SOURCES})

set(SOURCES ${SOURCELIST} PARENT_SCOPE)
