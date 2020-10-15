#include "../PKPlayer.h"

// Populated for us.
extern JavaVM *cachedJVM;
extern jobject cachedActivity;

extern "C" {
    JNIEXPORT void JNICALL Java_tv_youi_myapp_PKPlayerWrapper_nativeSendEvent(JNIEnv *, jclass, jobject, jstring, jstring);
}

// Boilerplate
static JNIEnv *GetEnv()
{
    static JNIEnv *pEnv;
    cachedJVM->GetEnv(reinterpret_cast<void **>(&pEnv), JNI_VERSION_1_6);
    return pEnv;
}

YI_RN_INSTANTIATE_MODULE(PKPlayerWrapper, yi::react::EventEmitterModule);
YI_RN_REGISTER_MODULE(PKPlayerWrapper);

void JNICALL Java_tv_youi_myapp_PKPlayerWrapper_nativeSendEvent(JNIEnv *pEnv, jclass cls, jobject instance, jstring name, jstring payload)
{
    auto *pBridge = reinterpret_cast<PKPlayerWrapper *>(pEnv->GetDirectBufferAddress(instance));
    const auto std_name = std::string(pEnv->GetStringUTFChars(name, NULL));
    const auto std_payload = std::string(pEnv->GetStringUTFChars(payload, NULL));
    dynamic parsed = NULL;
    if (!std_payload.empty()) {
       parsed = parseJson(std_payload); 
    }
    pBridge->Emit(std_name, parsed);
}

PKPlayerWrapper::PKPlayerWrapper()
{
    SetSupportedEvents(PKPLAYER_SUPPORTED_EVENTS);
    loadIDs();
}

