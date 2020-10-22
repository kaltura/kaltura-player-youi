#include "player/KalturaVideoPlayer.h"

#include <player/YiVideoPlayerStateManager.h>
#include <framework/YiAppContext.h>
#include <glm/glm.hpp>
#include <renderer/YiSurface.h>
#include <framework/YiScreen.h>

#include "player/KalturaVideoSurface.h"
#include "player/KalturaVideoPlayerPriv.h"

static const CYIString TAG("KalturaVideoPlayer");

YI_TYPE_DEF(KalturaVideoPlayer, CYIAbstractVideoPlayer)

KalturaVideoPlayer::KalturaVideoPlayer()
{
    m_pPriv = std::make_unique<KalturaVideoPlayerPriv>(this);
}

KalturaVideoPlayer::~KalturaVideoPlayer()
{
    m_pPriv->Stop_();
}

void KalturaVideoPlayer::Setup(int32_t partnerId, folly::dynamic options)
{
    m_pPriv->Setup_(partnerId, options);
}

void KalturaVideoPlayer::Load(std::string assetId, folly::dynamic options)
{
    m_pPriv->Load_(assetId, options);
}

bool KalturaVideoPlayer::SelectVideoTrack(uint32_t uID) {
    return m_pPriv->SelectVideoTrack_(uID);
}

std::vector<KalturaVideoPlayer::VideoTrackInfo> KalturaVideoPlayer::GetVideoTracks() {
    return m_pPriv->GetVideoTracks_();
}

KalturaVideoPlayer::VideoTrackInfo KalturaVideoPlayer::GetActiveVideoTrack() {
    return m_pPriv->GetActiveVideoTrack_();
}

CYIString KalturaVideoPlayer::GetName_() const
{
    return m_pPriv->GetName_();
}

CYIString KalturaVideoPlayer::GetVersion_() const
{
    return m_pPriv->GetVersion_();
}

CYIAbstractVideoPlayer::Statistics KalturaVideoPlayer::GetStatistics_() const
{
    return m_pPriv->GetStatistics_();
}

std::unique_ptr<CYIVideoSurface> KalturaVideoPlayer::CreateSurface_()
{
    return m_pPriv->CreateSurface_();
}

void KalturaVideoPlayer::SetVideoRectangle(const YI_RECT_REL &rVideoRectangle)
{
    m_pPriv->SetVideoRectangle(rVideoRectangle);
}

bool KalturaVideoPlayer::SupportsFormat_(CYIAbstractVideoPlayer::StreamingFormat eFormat, CYIAbstractVideoPlayer::DRMScheme eDRMScheme) const
{
    return m_pPriv->SupportsFormat_(eFormat, eDRMScheme);
}

void KalturaVideoPlayer::Prepare_(const CYIUrl &videoURI, CYIAbstractVideoPlayer::StreamingFormat eFormatHint)
{
    m_pPriv->Prepare_(videoURI, eFormatHint);
}

void KalturaVideoPlayer::Play_()
{
    m_pPriv->Play_();
}

void KalturaVideoPlayer::Pause_()
{
    m_pPriv->Pause_();
}

void KalturaVideoPlayer::Stop_()
{
    m_pPriv->Stop_();
}

uint64_t KalturaVideoPlayer::GetDurationMs_() const
{
    return m_pPriv->GetDurationMs_();
}

uint64_t KalturaVideoPlayer::GetCurrentTimeMs_() const
{
    return m_pPriv->GetCurrentTimeMs_();
}

void KalturaVideoPlayer::Seek_(uint64_t uSeekPositionMs)
{
    m_pPriv->Seek_(uSeekPositionMs);
}

void KalturaVideoPlayer::SetMaxBitrate_(uint64_t uMaxBitrate)
{
    m_pPriv->SetMaxBitrate_(uMaxBitrate);
}

bool KalturaVideoPlayer::SelectAudioTrack_(uint32_t uID)
{
    return m_pPriv->SelectAudioTrack_(uID);
}

