#include "player/KalturaVideoPlayer.h"

#include <player/YiVideoPlayerStateManager.h>
#include <player/YiVideoSurface.h>

#include "player/KalturaVideoPlayerPriv.h"

static const CYIString TAG("KalturaVideoPlayer");

static const char *loadMediaSuccessEvent = "loadMediaSuccess";
static const char *loadMediaFailedEvent = "loadMediaFailed";
static const char *stateChangedEvent = "stateChanged";
static const char *playEvent = "play";
static const char *pauseEvent = "pause";
static const char *durationChangedEvent = "durationChanged";
static const char *timeUpdateEvent = "timeUpdate";
static const char *canPlayEvent = "canPlay";
static const char *playingEvent = "playing";
static const char *endedEvent = "ended";
static const char *stoppedEvent = "stopped";
static const char *replayEvent = "replay";
static const char *tracksAvailableEvent = "tracksAvailable";
static const char *videoTrackChangedEvent = "videoTrackChanged";
static const char *audioTrackChangedEvent = "audioTrackChanged";
static const char *textTrackChangedEvent = "textTrackChanged";
static const char *seekingEvent = "seeking";
static const char *seekedEvent = "seeked";
static const char *volumeChangedEvent = "volumeChanged";
static const char *errorEvent = "error";
static const char *adProgressEvent = "adProgress";
static const char *adCuepointsChangedEvent = "adCuepointsChanged";
static const char *adStartedEvent = "adStarted";
static const char *adCompletedEvent = "adCompleted";
static const char *adPausedEvent = "adPaused";
static const char *adResumedEvent = "adResumed";
static const char *adBufferStartEvent = "adBufferStart";
static const char *adClickedEvent = "adClicked";
static const char *adSkippedEvent = "adSkipped";
static const char *adRequestedEvent = "adRequested";
static const char *adContentPauseRequestedEvent = "adContentPauseRequested";
static const char *adContentResumeRequestedEvent = "adContentResumeRequested";
static const char *allAdsCompletedEvent = "allAdsCompleted";
static const char *adErrorEvent = "adError";


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

