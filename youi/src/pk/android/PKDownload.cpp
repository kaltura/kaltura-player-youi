#include "../PKDownload.h"

// Populated for us.
extern JavaVM *cachedJVM;
extern jobject cachedActivity;

extern "C" {
    JNIEXPORT void JNICALL Java_tv_youi_kalturaplayertest_PKDownloadWrapper_nativeSendEvent(JNIEnv *, jclass, jobject, jstring, jstring);
}

// Boilerplate
static JNIEnv *GetEnv()
{
    static JNIEnv *pEnv;
    cachedJVM->GetEnv(reinterpret_cast<void **>(&pEnv), JNI_VERSION_1_6);
    return pEnv;
}

YI_RN_INSTANTIATE_MODULE(PKDownloadWrapper, yi::react::EventEmitterModule);
YI_RN_REGISTER_MODULE(PKDownloadWrapper);

void JNICALL Java_tv_youi_kalturaplayertest_PKDownloadWrapper_nativeSendEvent(JNIEnv *pEnv, jclass cls, jobject instance, jstring name, jstring payload)
{
    auto *pBridge = reinterpret_cast<PKDownloadWrapper *>(pEnv->GetDirectBufferAddress(instance));
    const auto std_name = std::string(pEnv->GetStringUTFChars(name, NULL));
    const auto std_payload = std::string(pEnv->GetStringUTFChars(payload, NULL));
    dynamic parsed = NULL;
    if (!std_payload.empty()) {
       parsed = parseJson(std_payload); 
    }
    pBridge->Emit(std_name, parsed);
}

PKDownloadWrapper::PKDownloadWrapper()
{
    SetSupportedEvents(PKDOWNLOAD_SUPPORTED_EVENTS);
    loadIDs();
}

PKDownloadWrapper::~PKDownloadWrapper()
{   
}

YI_RN_DEFINE_EXPORT_METHOD(PKDownloadWrapper, loadIDs)()
{
     jclass tmpBridgeClass = GetEnv()->FindClass("tv/youi/kalturaplayertest/PKDownloadWrapper");
     downloadWrapperBridgeClass = reinterpret_cast<jclass>(GetEnv()->NewGlobalRef(tmpBridgeClass));

    if (!downloadWrapperBridgeClass) {
        return;
    }
    
     setupMethodID = GetEnv()->GetStaticMethodID(downloadWrapperBridgeClass, "setup", "(Ljava/nio/ByteBuffer;ILjava/lang/String;)V");
     offlineStartMethodID = GetEnv()->GetStaticMethodID(downloadWrapperBridgeClass, "start", "(Ljava/lang/String;Ljava/lang/String;)V");
     offlinePauseMethodID = GetEnv()->GetStaticMethodID(downloadWrapperBridgeClass, "pause", "(Ljava/lang/String;)V");
     offlineResumeMethodID = GetEnv()->GetStaticMethodID(downloadWrapperBridgeClass, "resume", "(Ljava/lang/String;)V");
     offlineRemoveMethodID = GetEnv()->GetStaticMethodID(downloadWrapperBridgeClass, "remove", "(Ljava/lang/String;)V");
    
}

YI_RN_DEFINE_EXPORT_METHOD(PKDownloadWrapper, setup)(int partnerId, std::string serverUrl)
{
    if (!downloadWrapperBridgeClass) {
        return;
    }

    jobject jInstance = GetEnv()->NewDirectByteBuffer(this, sizeof(PKDownloadWrapper));
    jstring jServerUrl = GetEnv()->NewStringUTF(serverUrl.c_str());

    GetEnv()->CallStaticVoidMethod(downloadWrapperBridgeClass, setupMethodID, jInstance, partnerId, jServerUrl);
    GetEnv()->DeleteLocalRef(jInstance);
    GetEnv()->DeleteLocalRef(jServerUrl);
}

static void callOfflineAssetMethod(JNIEnv *pDownloadWrapperEnv, jclass downloadWrapperBridgeClass, jmethodID methodID, std::string assetId)
{
    if (!downloadWrapperBridgeClass) {
        return;
    }

    jstring jAssetId = GetEnv()->NewStringUTF(assetId.c_str());
    GetEnv()->CallStaticVoidMethod(downloadWrapperBridgeClass, methodID, jAssetId);
    GetEnv()->DeleteLocalRef(jAssetId);
}

YI_RN_DEFINE_EXPORT_METHOD(PKDownloadWrapper, offline_start)(std::string assetId, dynamic data)
{
    if (!downloadWrapperBridgeClass) {
        return;
    }

    jstring jAssetId = GetEnv()->NewStringUTF(assetId.c_str());
    auto jsonDataStr = toJson(data);
    jstring dataStr = GetEnv()->NewStringUTF(jsonDataStr.c_str());
    
    GetEnv()->CallStaticVoidMethod(downloadWrapperBridgeClass, offlineStartMethodID, jAssetId, dataStr);
    GetEnv()->DeleteLocalRef(jAssetId);
    GetEnv()->DeleteLocalRef(dataStr);
}

YI_RN_DEFINE_EXPORT_METHOD(PKDownloadWrapper, offline_pause)(std::string assetId)
{
    if (!downloadWrapperBridgeClass) {
        return;
    }
    
    callOfflineAssetMethod(GetEnv(), downloadWrapperBridgeClass, offlinePauseMethodID, assetId);
}

YI_RN_DEFINE_EXPORT_METHOD(PKDownloadWrapper, offline_resume)(std::string assetId)
{
    if (!downloadWrapperBridgeClass) {
        return;
    }

    callOfflineAssetMethod(GetEnv(), downloadWrapperBridgeClass, offlineResumeMethodID, assetId);
}

YI_RN_DEFINE_EXPORT_METHOD(PKDownloadWrapper, offline_remove)(std::string assetId)
{
    if (!downloadWrapperBridgeClass) {
        return;
    }

    callOfflineAssetMethod(GetEnv(), downloadWrapperBridgeClass, offlineRemoveMethodID, assetId);
}