std::vector<CYIAbstractVideoPlayer::AudioTrackInfo> KalturaVideoPlayer::GetAudioTracks_() const
{
    return m_pPriv->GetAudioTracks_();
}

CYIAbstractVideoPlayer::AudioTrackInfo KalturaVideoPlayer::GetActiveAudioTrack_() const
{
    return m_pPriv->GetActiveClosedCaptionsTrack_();
}

std::vector<CYIAbstractVideoPlayer::SeekableRange> KalturaVideoPlayer::GetLiveSeekableRanges_() const
{
    return m_pPriv->GetLiveSeekableRanges_();
}

bool KalturaVideoPlayer::SelectClosedCaptionsTrack_(uint32_t uID)
{
    return m_pPriv->SelectClosedCaptionsTrack_(uID);
}

std::vector<CYIAbstractVideoPlayer::ClosedCaptionsTrackInfo> KalturaVideoPlayer::GetClosedCaptionsTracks_() const
{
    return m_pPriv->GetClosedCaptionsTracks_();
}

CYIAbstractVideoPlayer::ClosedCaptionsTrackInfo KalturaVideoPlayer::GetActiveClosedCaptionsTrack_() const
{
    return m_pPriv->GetActiveClosedCaptionsTrack_();
}

bool KalturaVideoPlayer::IsMuted_() const
{
    return m_pPriv->IsMuted_();
}

void KalturaVideoPlayer::Mute_(bool bMute)
{
    m_pPriv->Mute_(bMute);
}

void KalturaVideoPlayer::DisableClosedCaptions_()
{
    m_pPriv->DisableClosedCaptions_();
}

/*
 # KalturaVideoPlayerPriv
 This class is responsible for communicating with the Kaltura player implementation in Java.
*/

extern JavaVM *cachedJVM;
extern jobject cachedActivity;

extern "C" {
    JNIEXPORT void JNICALL Java_tv_youi_kalturaplayertest_PKPlayerWrapper_nativeSendEvent(JNIEnv *, jclass, jobject, jstring, jstring);
}

static JNIEnv *GetEnv_KalturaPlayer()
{
    JNIEnv *pEnv;
    cachedJVM->GetEnv(reinterpret_cast<void **>(&pEnv), JNI_VERSION_1_6);
    return pEnv;
}

void JNICALL Java_tv_youi_kalturaplayertest_PKPlayerWrapper_nativeSendEvent(JNIEnv *pEnv, jclass cls, jobject instance, jstring name, jstring payload)
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
    loadMethodID = 0;
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

