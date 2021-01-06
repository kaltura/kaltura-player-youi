#ifndef _PKDOWNLOAD_H_
#define _PKDOWNLOAD_H_

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

using folly::dynamic;
using folly::parseJson;
using folly::toJson;

#ifdef __OBJC__
#endif

#define PKDOWNLOAD_SUPPORTED_EVENTS {\
    "offlineManagerStarted", \
    "onAssetDownloadFailed", \
    "onAssetDownloadComplete", \
    "onAssetDownloadPending", \
    "onAssetDownloadPaused", \
    "onAssetRegistered", \
    "onAssetRegisterError", \
    "onAssetStateChanged", \
    "onAssetRemoved", \
    "onDownloadProgress" \
}

class YI_RN_MODULE(PKDownloadWrapper, yi::react::EventEmitterModule)
{
    
private:

#ifdef ANDROID

    jclass downloadWrapperBridgeClass = NULL;
    jmethodID setupMethodID = 0;
    jmethodID offlineStartMethodID = 0;
    jmethodID offlinePauseMethodID= 0;
    jmethodID offlineResumeMethodID = 0;
    jmethodID offlineRemoveMethodID= 0;

    YI_RN_EXPORT_METHOD(loadIDs)();

#endif

public:

    YI_RN_EXPORT_NAME(PKDownloadWrapper);

    PKDownloadWrapper();
    ~PKDownloadWrapper();
    
    YI_RN_EXPORT_METHOD(setup)(int partnerId, std::string serverUrl);

    // Download
    YI_RN_EXPORT_METHOD(offline_start)(std::string assetId, dynamic data);
    YI_RN_EXPORT_METHOD(offline_pause)(std::string assetId);
    YI_RN_EXPORT_METHOD(offline_resume)(std::string assetId);
    YI_RN_EXPORT_METHOD(offline_remove)(std::string assetId);

#ifdef ANDROID
    void Emit(const std::string &event, const folly::dynamic &content) {EmitEvent(event, content);}
#endif
    
#ifdef __OBJC__ // For iOS/tvOS
private:
#endif
    
};

#endif // _PKDOWNLOAD_H_
