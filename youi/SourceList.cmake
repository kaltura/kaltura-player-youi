# =============================================================================
# © You i Labs Inc. 2000-2020. All rights reserved.

if(IOS OR TVOS)
  # Declare the source and header files for iOS and tvOS

  set(YI_PROJECT_PLATFORM_SOURCE
    src/player/ios/KalturaPlayerYI.m
    src/player/ios/KalturaVideoPlayer.mm
    src/pk/ios/PKDownload.mm
  )
  set (YI_PROJECT_PLATFORM_HEADERS
    src/player/ios/KalturaPlayerYI.h
  )

elseif(ANDROID)
  # Declare the source and header files for Android.

  set(YI_PROJECT_PLATFORM_SOURCE
    src/player/android/KalturaVideoPlayer_android.cpp
    src/pk/android/PKDownload.cpp
  )
  set (YI_PROJECT_PLATFORM_HEADERS
    
  )

else()
  # Declare the source and header files for default module implementations

  set(YI_PROJECT_PLATFORM_SOURCE
    
  )
  set (YI_PROJECT_PLATFORM_HEADERS
    
  )

endif()



# Set all project source files, including platform specific ones above
set (YI_PROJECT_SOURCE
    src/app/App.cpp
    src/app/AppFactory.cpp
    src/nativemodule/KalturaVideoNativeModule.cpp
    src/player/KalturaVideoPlayer.cpp
    src/player/KalturaVideoSurface.cpp
    ${YI_PROJECT_PLATFORM_SOURCE}
)

# Set all project header files, including platform specific ones above
set (YI_PROJECT_HEADERS
    src/app/App.h
    src/nativemodule/KalturaVideoNativeModule.h
    src/player/IVideoSurfaceListener.h
    src/player/KalturaVideoPlayer.h
    src/player/KalturaVideoPlayerPriv.h
    src/player/KalturaVideoSurface.h
    src/pk/PKDownload.h
    ${YI_PROJECT_PLATFORM_HEADERS}
)