void KalturaVideoPlayerPriv::HandleEvent(const CYIString& name, folly::dynamic content)
{
    if (name == "playerInitialized")
    {
        YI_LOGD(TAG, "playerInitialized");
    }
    else if (name == "loadMediaSuccess")
    {
        YI_LOGD(TAG, "loadMediaSuccess");
        m_pPub->m_pStateManager->TransitionToMediaReady();
    }
    else if (name == "stateChanged")
    {
        const std::string& state = content["newState"].asString();
        if (state == "READY")
        {
            YI_LOGD(TAG, "stateChanged READY");
            m_pPub->m_pStateManager->TransitionToPlaybackPlaying();
        }
        else if (state == "BUFFERING")
        {
            YI_LOGD(TAG, "stateChanged BUFFERING");
            m_pPub->m_pStateManager->TransitionToPlaybackBuffering();
        }
    }
    else if (name == "play")
    {
        YI_LOGD(TAG, "play");
    }
    else if (name == "pause")
    {
        YI_LOGD(TAG, "pause");
        m_pPub->m_pStateManager->TransitionToPlaybackPaused();
    }
    else if (name == "durationChanged")
    {
        const auto duration = content["duration"].asDouble();
        m_durationMs = static_cast<uint64_t>(duration * 1000);
        m_pPub->DurationChanged.Emit(m_durationMs);
    }
    else if (name == "timeUpdate")
    {
        const auto currentTime = content["position"].asDouble();
        m_currentTimeMs = static_cast<uint64_t>(currentTime * 1000);
        m_pPub->CurrentTimeUpdated.Emit(m_currentTimeMs);
    }
    if (name == "canPlay")
    {
        YI_LOGD(TAG, "canPlay");
    }
    else if (name == "playing")
    {
        YI_LOGD(TAG, "playing");
        m_pPub->m_pStateManager->TransitionToPlaybackPlaying();
    }
    else if (name == "ended")
    {
        YI_LOGD(TAG, "ended");
    }
    else if (name == "stopped")
    {
        YI_LOGD(TAG, "stopped");
    }
    else if (name == "tracksAvailable")
    {
        YI_LOGD(TAG, "tracksAvailable %s", JSONFromDynamic(content).c_str());

        if (!content["video"].isNull())
        {
            auto videoTracks = content["video"];

            for (const auto& track : videoTracks)
            {
                const CYIString uniqueId = track["id"].asString();
                auto bitrate = static_cast<uint64_t>(track["bitrate"].asInt());
                auto width = static_cast<uint32_t>(track["width"].asInt());
                auto height = static_cast<uint32_t>(track["height"].asInt());
                bool isAdaptive = track["isAdaptive"].asBool();
                bool isSelected = track["isSelected"].asBool();

                if (isSelected)
                {
                    m_selectedVideoTrack = static_cast<int32_t>(m_videoTracks.size());
                }

                m_videoTracks.emplace_back(m_videoTracks.size(), uniqueId, bitrate, width, height, isAdaptive, isSelected);
            }

            if (m_videoTracks.size() > 0)
            {
                m_pPub->AvailableVideoTracksChanged.Emit(m_videoTracks);
            }
        }

        if (!content["audio"].isNull())
        {
            auto audioTracks = content["audio"];

            for (const auto& track : audioTracks)
            {
                const CYIString uniqueId = track["id"].asString();

                CYIString label;
                if (track.find("label") != track.items().end() && !track["label"].isNull())
                {
                    label = track["label"].asString();
                }

                CYIString language;
                if (track.find("language") != track.items().end() && !track["language"].isNull())
                {
                    language = track["language"].asString();
                }

                m_audioTracks.emplace_back(m_audioTracks.size(), uniqueId, label, language);

                bool isSelected = track["isSelected"].asBool();
                if (isSelected)
                {
                    m_selectedAudioTrack = static_cast<int32_t>(m_audioTracks.size());
                }
            }
        }

        if (!content["text"].isNull())
        {
            auto textTracks = content["text"];

            for (const auto& track : textTracks)
            {
                const CYIString uniqueId = track["id"].asString();

                CYIString label;
                if (track.find("label") != track.items().end() && !track["label"].isNull())
                {
                    label = track["label"].asString();
                }

                CYIString language;
                if (track.find("language") != track.items().end() && !track["language"].isNull())
                {
                    language = track["language"].asString();
                }

                m_closedCaptionsTracks.emplace_back(
                        m_closedCaptionsTracks.size(), uniqueId, label, language);

                bool isSelected = track["isSelected"].asBool();
                if (isSelected)
                {
                    m_selectedClosedCaptionTrack
                            = static_cast<int32_t>(m_closedCaptionsTracks.size());
                }
            }
        }
    }
    else if (name == "videoTrackChanged")
    {
        YI_LOGD(TAG, "videoTrackChanged");
    }
    else if (name == "audioTrackChanged")
    {
        YI_LOGD(TAG, "audioTrackChanged");
    }
    else if (name == "textTrackChanged")
    {
        YI_LOGD(TAG, "textTrackChanged");
    }
    else if (name == "seeking")
    {
        YI_LOGD(TAG, "seeking");
    }
    else if (name == "seeked")
    {
        YI_LOGD(TAG, "seeked");
    }
    else if (name == "error")
    {
        YI_LOGD(TAG, "error - %s", JSONFromDynamic(content).c_str());

        CYIAbstractVideoPlayer::Error error;
        error.errorCode = CYIAbstractVideoPlayer::ErrorCode::PlaybackError;
        error.message = JSONFromDynamic(content).c_str();

        CYIString errorType = content["errorType"].asString();
        error.nativePlayerErrorCode = errorType;

        m_pPub->ErrorOccurred.Emit(error);

        //m_pPub->m_pStateManager->TransitionToMediaUnloaded();
    }
    else if (name == "adProgress")
    {
        YI_LOGD(TAG, "adProgress");
    }
    else if (name == "adCuepointsChanged")
    {
        YI_LOGD(TAG, "adCuepointsChanged");
    }
    else if (name == "adStarted")
    {
        YI_LOGD(TAG, "adStarted");
    }
    else if (name == "adCompleted")
    {
        YI_LOGD(TAG, "adCompleted");
    }
    else if (name == "adPaused")
    {
        YI_LOGD(TAG, "adPaused");
    }
    else if (name == "adResumed")
    {
        YI_LOGD(TAG, "adResumed");
    }
    else if (name == "adBufferStart")
    {
        YI_LOGD(TAG, "adBufferStart");
    }
    else if (name == "adClicked")
    {
        YI_LOGD(TAG, "adClicked");
    }
    else if (name == "adSkipped")
    {
        YI_LOGD(TAG, "adSkipped");
    }
    else if (name == "adRequested")
    {
        YI_LOGD(TAG, "adRequested");
    }
    else if (name == "adContentPauseRequested")
    {
        YI_LOGD(TAG, "adContentPauseRequested");
    }
    else if (name == "adContentResumeRequested")
    {
        YI_LOGD(TAG, "adContentResumeRequested");
    }
    else if (name == "allAdsCompleted")
    {
        YI_LOGD(TAG, "allAdsCompleted");
    }
    else if (name == "adError")
    {
        YI_LOGD(TAG, "adError %s", JSONFromDynamic(content).c_str());

        CYIAbstractVideoPlayer::Error error;
        error.errorCode = CYIAbstractVideoPlayer::ErrorCode::PlaybackError;
        error.message = JSONFromDynamic(content).c_str();

        CYIString errorType = content["errorType"].asString();
        error.nativePlayerErrorCode = errorType;

        m_pPub->ErrorOccurred.Emit(error);
    }
    else
    {
        YI_LOGW(TAG, "Unhandled event received - %s", name.GetData());
    }
}

