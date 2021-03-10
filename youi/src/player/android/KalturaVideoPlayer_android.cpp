#include "player/KalturaVideoPlayerPriv.h"

#include <player/YiVideoPlayerStateManager.h>
#include <framework/YiAppContext.h>
#include <glm/glm.hpp>
#include <renderer/YiSurface.h>
#include <framework/YiScreen.h>

#include "player/KalturaVideoSurface.h"

static const CYIString TAG("KalturaVideoPlayerPriv");

extern JavaVM *cachedJVM;
extern jobject cachedActivity;

extern "C" {
    JNIEXPORT void JNICALL Java_tv_youi_kalturaplayeryoui_PKPlayerWrapper_nativeSendEvent(JNIEnv *, jclass, jobject, jstring, jstring);
}

static JNIEnv *GetEnv_KalturaPlayer()
{
    JNIEnv *pEnv;
    cachedJVM->GetEnv(reinterpret_cast<void **>(&pEnv), JNI_VERSION_1_6);
    return pEnv;
}

void JNICALL Java_tv_youi_kalturaplayeryoui_PKPlayerWrapper_nativeSendEvent(JNIEnv *pEnv, jclass cls, jobject instance, jstring name, jstring payload)
{
    auto *pBridge = reinterpret_cast<KalturaVideoPlayerPriv *>(pEnv->GetDirectBufferAddress(instance));
    const auto event = std::string(pEnv->GetStringUTFChars(name, NULL));
    const auto std_payload = std::string(pEnv->GetStringUTFChars(payload, NULL));
    folly::dynamic content;
    if (!std_payload.empty())
    {
        content = folly::parseJson(std_payload);
    }

    pBridge->HandleEvent(event, content);
}

void KalturaVideoPlayerPriv::HandleEvent(const CYIString& name, folly::dynamic content)
{
    m_pPub->HandleEvent(name, content);
}

KalturaVideoPlayerPriv::KalturaVideoPlayerPriv(KalturaVideoPlayer *pPub)
    : m_pPub(pPub)
{
    // jobject localPlayerObject = pEnv->NewObject(playerClass, _constructor);
    // playerObject = pEnv->NewGlobalRef(localPlayerObject);

    LoadIDs();
}

KalturaVideoPlayerPriv::~KalturaVideoPlayerPriv()
{
    if (playerWrapperBridgeClass)
    {
        GetEnv_KalturaPlayer()->CallStaticVoidMethod(playerWrapperBridgeClass, destroyMethodID);
        return;
    }

    playerWrapperBridgeClass = NULL;
    setupMethodID = 0;
    loadMediaMethodID = 0;
    setMediaMethodID = 0;
    prepareMethodID = 0;
    playMethodID = 0;
    pauseMethodID = 0;
    replayMethodID = 0;
    destroyMethodID = 0;
    stopMethodID = 0;
    seekToMethodID = 0;
    changeTrackMethodID = 0;
    changePlaybackRateMethodID = 0;
    setAutoplayMethodID = 0;
    setKSMethodID = 0;
    setZIndexMethodID = 0;
    setFrameMethodID = 0;
    setVolumeMethodID = 0;
    setLogLevelMethodID = 0;
}

