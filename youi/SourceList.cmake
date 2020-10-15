# =============================================================================
# © You i Labs Inc. 2000-2020. All rights reserved.

if(IOS OR TVOS OR OSX)
set (YI_PROJECT_SOURCE
    src/App.cpp
    src/AppFactory.cpp
    src/pk/ios/PKPlayer.mm
    src/pk/ios/PKDownload.mm
    src/pk/ios/KalturaPlayerYI.m
)
else()
set (YI_PROJECT_SOURCE
    src/App.cpp
    src/AppFactory.cpp
    src/pk/android/PKPlayer.cpp
    src/pk/android/PKDownload.cpp
)
endif()

set (YI_PROJECT_HEADERS
    src/App.h
    src/pk/PKPlayer.h
    src/pk/PKDownload.h
)
