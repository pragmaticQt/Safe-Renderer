cmake_minimum_required(VERSION 2.8.12)

project("engine")

message(STATUS "Start process project ${PROJECT_NAME}")

set(ENGINE_PROJECT_NAME ${PROJECT_NAME})

if(NOT DEFINED LSRROOT)
    set(LSRROOT "${CMAKE_CURRENT_SOURCE_DIR}/..")
    message(STATUS "--> LSRROOT: ${LSRROOT}")
endif()

if(NOT DEFINED LSRENGINE)
    set(LSRENGINE ${CMAKE_CURRENT_SOURCE_DIR})
    message(STATUS "--> LSRENGINE: ${LSRENGINE}")
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR};${LSRROOT}/cmake;${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

if(NOT DEFINED LSR_INITED)
    include(Init)
endif()
include(EnginePlatformSpecific)

if(NOT DEFINED ENGINE_PREFIX)
    set(ENGINE_PREFIX "engine")
endif()

add_subdirectory(common)
add_subdirectory(database)
add_subdirectory(display)
add_subdirectory(lsr)
add_subdirectory(framehandler)

add_library(${PROJECT_NAME} STATIC
    $<TARGET_OBJECTS:common>
    $<TARGET_OBJECTS:database>
    $<TARGET_OBJECTS:display>
    $<TARGET_OBJECTS:framehandler>
    $<TARGET_OBJECTS:lsr>
)

target_link_libraries(${PROJECT_NAME} ${SYSTEM_LINK_LIBS})

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${ENGINE_PREFIX}
    LIBRARY DESTINATION ${ENGINE_PREFIX}
    ARCHIVE DESTINATION ${ENGINE_PREFIX}
)

include(CPack)
