include(${YouiEngine_DIR}/cmake/Platform/YiIos.cmake)

if(DEFINED ENV{DEVELOPMENT_TEAM_ID})
    if(NOT DEFINED YI_DEVELOPMENT_TEAM)
        set(YI_DEVELOPMENT_TEAM $ENV{DEVELOPMENT_TEAM_ID})
    endif()
elseif(NOT DEFINED YI_DEVELOPMENT_TEAM)
    include(${YouiEngine_DIR}/cmake/YiAppleCodeSigning.cmake OPTIONAL RESULT_VARIABLE _DEFAULT_SIGNING_PATHNAME)
    if(_DEFAULT_SIGNING_PATHNAME MATCHES NOTFOUND)
        message(FATAL_ERROR "A development team ID is missing. Please set the YI_DEVELOPMENT_TEAM variable when generating the project or set a DEVELOPMENT_TEAM_ID environment variable.")
    endif()
endif()

set(YI_DEVELOPMENT_TEAM "${YI_DEVELOPMENT_TEAM}" CACHE STRING "The 10-character alphanumeric string representing the development team. Required for Xcode 8+ builds.")

macro(add_pk_ios_framework FW_NAME)
    yi_configure_framework(TARGET ${_ARGS_PROJECT_TARGET}
        FRAMEWORK_PATH "${KALTURA_PLAYER_FRAMEWORKS_PATH}/ios-libs/${FW_NAME}/${FW_NAME}.framework"
        CODE_SIGN_IDENTITY ${YI_CODE_SIGN_IDENTITY}
        EMBEDDED
    )
endmacro()

macro(yi_configure_platform)
    cmake_parse_arguments(_ARGS "" "PROJECT_TARGET" "" ${ARGN})

    if(NOT _ARGS_PROJECT_TARGET)
        message(FATAL_ERROR "'yi_configure_platform' requires the PROJECT_TARGET argument be set")
    endif()

    set_target_properties(${_ARGS_PROJECT_TARGET} PROPERTIES
        XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "${YI_DEVELOPMENT_TEAM}"
	XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "10.0"
    )

    _yi_configure_platform(PROJECT_TARGET ${_ARGS_PROJECT_TARGET})

    include(Modules/apple/YiConfigureFramework)

    foreach(FW_NAME KalturaPlayer PlayKit KalturaNetKit PlayKitUtils PlayKitKava PlayKitProviders)
    add_pk_ios_framework(${FW_NAME})
    endforeach()

    foreach(FW_NAME XCGLogger SwiftyJSON ObjcExceptionBridging SwiftyXMLParser)
    add_pk_ios_framework(${FW_NAME})
    endforeach()

    foreach(FW_NAME PlayKit_IMA GoogleInteractiveMediaAds)
    add_pk_ios_framework(${FW_NAME})
    endforeach()

#    foreach(FW_NAME DownloadToGo RealmSwift Realm GCDWebServer M3U8Kit)
#    add_pk_ios_framework(${FW_NAME})
#    endforeach()

    foreach(FW_NAME YouboraLib PlayKitYoubora)
    add_pk_ios_framework(${FW_NAME})
    endforeach()

    foreach(FW_NAME AFNetworking SmartLib SmartLibKaltura PlayKitBroadpeak)
    add_pk_ios_framework(${FW_NAME})
    endforeach()

endmacro(yi_configure_platform)
