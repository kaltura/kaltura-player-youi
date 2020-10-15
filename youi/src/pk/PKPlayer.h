#ifndef _PKPLAYER_H_
#define _PKPLAYER_H_

#include <youireact/NativeModuleRegistry.h>
#include <youireact/NativeModule.h>
#include <youireact/modules/EventEmitter.h>
#include <utility/YiTimer.h>
#include <folly/dynamic.h>
#include <folly/json.h>

#ifdef ANDROID
// Exposes the JNI environment.
#include <jni.h>
#endif

#ifdef __OBJC__
@class KalturaPlayerYI;
#endif

#define PKPLAYER_SUPPORTED_EVENTS {\
    "loadMediaSuccess", \
    "loadMediaFailed", \
    "ended", \
    "canPlay", \
    "playing", \
    "play", \
    "pause", \
    "stopped", \
    "durationChanged", \
    "timeUpdate", \
    "stateChanged", \
    "tracksAvailable", \
    "videoTrackChanged", \
    "audioTrackChanged", \
    "textTrackChanged", \
    "seeking", \
    "seeked", \
    "error", \
    "adProgress", \
    "adCuepointsChanged", \
    "adStarted", \
    "adCompleted", \
    "adPaused", \
    "adResumed", \
    "adBufferStart", \
    "adClicked", \
    "adSkipped", \
    "adRequested", \
    "adContentPauseRequested", \
    "adContentResumeRequested", \
    "allAdsCompleted", \
    "adError" \
}

using folly::dynamic;
using folly::parseJson;
using folly::toJson;

class YI_RN_MODULE(PKPlayerWrapper, yi::react::EventEmitterModule)
{

private:

#ifdef ANDROID

    jclass playerWrapperBridgeClass = NULL;
    jmethodID setupMethodID = 0;
    jmethodID loadMethodID = 0;
    jmethodID prepareMethodID = 0;
    jmethodID playMethodID = 0;
    jmethodID pauseMethodID = 0;
    jmethodID replayMethodID = 0;
    jmethodID destroyMethodID = 0;
    jmethodID stopMethodID = 0;
    jmethodID seekToMethodID = 0;
    jmethodID changeTrackMethodID = 0;
    jmethodID changePlaybackRateMethodID = 0;
    jmethodID setAutoplayMethodID = 0;
    jmethodID setKSMethodID = 0;
    jmethodID setZIndexMethodID = 0;
    jmethodID setFrameMethodID = 0;
    jmethodID setVolumeMethodID = 0;
    jmethodID setLogLevelMethodID = 0;

    YI_RN_EXPORT_METHOD(loadIDs)();
    
#endif

public:

    YI_RN_EXPORT_NAME(PKPlayerWrapper);

    PKPlayerWrapper();
    ~PKPlayerWrapper();

    YI_RN_EXPORT_METHOD(setup)(int partnerId, dynamic options);

    YI_RN_EXPORT_METHOD(load)(std::string assetId, dynamic options);
    YI_RN_EXPORT_METHOD(prepare)();
    
    YI_RN_EXPORT_METHOD(play)();
    YI_RN_EXPORT_METHOD(pause)();
    YI_RN_EXPORT_METHOD(replay)();
    YI_RN_EXPORT_METHOD(destroy)();
    YI_RN_EXPORT_METHOD(stop)();
    YI_RN_EXPORT_METHOD(seekTo)(float position);
    
    YI_RN_EXPORT_METHOD(changeTrack)(std::string uniqueId);
    YI_RN_EXPORT_METHOD(changePlaybackRate)(float playbackRate);
    YI_RN_EXPORT_METHOD(setAutoplay)(bool autoplay);
    YI_RN_EXPORT_METHOD(setKS)(std::string ks);
    YI_RN_EXPORT_METHOD(setZIndex)(float index);
    YI_RN_EXPORT_METHOD(setFrame)(int playerViewWidth, int playerViewHeight, int playerViewPosX, int playerViewPosY);
    YI_RN_EXPORT_METHOD(setVolume)(float volume);
    YI_RN_EXPORT_METHOD(setLogLevel)(std::string logLevel);

    void Emit(const std::string &event, const folly::dynamic &content) {EmitEvent(event, content);}
    
#ifdef __OBJC__ // For iOS/tvOS
private:
    KalturaPlayerYI* m_player = NULL;
    
#endif
    
};

#endif // _PKPLAYER_H_