PKPlayerWrapper::~PKPlayerWrapper()
{
    destroy();
}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, loadIDs)()
{
     jclass tmpBridgeClass = GetEnv()->FindClass("tv/youi/myapp/PKPlayerWrapper");
     playerWrapperBridgeClass = reinterpret_cast<jclass>(GetEnv()->NewGlobalRef(tmpBridgeClass));
     
    if (!playerWrapperBridgeClass) {
        return;
    }

     setupMethodID = GetEnv()->GetStaticMethodID(playerWrapperBridgeClass, "setup", "(Ljava/nio/ByteBuffer;ILjava/lang/String;)V");
     loadMethodID = GetEnv()->GetStaticMethodID(playerWrapperBridgeClass, "load", "(Ljava/lang/String;Ljava/lang/String;)V");
     prepareMethodID = GetEnv()->GetStaticMethodID(playerWrapperBridgeClass, "prepare", "()V");
     playMethodID = GetEnv()->GetStaticMethodID(playerWrapperBridgeClass, "play", "()V");
     pauseMethodID = GetEnv()->GetStaticMethodID(playerWrapperBridgeClass, "pause", "()V");
     replayMethodID = GetEnv()->GetStaticMethodID(playerWrapperBridgeClass, "replay", "()V");
     destroyMethodID = GetEnv()->GetStaticMethodID(playerWrapperBridgeClass, "destroy", "()V");
     stopMethodID = GetEnv()->GetStaticMethodID(playerWrapperBridgeClass, "stop", "()V");
     seekToMethodID = GetEnv()->GetStaticMethodID(playerWrapperBridgeClass, "seekTo", "(F)V");
     changeTrackMethodID = GetEnv()->GetStaticMethodID(playerWrapperBridgeClass, "changeTrack", "(Ljava/lang/String;)V");
     changePlaybackRateMethodID = GetEnv()->GetStaticMethodID(playerWrapperBridgeClass, "changePlaybackRate", "(F)V");
     setAutoplayMethodID = GetEnv()->GetStaticMethodID(playerWrapperBridgeClass, "setAutoplay", "(Z)V");
     setKSMethodID = GetEnv()->GetStaticMethodID(playerWrapperBridgeClass, "setKS", "(Ljava/lang/String;)V");
     setZIndexMethodID = GetEnv()->GetStaticMethodID(playerWrapperBridgeClass, "setZIndex", "(F)V");
     setFrameMethodID = GetEnv()->GetStaticMethodID(playerWrapperBridgeClass, "setFrame", "(IIII)V");
     setVolumeMethodID = GetEnv()->GetStaticMethodID(playerWrapperBridgeClass, "setVolume", "(F)V");
     setLogLevelMethodID = GetEnv()->GetStaticMethodID(playerWrapperBridgeClass, "setLogLevel", "(Ljava/lang/String;)V");
}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, setup)(int partnerId, dynamic options)
{ 
    if (!playerWrapperBridgeClass) {
        return;
    }

    jobject jInstance = GetEnv()->NewDirectByteBuffer(this, sizeof(PKPlayerWrapper));
    auto jsonOptionsStr = toJson(options);
    jstring optionsStr = GetEnv()->NewStringUTF(jsonOptionsStr.c_str());
    GetEnv()->CallStaticVoidMethod(playerWrapperBridgeClass, setupMethodID, jInstance, partnerId, optionsStr);
    GetEnv()->DeleteLocalRef(jInstance);
    GetEnv()->DeleteLocalRef(optionsStr);
}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, load)(std::string assetId, dynamic options)
{
    if (!playerWrapperBridgeClass) {
        return;
    }
    
    jstring jAssetId = GetEnv()->NewStringUTF(assetId.c_str());
    auto jsonOptionsStr = toJson(options);
    jstring optionsStr = GetEnv()->NewStringUTF(jsonOptionsStr.c_str());

    GetEnv()->CallStaticVoidMethod(playerWrapperBridgeClass, loadMethodID, jAssetId, optionsStr);
    GetEnv()->DeleteLocalRef(jAssetId);
    GetEnv()->DeleteLocalRef(optionsStr);
}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, prepare)()
{
    if (!playerWrapperBridgeClass) {
        return;
    }

    GetEnv()->CallStaticVoidMethod(playerWrapperBridgeClass, prepareMethodID);
}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, play)()
{
    if (!playerWrapperBridgeClass) {
        return;
    }

    GetEnv()->CallStaticVoidMethod(playerWrapperBridgeClass, playMethodID);
}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, pause)()
{
    if (!playerWrapperBridgeClass) {
        return;
    }

    GetEnv()->CallStaticVoidMethod(playerWrapperBridgeClass, pauseMethodID);
}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, replay)()
{
    if (!playerWrapperBridgeClass) {
        return;
    }

    GetEnv()->CallStaticVoidMethod(playerWrapperBridgeClass, replayMethodID);
}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, stop)()
{
    if (!playerWrapperBridgeClass) {
        return;
    }

    GetEnv()->CallStaticVoidMethod(playerWrapperBridgeClass, stopMethodID);
}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, destroy)()
{
    if (!playerWrapperBridgeClass) {
        return;
    }

    GetEnv()->CallStaticVoidMethod(playerWrapperBridgeClass, destroyMethodID);
}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, seekTo)(float position)
{
    if (!playerWrapperBridgeClass) {
        return;
    }

    GetEnv()->CallStaticVoidMethod(playerWrapperBridgeClass, seekToMethodID, position);
}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, changeTrack)(std::string uniqueId)
{
    if (!playerWrapperBridgeClass) {
        return;
    }

    jstring jUniqueId = GetEnv()->NewStringUTF(uniqueId.c_str());
    GetEnv()->CallStaticVoidMethod(playerWrapperBridgeClass, changeTrackMethodID, jUniqueId);
    GetEnv()->DeleteLocalRef(jUniqueId);

}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, changePlaybackRate)(float playbackRate)
{
    if (!playerWrapperBridgeClass) {
        return;
    }

    GetEnv()->CallStaticVoidMethod(playerWrapperBridgeClass, changePlaybackRateMethodID, playbackRate);
}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, setAutoplay)(bool autoplay)
{
    if (!playerWrapperBridgeClass) {
        return;
    }

    GetEnv()->CallStaticVoidMethod(playerWrapperBridgeClass, setAutoplayMethodID, autoplay);
}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, setKS)(std::string ks)
{
    if (!playerWrapperBridgeClass) {
        return;
    }

    jstring jKs = GetEnv()->NewStringUTF(ks.c_str());
    GetEnv()->CallStaticVoidMethod(playerWrapperBridgeClass, setKSMethodID, jKs);
    GetEnv()->DeleteLocalRef(jKs);
}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, setZIndex)(float index)
{
    if (!playerWrapperBridgeClass) {
        return;
    }

    GetEnv()->CallStaticVoidMethod(playerWrapperBridgeClass, setZIndexMethodID, index);
}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, setFrame)(int playerViewWidth, int playerViewHeight, int playerViewPosX, int playerViewPosY)
{
    if (!playerWrapperBridgeClass) {
        return;
    }

    GetEnv()->CallStaticVoidMethod(playerWrapperBridgeClass, setFrameMethodID, playerViewWidth, playerViewHeight, playerViewPosX, playerViewPosY);
}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, setVolume)(float volume)
{
    if (!playerWrapperBridgeClass) {
        return;
    }

    GetEnv()->CallStaticVoidMethod(playerWrapperBridgeClass, setVolumeMethodID, volume);
}

//Valid values: "VERBOSE", "DEBUG", "INFO", "WARN", "ERROR", "OFF"
YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, setLogLevel)(std::string logLevel)
{
    if (!playerWrapperBridgeClass) {
        return;
    }

    jstring jLogLevel = GetEnv()->NewStringUTF(logLevel.c_str());
    GetEnv()->CallStaticVoidMethod(playerWrapperBridgeClass, setLogLevelMethodID, jLogLevel);
    GetEnv()->DeleteLocalRef(jLogLevel);
}
