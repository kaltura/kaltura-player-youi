include(${YouiEngine_DIR}/cmake/Platform/YiTvos.cmake)



macro(add_pk_tvos_framework FW_NAME)
    yi_configure_framework(TARGET ${_ARGS_PROJECT_TARGET}
        FRAMEWORK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tvos-libs/${FW_NAME}/${FW_NAME}.framework"
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
	XCODE_ATTRIBUTE_TVOS_DEPLOYMENT_TARGET "10.0"
    )


    _yi_configure_platform(PROJECT_TARGET ${_ARGS_PROJECT_TARGET})

    include(Modules/apple/YiConfigureFramework)


    foreach(FW_NAME KalturaPlayer PlayKit KalturaNetKit PlayKitUtils PlayKitKava PlayKitProviders)
    add_pk_tvos_framework(${FW_NAME})
    endforeach()

    foreach(FW_NAME XCGLogger SwiftyJSON ObjcExceptionBridging SwiftyXMLParser)
    add_pk_tvos_framework(${FW_NAME})
    endforeach()

    foreach(FW_NAME PlayKit_IMA GoogleInteractiveMediaAds)
    add_pk_tvos_framework(${FW_NAME})
    endforeach()

#    foreach(FW_NAME DownloadToGo RealmSwift Realm GCDWebServer M3U8Kit)
#    add_pk_tvos_framework(${FW_NAME})
#    endforeach()

    foreach(FW_NAME YouboraLib PlayKitYoubora)
    add_pk_tvos_framework(${FW_NAME})
    endforeach()

   # Strip unwanted architectures from dynamic libraries
    add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD COMMAND
        "${CMAKE_CURRENT_SOURCE_DIR}/Scripts/strippingUnwantedArchitecturesFromDynamicLibraries.sh"
    )

endmacro(yi_configure_platform)