void KalturaVideoPlayer::LoadMedia(const CYIString &assetId, folly::dynamic options)
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
    if (m_currentVideoRectangle != rVideoRectangle) {
        m_pPriv->SetVideoRectangle(rVideoRectangle);
        m_currentVideoRectangle = rVideoRectangle;
    }
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

    if (name.Compare(loadMediaSuccessEvent) == 0)
    {
        YI_LOGD(TAG, "loadMediaSuccessEvent");
        m_pStateManager->TransitionToMediaReady();
    }
    else if (name.Compare(loadMediaFailedEvent) == 0)
    {
        YI_LOGD(TAG, "loadMediaFailedEvent");
        if (!content.isNull()) {
            const CYIString code = content["code"].asString();
            const CYIString extra = content["extra"].asString();
            const CYIString message = content["message"].asString();
            const CYIString errorName = content["name"].asString();
            YI_LOGD(TAG, "loadMediaFailedEvent message = <%s>", message.GetData());
        }
        
        m_pStateManager->TransitionToMediaUnloaded();
//Example:
//        //buffering / playing // paused
//        if (m_pStateManager->GetPlayerState().playbackState == CYIAbstractVideoPlayer::PlaybackState::Buffering) {
//
//        }
//        // unloaded // prepering // ready
//        if (m_pStateManager->GetPlayerState().mediaState == CYIAbstractVideoPlayer::MediaState::Unloaded) {
//
//        }
    }
    else if (name.Compare(stateChangedEvent) == 0)
    {
        const CYIString &state = content["newState"].asString();
        YI_LOGD(TAG, "stateChangedEvent %s", state.GetData());

        if (state == "BUFFERING")
        {
            m_pStateManager->TransitionToPlaybackBuffering();
        }
    }
    else if (name.Compare(playEvent) == 0)
    {
        YI_LOGD(TAG, "playEvent");
    }
    else if (name.Compare(pauseEvent) == 0)
    {
        YI_LOGD(TAG, "pauseEvent");
        m_pStateManager->TransitionToPlaybackPaused();
    }
    else if (name.Compare(durationChangedEvent) == 0)
    {
        YI_LOGD(TAG, "durationChangedEvent");
        const auto duration = content["duration"].asDouble();
        m_durationMs = static_cast<uint64_t>(duration * 1000);
        DurationChanged.Emit(m_durationMs);
    }
    else if (name.Compare(timeUpdateEvent) == 0)
    {
        const auto currentTime = content["position"].asDouble();
        m_currentTimeMs = static_cast<uint64_t>(currentTime * 1000);
        //YI_LOGW(TAG, "timeUpdateEvent");
        CurrentTimeUpdated.Emit(m_currentTimeMs);

        if (!content["bufferPosition"].isNull()) {
            const auto currentBufferTime = content["bufferPosition"].asDouble();
            //YI_LOGW(TAG, "buffer");
            uint64_t currentBufferTimeMs = static_cast<uint64_t>(currentBufferTime * 1000);
            CurrentBufferTimeUpdated.Emit(currentBufferTimeMs);
        }
    }
    else if (name.Compare(canPlayEvent) == 0)
    {
        YI_LOGD(TAG, "canPlayEvent");
    }
    else if (name.Compare(playingEvent) == 0)
    {
        YI_LOGD(TAG, "playingEvent");
        m_pStateManager->TransitionToPlaybackPlaying();
    }
    else if (name.Compare(endedEvent) == 0)
    {
        YI_LOGD(TAG, "endedEvent");
    }
    else if (name.Compare(stoppedEvent) == 0)
    {
        YI_LOGD(TAG, "stoppedEvent");
    }
    else if (name.Compare(replayEvent) == 0)
    {
        YI_LOGD(TAG, "replayEvent");
    }
    else if (name.Compare(tracksAvailableEvent) == 0)
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
    else if (name.Compare(videoTrackChangedEvent) == 0)
    {
        YI_LOGD(TAG, "videoTrackChangedEvent");
    }
    else if (name.Compare(audioTrackChangedEvent) == 0)
    {
        YI_LOGD(TAG, "audioTrackChangedEvent");
    }
    else if (name.Compare(textTrackChangedEvent) == 0)
    {
        YI_LOGD(TAG, "textTrackChangedEvent");
    }
    else if (name.Compare(seekingEvent) == 0)
    {
        YI_LOGD(TAG, "seekingEvent");
    }
    else if (name.Compare(seekedEvent) == 0)
    {
        YI_LOGD(TAG, "seekedEvent");
    }
    else if (name.Compare(volumeChangedEvent) == 0) {
            YI_LOGD(TAG, "volumeChangedEvent");
            if (!content["volume"].isNull()) {
                float volume = static_cast<float>(content["volume"].asDouble());
                VolumeChanged.Emit(volume);
            }
    }
    else if (name.Compare(errorEvent) == 0)
    {
        YI_LOGD(TAG, "errorEvent - %s", JSONFromDynamic(content).c_str());

        CYIAbstractVideoPlayer::Error error;
        error.errorCode = CYIAbstractVideoPlayer::ErrorCode::PlaybackError;
        error.message = JSONFromDynamic(content).c_str();

        CYIString errorType = content["errorType"].asString();
        error.nativePlayerErrorCode = errorType;
        if (!content["errorSeverity"].isNull()) {
            CYIString errorSeverity = content["errorSeverity"].asString();
            ErrorOccurred.Emit(error);
            if (errorSeverity == "fatal") {
                YI_LOGD(TAG, "errorEvent fatal");
                m_pStateManager->TransitionToMediaUnloaded();
            }
        } else {
            m_pStateManager->TransitionToMediaUnloaded();
        }
    }
    else if (name.Compare(adProgressEvent) == 0)
    {
        YI_LOGD(TAG, "adProgressEvent");
    }
    else if (name.Compare(adCuepointsChangedEvent) == 0)
    {
        YI_LOGD(TAG, "adCuepointsChangedEvent");
    }
    else if (name.Compare(adStartedEvent) == 0)
    {
        YI_LOGD(TAG, "adStartedEvent");
        m_pStateManager->TransitionToPlaybackPlaying();
    }
    else if (name.Compare(adCompletedEvent) == 0)
    {
        YI_LOGD(TAG, "adCompletedEvent");
    }
    else if (name.Compare(adPausedEvent) == 0)
    {
        YI_LOGD(TAG, "adPausedEvent");
        m_pStateManager->TransitionToPlaybackPaused();
    }
    else if (name.Compare(adResumedEvent) == 0)
    {
        YI_LOGD(TAG, "adResumedEvent");
         m_pStateManager->TransitionToPlaybackPlaying();
    }
    else if (name.Compare(adBufferStartEvent) == 0)
    {
        YI_LOGD(TAG, "adBufferStartEvent");
    }
    else if (name.Compare(adClickedEvent) == 0)
    {
        YI_LOGD(TAG, "adClickedEvent");
    }
    else if (name.Compare(adSkippedEvent) == 0)
    {
        YI_LOGD(TAG, "adSkipped");
    }
    else if (name.Compare(adRequestedEvent) == 0)
    {
        YI_LOGD(TAG, "adRequestedEvent");
    }
    else if (name.Compare(adContentPauseRequestedEvent) == 0)
    {
        YI_LOGD(TAG, "adContentPauseRequested");
    }
    else if (name.Compare(adContentResumeRequestedEvent) == 0)
    {
        YI_LOGD(TAG, "adContentResumeRequestedEvent");
    }
    else if (name.Compare(allAdsCompletedEvent) == 0)
    {
        YI_LOGD(TAG, "allAdsCompletedEvent");
    }
    else if (name.Compare(adErrorEvent) == 0)
    {
        YI_LOGD(TAG, "adErrorEvent %s", JSONFromDynamic(content).c_str());

        CYIAbstractVideoPlayer::Error error;
        error.errorCode = CYIAbstractVideoPlayer::ErrorCode::PlaybackError;
        error.message = JSONFromDynamic(content).c_str();

        CYIString errorType = content["errorType"].asString();
        error.nativePlayerErrorCode = errorType;

        ErrorOccurred.Emit(error);
    }
    else
    {
        YI_LOGW(TAG, "Unhandled event received - <%s>", name.GetData());
    }
}



