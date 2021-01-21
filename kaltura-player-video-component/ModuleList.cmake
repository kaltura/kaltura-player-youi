message(STATUS "")
message(STATUS "@kaltura-player/video-component")
message(STATUS "Running: ${CMAKE_CURRENT_LIST_DIR}/ModuleList.cmake")

set(_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/youi/src")
set(KALTURA_PLAYER_JAVA_SRC_PATH ${CMAKE_CURRENT_LIST_DIR})

file(GLOB_RECURSE _headers1 "${_MODULE_PATH}/nativemodule/*.h" "${_MODULE_PATH}/pk/*.h")

if(IOS OR TVOS)
	file(GLOB_RECURSE _headers2 "${_MODULE_PATH}/player/ios/*.h")
	set(_MODULE_HEADERS ${_headers1} ${_headers2})
endif()

if(ANDROID)
	set(_MODULE_HEADERS ${_headers1})
	message(STATUS "_MODULE_HEADERS : ${_MODULE_HEADERS}")
endif()

file(GLOB_RECURSE _source1 "${_MODULE_PATH}/nativemodule/*.cpp" "${_MODULE_PATH}/player/*.cpp")

if(IOS OR TVOS)
	file(GLOB_RECURSE _source2 "${_MODULE_PATH}/player/ios/*.m" "${_MODULE_PATH}/player/ios/*.mm")
	file(GLOB_RECURSE _source3 "${_MODULE_PATH}/pk/ios/*.cpp")
	set(_MODULE_SOURCE ${_source1} ${_source2} ${_source3})
endif()

if(ANDROID)
	file(GLOB_RECURSE _source4 "${_MODULE_PATH}/pk/android/*.cpp")
	set(_MODULE_SOURCE ${_source1} ${_source4})
	message(STATUS "_MODULE_SOURCE : ${_MODULE_SOURCE}")
endif()

# Make sure the specific platform include is processed
if(IOS)
    include(${CMAKE_CURRENT_LIST_DIR}/youi/cmake/Platform/YiIos.cmake)
endif()
if(TVOS)
    include(${CMAKE_CURRENT_LIST_DIR}/youi/cmake/Platform/YiTvos.cmake)
endif()

message(STATUS "PROJECT_NAME : ${YI_PROJECT_NAME}")

target_sources(${YI_PROJECT_NAME} PRIVATE
	${_MODULE_HEADERS}
	${_MODULE_SOURCE}
)

# make headers available for linking the project
target_include_directories(${YI_PROJECT_NAME}
	PRIVATE ${_MODULE_PATH}
)

message(STATUS "_MODULE_PATH: ${_MODULE_PATH}")
message(STATUS "")
