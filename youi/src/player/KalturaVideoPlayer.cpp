#include "player/KalturaVideoPlayer.h"

#include <player/YiVideoPlayerStateManager.h>
#include <player/YiVideoSurface.h>

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

void KalturaVideoPlayer::LoadMedia(std::string assetId, folly::dynamic options)
{
    m_pPriv->LoadMedia_(assetId, options);
}

void KalturaVideoPlayer::SetMedia(const CYIUrl &videoURI)
{
    m_pPriv->SetMedia_(videoURI);
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

void KalturaVideoPlayer::HandleEvent(const CYIString& name, folly::dynamic content)
{
    if (name == "playerInitialized")
    {
        YI_LOGD(TAG, "playerInitialized");
    }
    else if (name == "loadMediaSuccess")
    {
        YI_LOGD(TAG, "loadMediaSuccess");
        m_pStateManager->TransitionToMediaReady();
    }
    else if (name == "stateChanged")
    {
        const std::string& state = content["newState"].asString();
        if (state == "READY")
        {
            YI_LOGD(TAG, "stateChanged READY");
            m_pStateManager->TransitionToPlaybackPlaying();
        }
        else if (state == "BUFFERING")
        {
            YI_LOGD(TAG, "stateChanged BUFFERING");
            m_pStateManager->TransitionToPlaybackBuffering();
        }
    }
    else if (name == "play")
    {
        YI_LOGD(TAG, "play");
    }
    else if (name == "pause")
    {
        YI_LOGD(TAG, "pause");
        m_pStateManager->TransitionToPlaybackPaused();
    }
    else if (name == "durationChanged")
    {
        const auto duration = content["duration"].asDouble();
        m_durationMs = static_cast<uint64_t>(duration * 1000);
        DurationChanged.Emit(m_durationMs);
    }
    else if (name == "timeUpdate")
    {
        const auto currentTime = content["position"].asDouble();
        m_currentTimeMs = static_cast<uint64_t>(currentTime * 1000);
        CurrentTimeUpdated.Emit(m_currentTimeMs);
    }
    if (name == "canPlay")
    {
        YI_LOGD(TAG, "canPlay");
    }
    else if (name == "playing")
    {
        YI_LOGD(TAG, "playing");
        m_pStateManager->TransitionToPlaybackPlaying();
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
                AvailableVideoTracksChanged.Emit(m_videoTracks);
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
    else if (name == "volumeChanged")
    {
        YI_LOGD(TAG, "volumeChanged");
        VolumeChanged.Emit(content);
    }
    else if (name == "error")
    {
        YI_LOGD(TAG, "error - %s", JSONFromDynamic(content).c_str());

        CYIAbstractVideoPlayer::Error error;
        error.errorCode = CYIAbstractVideoPlayer::ErrorCode::PlaybackError;
        error.message = JSONFromDynamic(content).c_str();

        CYIString errorType = content["errorType"].asString();
        error.nativePlayerErrorCode = errorType;

        ErrorOccurred.Emit(error);

        //m_pStateManager->TransitionToMediaUnloaded();
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

        ErrorOccurred.Emit(error);
    }
    else
    {
        YI_LOGW(TAG, "Unhandled event received - %s", name.GetData());
    }
}