void KalturaVideoPlayerPriv::LoadIDs()
{
    jclass tmpBridgeClass = GetEnv_KalturaPlayer()->FindClass("tv/youi/kalturaplayertest/PKPlayerWrapper");
    playerWrapperBridgeClass = reinterpret_cast<jclass>(GetEnv_KalturaPlayer()->NewGlobalRef(tmpBridgeClass));

    if (!playerWrapperBridgeClass)
    {
        return;
    }

    setupMethodID = GetEnv_KalturaPlayer()->GetStaticMethodID(playerWrapperBridgeClass, "setup", "(Ljava/nio/ByteBuffer;ILjava/lang/String;)V");
    loadMethodID = GetEnv_KalturaPlayer()->GetStaticMethodID(playerWrapperBridgeClass, "load", "(Ljava/lang/String;Ljava/lang/String;)V");
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

void KalturaVideoPlayerPriv::Load_(std::string assetId, folly::dynamic options)
{
    if (!playerWrapperBridgeClass)
    {
        return;
    }

    m_pPub->m_pStateManager->TransitionToMediaPreparing();

    jstring jAssetId = GetEnv_KalturaPlayer()->NewStringUTF(assetId.c_str());
    auto jsonOptionsStr = folly::toJson(options);
    jstring optionsStr = GetEnv_KalturaPlayer()->NewStringUTF(jsonOptionsStr.c_str());

    GetEnv_KalturaPlayer()->CallStaticVoidMethod(playerWrapperBridgeClass, loadMethodID, jAssetId, optionsStr);
    GetEnv_KalturaPlayer()->DeleteLocalRef(jAssetId);
    GetEnv_KalturaPlayer()->DeleteLocalRef(optionsStr);
}

CYIString KalturaVideoPlayerPriv::GetName_() const
{
    return "Kaltura Video Player";
}

CYIString KalturaVideoPlayerPriv::GetVersion_() const
{
    return "1";
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

uint64_t KalturaVideoPlayerPriv::GetDurationMs_() const
{
    return m_durationMs;
}

uint64_t KalturaVideoPlayerPriv::GetCurrentTimeMs_() const
{
    return m_currentTimeMs;
}

void KalturaVideoPlayerPriv::Seek_(uint64_t uSeekPositionMs)
{
    if (!playerWrapperBridgeClass)
    {
        return;
    }

    double seekTime = static_cast<double>(uSeekPositionMs) / 1000.f;
    GetEnv_KalturaPlayer()->CallStaticVoidMethod(playerWrapperBridgeClass, seekToMethodID, seekTime);

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

    const auto &track = m_videoTracks[uID];
    jstring jUniqueId = GetEnv_KalturaPlayer()->NewStringUTF(track.uniqueId.GetData());
    GetEnv_KalturaPlayer()->CallStaticVoidMethod(playerWrapperBridgeClass, changeTrackMethodID, jUniqueId);
    return true;
}

std::vector<KalturaVideoPlayer::VideoTrackInfo> KalturaVideoPlayerPriv::GetVideoTracks_() const
{
    return m_videoTracks;
}

KalturaVideoPlayer::VideoTrackInfo KalturaVideoPlayerPriv::GetActiveVideoTrack_() const
{
    if (m_selectedVideoTrack >= 0)
    {
        return m_videoTracks[m_selectedVideoTrack];
    }

    return KalturaVideoPlayer::VideoTrackInfo();
}

bool KalturaVideoPlayerPriv::SelectAudioTrack_(uint32_t uID)
{
    if (!playerWrapperBridgeClass)
    {
        return false;
    }

    const auto &track = m_audioTracks[uID];
    jstring jUniqueId = GetEnv_KalturaPlayer()->NewStringUTF(track.uniqueId.GetData());
    GetEnv_KalturaPlayer()->CallStaticVoidMethod(playerWrapperBridgeClass, changeTrackMethodID, jUniqueId);
    return true;
}

std::vector<CYIAbstractVideoPlayer::AudioTrackInfo> KalturaVideoPlayerPriv::GetAudioTracks_() const
{
    std::vector<CYIAbstractVideoPlayer::AudioTrackInfo> result(m_audioTracks.begin(), m_audioTracks.end());
    return result;
}

CYIAbstractVideoPlayer::AudioTrackInfo KalturaVideoPlayerPriv::GetActiveAudioTrack_() const
{
    if (m_selectedAudioTrack >= 0)
    {
        return m_audioTracks[m_selectedAudioTrack];
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

    const auto &track = m_closedCaptionsTracks[uID];
    jstring jUniqueId = GetEnv_KalturaPlayer()->NewStringUTF(track.uniqueId.GetData());
    GetEnv_KalturaPlayer()->CallStaticVoidMethod(playerWrapperBridgeClass, changeTrackMethodID, jUniqueId);
    return true;
}

std::vector<CYIAbstractVideoPlayer::ClosedCaptionsTrackInfo> KalturaVideoPlayerPriv::GetClosedCaptionsTracks_() const
{
    std::vector<CYIAbstractVideoPlayer::ClosedCaptionsTrackInfo> result(m_closedCaptionsTracks.begin(), m_closedCaptionsTracks.end());
    return result;
}

CYIAbstractVideoPlayer::ClosedCaptionsTrackInfo KalturaVideoPlayerPriv::GetActiveClosedCaptionsTrack_() const
{
    if (m_selectedClosedCaptionTrack >= 0)
    {
        return m_closedCaptionsTracks[m_selectedClosedCaptionTrack];
    }

    return CYIAbstractVideoPlayer::ClosedCaptionsTrackInfo();
}

bool KalturaVideoPlayerPriv::IsMuted_() const
{
    return false;
}

void KalturaVideoPlayerPriv::Mute_(bool bMute)
{
}

void KalturaVideoPlayerPriv::DisableClosedCaptions_()
{
}