void KalturaVideoPlayerPriv::LoadIDs()
{
    jclass tmpBridgeClass = GetEnv_KalturaPlayer()->FindClass("tv/youi/kalturaplayeryoui/PKPlayerWrapper");
    playerWrapperBridgeClass = reinterpret_cast<jclass>(GetEnv_KalturaPlayer()->NewGlobalRef(tmpBridgeClass));

    if (!playerWrapperBridgeClass)
    {
        return;
    }

    setupMethodID = GetEnv_KalturaPlayer()->GetStaticMethodID(playerWrapperBridgeClass, "setup", "(Ljava/nio/ByteBuffer;ILjava/lang/String;)V");
    loadMediaMethodID = GetEnv_KalturaPlayer()->GetStaticMethodID(playerWrapperBridgeClass, "loadMedia", "(Ljava/lang/String;Ljava/lang/String;)V");
    setMediaMethodID = GetEnv_KalturaPlayer()->GetStaticMethodID(playerWrapperBridgeClass, "setMedia", "(Ljava/lang/String;)V");
    prepareMethodID = GetEnv_KalturaPlayer()->GetStaticMethodID(playerWrapperBridgeClass, "prepare", "()V");
    playMethodID = GetEnv_KalturaPlayer()->GetStaticMethodID(playerWrapperBridgeClass, "play", "()V");
    pauseMethodID = GetEnv_KalturaPlayer()->GetStaticMethodID(playerWrapperBridgeClass, "pause", "()V");
    replayMethodID = GetEnv_KalturaPlayer()->GetStaticMethodID(playerWrapperBridgeClass, "replay", "()V");
    destroyMethodID = GetEnv_KalturaPlayer()->GetStaticMethodID(playerWrapperBridgeClass, "destroy", "()V");
    stopMethodID = GetEnv_KalturaPlayer()->GetStaticMethodID(playerWrapperBridgeClass, "stop", "()V");
    seekToMethodID = GetEnv_KalturaPlayer()->GetStaticMethodID(playerWrapperBridgeClass, "seekTo", "(F)V");
    changeTrackMethodID = GetEnv_KalturaPlayer()->GetStaticMethodID(playerWrapperBridgeClass, "changeTrack", "(Ljava/lang/String;)V");
    changePlaybackRateMethodID = GetEnv_KalturaPlayer()->GetStaticMethodID(playerWrapperBridgeClass, "changePlaybackRate", "(F)V");
    setAutoplayMethodID = GetEnv_KalturaPlayer()->GetStaticMethodID(playerWrapperBridgeClass, "setAutoplay", "(Z)V");
    setKSMethodID = GetEnv_KalturaPlayer()->GetStaticMethodID(playerWrapperBridgeClass, "setKS", "(Ljava/lang/String;)V");
    setZIndexMethodID = GetEnv_KalturaPlayer()->GetStaticMethodID(playerWrapperBridgeClass, "setZIndex", "(F)V");
    setFrameMethodID = GetEnv_KalturaPlayer()->GetStaticMethodID(playerWrapperBridgeClass, "setFrame", "(IIII)V");
    setVolumeMethodID = GetEnv_KalturaPlayer()->GetStaticMethodID(playerWrapperBridgeClass, "setVolume", "(F)V");
    setLogLevelMethodID = GetEnv_KalturaPlayer()->GetStaticMethodID(playerWrapperBridgeClass, "setLogLevel", "(Ljava/lang/String;)V");
}

void KalturaVideoPlayerPriv::Setup_(int32_t partnerId, folly::dynamic options)
{
    if (!playerWrapperBridgeClass)
    {
        return;
    }

    jobject jInstance = GetEnv_KalturaPlayer()->NewDirectByteBuffer(this, sizeof(KalturaVideoPlayerPriv));
    auto jsonOptionsStr = folly::toJson(options);
    jstring optionsStr = GetEnv_KalturaPlayer()->NewStringUTF(jsonOptionsStr.c_str());
    GetEnv_KalturaPlayer()->CallStaticVoidMethod(playerWrapperBridgeClass, setupMethodID, jInstance, partnerId, optionsStr);
    GetEnv_KalturaPlayer()->DeleteLocalRef(jInstance);
    GetEnv_KalturaPlayer()->DeleteLocalRef(optionsStr);
}

void KalturaVideoPlayerPriv::LoadMedia_(const CYIString &assetId, folly::dynamic options)
{
    if (!playerWrapperBridgeClass)
    {
        return;
    }

    if (m_pPub->GetPlayerState().mediaState == CYIAbstractVideoPlayer::MediaState::Ready)
    {
        m_pPub->m_pStateManager->TransitionToMediaUnloaded();
    }

    m_pPub->m_pStateManager->TransitionToMediaPreparing();

    jstring jAssetId = GetEnv_KalturaPlayer()->NewStringUTF(assetId.GetData());
    auto jsonOptionsStr = folly::toJson(options);
    jstring optionsStr = GetEnv_KalturaPlayer()->NewStringUTF(jsonOptionsStr.c_str());

    GetEnv_KalturaPlayer()->CallStaticVoidMethod(playerWrapperBridgeClass, loadMediaMethodID, jAssetId, optionsStr);
    GetEnv_KalturaPlayer()->DeleteLocalRef(jAssetId);
    GetEnv_KalturaPlayer()->DeleteLocalRef(optionsStr);

    if (m_bringToFront) {
        BringToFront_();
        m_bringToFront = false;
    }
}

void KalturaVideoPlayerPriv::SetMedia_(const CYIUrl &contentUrl)
{
  if (!playerWrapperBridgeClass)
    {
        return;
    }

    if (m_pPub->GetPlayerState().mediaState == CYIAbstractVideoPlayer::MediaState::Ready)
    {
        m_pPub->m_pStateManager->TransitionToMediaUnloaded();
    }

    m_pPub->m_pStateManager->TransitionToMediaPreparing();

    jstring url = GetEnv_KalturaPlayer()->NewStringUTF(contentUrl.ToString().GetData());

    GetEnv_KalturaPlayer()->CallStaticVoidMethod(playerWrapperBridgeClass, setMediaMethodID, url);
    GetEnv_KalturaPlayer()->DeleteLocalRef(url);

    if (m_bringToFront) {
        BringToFront_();
        m_bringToFront = false;
    }
}

