# =============================================================================
# © You i Labs Inc. 2000-2020. All rights reserved.

file(GLOB_RECURSE YI_PROJECT_SOURCE "${_INCLUDE_DIR}/*.m" "${_INCLUDE_DIR}/*.mm" "${_INCLUDE_DIR}/*.cpp")
file(GLOB_RECURSE YI_PROJECT_HEADERS "${_INCLUDE_DIR}/*.h")

set(PLATFORMS android ios osx tvos)
list(REMOVE_ITEM PLATFORMS ${YI_PLATFORM_LOWER})

foreach(PLATFORM ${PLATFORMS})
    foreach(YI_SRC_FILE ${YI_PROJECT_SOURCE})
        if("${YI_SRC_FILE}" MATCHES "(.*)/${PLATFORM}/(.*)")
            if(${CMAKE_VERBOSE_MAKEFILE})
                message(STATUS "removing ${YI_SRC_FILE}...")
            endif()
            list(REMOVE_ITEM YI_PROJECT_SOURCE ${YI_SRC_FILE})
        endif()
    endforeach(YI_SRC_FILE)
endforeach(PLATFORM)

foreach(YI_HDR_FILE ${YI_PROJECT_HEADERS})
    if("${YI_HDR_FILE}" MATCHES "(.*)/${PLATFORM}/(.*)")
        if(${CMAKE_VERBOSE_MAKEFILE})
            message(STATUS "removing ${YI_HDR_FILE}...")
        endif()
        list(REMOVE_ITEM YI_PROJECT_HEADERS ${YI_HDR_FILE})
    endif()
endforeach(YI_HDR_FILE)

source_group(App                    REGULAR_EXPRESSION "app/.*")
source_group(Player                 REGULAR_EXPRESSION "player/.*")
source_group(NativeModule           REGULAR_EXPRESSION "nativemodule/.*")