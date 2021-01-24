message(STATUS "")
message(STATUS "@kaltura-player/video-component")
message(STATUS "Running: ${CMAKE_CURRENT_LIST_DIR}/ModuleList.cmake")

set(_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/youi/src")
set(KALTURA_PLAYER_JAVA_SRC_PATH ${CMAKE_CURRENT_LIST_DIR})
set(KALTURA_PLAYER_FRAMEWORKS_PATH ${CMAKE_CURRENT_LIST_DIR}) 



file(GLOB _commonheaders "${_MODULE_PATH}/nativemodule/*.h" "${_MODULE_PATH}/pk/*.h" "${_MODULE_PATH}/player/*.h")
file(GLOB _commonsource "${_MODULE_PATH}/nativemodule/*.cpp" "${_MODULE_PATH}/player/*.cpp")

if(IOS OR TVOS)
	file(GLOB _appleheaders "${_MODULE_PATH}/player/ios/*.h")
	set(_MODULE_HEADERS ${_commonheaders} ${_appleheaders})

 	file(GLOB _applesource "${_MODULE_PATH}/pk/ios/*.mm" "${_MODULE_PATH}/player/ios/*.mm" "${_MODULE_PATH}/player/ios/*.m")
	set(_MODULE_SOURCE ${_commonsource} ${_applesource})
endif()


if(ANDROID)
	set(_MODULE_HEADERS ${_commonheaders})

	file(GLOB _androidsource "${_MODULE_PATH}/pk/android/*.cpp" "${_MODULE_PATH}/player/android/*.cpp")
	set(_MODULE_SOURCE ${_commonsource} ${_androidsource})
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

# Make headers available for linking the project
target_include_directories(${YI_PROJECT_NAME}
	PRIVATE ${_MODULE_PATH}
)

message(STATUS "_MODULE_PATH: ${_MODULE_PATH}")
message(STATUS "")