void KalturaVideoPlayerPriv::SetLogLevel_(const CYIString &logLevel) {

    if (!playerWrapperBridgeClass)
    {
        return;
    }

    jstring logLevelStr = GetEnv_KalturaPlayer()->NewStringUTF(logLevel.GetData());

    GetEnv_KalturaPlayer()->CallStaticVoidMethod(playerWrapperBridgeClass, setLogLevelMethodID, logLevelStr);
    GetEnv_KalturaPlayer()->DeleteLocalRef(logLevelStr);
}

CYIString KalturaVideoPlayerPriv::GetName_() const
{
    return "kaltura-yi-android";
}

CYIAbstractVideoPlayer::Statistics KalturaVideoPlayerPriv::GetStatistics_() const
{
    CYIAbstractVideoPlayer::Statistics stats;
    return stats;
}

std::unique_ptr<CYIVideoSurface> KalturaVideoPlayerPriv::CreateSurface_()
{
    return std::unique_ptr<CYIVideoSurface>(new KalturaVideoSurface(m_pPub));
}

void KalturaVideoPlayerPriv::SetVideoRectangle(const YI_RECT_REL &videoRectangle)
{
    if (!playerWrapperBridgeClass)
    {
        return;
    }
    //YI_LOGD(TAG, "SetVideoRectangle width = %d height = %d", videoRectangle.width, videoRectangle.height);
    GetEnv_KalturaPlayer()->CallStaticVoidMethod(playerWrapperBridgeClass, setFrameMethodID, videoRectangle.width, videoRectangle.height, videoRectangle.x, videoRectangle.y);
}

bool KalturaVideoPlayerPriv::SupportsFormat_(CYIAbstractVideoPlayer::StreamingFormat eFormat, CYIAbstractVideoPlayer::DRMScheme eDRMScheme) const
{
    YI_UNUSED(eDRMScheme);
    return true;
}

void KalturaVideoPlayerPriv::Prepare_(const CYIUrl &videoURI, CYIAbstractVideoPlayer::StreamingFormat /*eFormatHint*/)
{
}

void KalturaVideoPlayerPriv::Play_()
{
    if (!playerWrapperBridgeClass)
    {
        return;
    }

    GetEnv_KalturaPlayer()->CallStaticVoidMethod(playerWrapperBridgeClass, playMethodID);
}

void KalturaVideoPlayerPriv::Pause_()
{
    if (!playerWrapperBridgeClass)
    {
        return;
    }

    GetEnv_KalturaPlayer()->CallStaticVoidMethod(playerWrapperBridgeClass, pauseMethodID);
}

void KalturaVideoPlayerPriv::Stop_()
{
    if (!playerWrapperBridgeClass)
    {
        return;
    }

    GetEnv_KalturaPlayer()->CallStaticVoidMethod(playerWrapperBridgeClass, stopMethodID);
}

void KalturaVideoPlayerPriv::Replay_()
{
    if (!playerWrapperBridgeClass)
    {
        return;
    }
    GetEnv_KalturaPlayer()->CallStaticVoidMethod(playerWrapperBridgeClass, replayMethodID);
}

void KalturaVideoPlayerPriv::ChangePlaybackRate_(float playbackRate)
{
    if (!playerWrapperBridgeClass)
    {
        return;
    }
    GetEnv_KalturaPlayer()->CallStaticVoidMethod(playerWrapperBridgeClass, changePlaybackRateMethodID, playbackRate);
}

void KalturaVideoPlayerPriv::SetPlayerZIndex_(float zIndex)
{
    if (!playerWrapperBridgeClass)
    {
        return;
    }
    GetEnv_KalturaPlayer()->CallStaticVoidMethod(playerWrapperBridgeClass, setZIndexMethodID, zIndex);
}

uint64_t KalturaVideoPlayerPriv::GetDurationMs_() const
{
    return m_pPub->m_durationMs;
}

uint64_t KalturaVideoPlayerPriv::GetCurrentTimeMs_() const
{
    return m_pPub->m_currentTimeMs;
}

void KalturaVideoPlayerPriv::Seek_(uint64_t uSeekPositionMs)
{
    if (!playerWrapperBridgeClass)
    {
        return;
    }

    double seekTime = static_cast<double>(uSeekPositionMs) / 1000.f;
    GetEnv_KalturaPlayer()->CallStaticVoidMethod(playerWrapperBridgeClass, seekToMethodID, (float)seekTime);
}

void KalturaVideoPlayerPriv::SetMaxBitrate_(uint64_t uMaxBitrate)
{
}

