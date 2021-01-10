message(STATUS "")
message(STATUS "@kaltura-player/video-component")
message(STATUS "Running: ${CMAKE_CURRENT_LIST_DIR}/ModuleList.cmake")

set(_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/youi/src")

file(GLOB _headers1 "${_MODULE_PATH}/nativemodule/*.h" "${_MODULE_PATH}/pk/*.h")
file(GLOB _headers2 "${_MODULE_PATH}/player/ios/*.h")
set(_MODULE_HEADERS ${_headers1} ${_headers2})

file(GLOB _source1 "${_MODULE_PATH}/nativemodule/*.cpp" "${_MODULE_PATH}/player/*.cpp")
file(GLOB _source2 "${_MODULE_PATH}/player/ios/*.m" "${_MODULE_PATH}/player/ios/*.mm")
set(_MODULE_SOURCE ${_source1} ${_source2})
#set(KALTURA_PLAYER_ANDROID_PATH ${CMAKE_CURRENT_LIST_DIR})     

# Make sure the specific platform include is processed
if(IOS)
    include(${CMAKE_CURRENT_LIST_DIR}/youi/cmake/Platform/YiIos.cmake) 
if(TVOS)
    include(${CMAKE_CURRENT_LIST_DIR}/youi/cmake/Platform/YiTvos.cmake)
if (ANDROID)
    include(${CMAKE_CURRENT_LIST_DIR}/youi/android/modules/KalturaPlayer/src)
endif()

target_sources(${PROJECT_NAME} PRIVATE
    ${_MODULE_HEADERS}
    ${_MODULE_SOURCE}
)

# make headers available for linking the project
target_include_directories(${PROJECT_NAME}
    PRIVATE ${_MODULE_PATH}
)

message(STATUS "_MODULE_PATH: ${_MODULE_PATH}")
message(STATUS "")