bool KalturaVideoPlayerPriv::SelectVideoTrack_(uint32_t uID)
{
    if (!playerWrapperBridgeClass)
    {
        return false;
    }

    const auto &track = m_pPub->m_videoTracks[uID];
    jstring jUniqueId = GetEnv_KalturaPlayer()->NewStringUTF(track.uniqueId.GetData());
    GetEnv_KalturaPlayer()->CallStaticVoidMethod(playerWrapperBridgeClass, changeTrackMethodID, jUniqueId);
    return true;
}

std::vector<KalturaVideoPlayer::VideoTrackInfo> KalturaVideoPlayerPriv::GetVideoTracks_() const
{
    return m_pPub->m_videoTracks;
}

KalturaVideoPlayer::VideoTrackInfo KalturaVideoPlayerPriv::GetActiveVideoTrack_() const
{
    if (m_pPub->m_selectedVideoTrack >= 0)
    {
        return m_pPub->m_videoTracks[m_pPub->m_selectedVideoTrack];
    }

    return KalturaVideoPlayer::VideoTrackInfo();
}

bool KalturaVideoPlayerPriv::SelectAudioTrack_(uint32_t uID)
{
    if (!playerWrapperBridgeClass)
    {
        return false;
    }

    const auto &track = m_pPub->m_audioTracks[uID];
    jstring jUniqueId = GetEnv_KalturaPlayer()->NewStringUTF(track.uniqueId.GetData());
    GetEnv_KalturaPlayer()->CallStaticVoidMethod(playerWrapperBridgeClass, changeTrackMethodID, jUniqueId);
    return true;
}

std::vector<CYIAbstractVideoPlayer::AudioTrackInfo> KalturaVideoPlayerPriv::GetAudioTracks_() const
{
    std::vector<CYIAbstractVideoPlayer::AudioTrackInfo> result(m_pPub->m_audioTracks.begin(), m_pPub->m_audioTracks.end());
    return result;
}

CYIAbstractVideoPlayer::AudioTrackInfo KalturaVideoPlayerPriv::GetActiveAudioTrack_() const
{
    if (m_pPub->m_selectedAudioTrack >= 0)
    {
        return m_pPub->m_audioTracks[m_pPub->m_selectedAudioTrack];
    }

    return CYIAbstractVideoPlayer::AudioTrackInfo();
}

std::vector<CYIAbstractVideoPlayer::SeekableRange> KalturaVideoPlayerPriv::GetLiveSeekableRanges_() const
{
    std::vector<CYIAbstractVideoPlayer::SeekableRange> ranges;
    return ranges;
}

bool KalturaVideoPlayerPriv::SelectClosedCaptionsTrack_(uint32_t uID)
{
    if (!playerWrapperBridgeClass)
    {
        return false;
    }

    const auto &track = m_pPub->m_closedCaptionsTracks[uID];
    jstring jUniqueId = GetEnv_KalturaPlayer()->NewStringUTF(track.uniqueId.GetData());
    GetEnv_KalturaPlayer()->CallStaticVoidMethod(playerWrapperBridgeClass, changeTrackMethodID, jUniqueId);
    return true;
}

std::vector<CYIAbstractVideoPlayer::ClosedCaptionsTrackInfo> KalturaVideoPlayerPriv::GetClosedCaptionsTracks_() const
{
    std::vector<CYIAbstractVideoPlayer::ClosedCaptionsTrackInfo> result(m_pPub->m_closedCaptionsTracks.begin(), m_pPub->m_closedCaptionsTracks.end());
    return result;
}

CYIAbstractVideoPlayer::ClosedCaptionsTrackInfo KalturaVideoPlayerPriv::GetActiveClosedCaptionsTrack_() const
{
    if (m_pPub->m_selectedClosedCaptionTrack >= 0)
    {
        return m_pPub->m_closedCaptionsTracks[m_pPub->m_selectedClosedCaptionTrack];
    }

    return CYIAbstractVideoPlayer::ClosedCaptionsTrackInfo();
}

void KalturaVideoPlayerPriv::Mute_(bool bMute)
{
    float volume = 1.0;
    if (bMute == true) {
        volume = 0;
    }
    GetEnv_KalturaPlayer()->CallStaticVoidMethod(playerWrapperBridgeClass, setVolumeMethodID, volume);
}

void KalturaVideoPlayerPriv::DisableClosedCaptions_()
{
}

void KalturaVideoPlayerPriv::BringToFront_()
{
    if (!playerWrapperBridgeClass)
    {
        m_bringToFront = true;
        return;
    }

    float index = 1.0;
    GetEnv_KalturaPlayer()->CallStaticVoidMethod(playerWrapperBridgeClass, setZIndexMethodID, index);
}
