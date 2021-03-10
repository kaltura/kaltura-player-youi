#include "player/KalturaVideoPlayer.h"

#include <player/YiVideoPlayerStateManager.h>
#include <player/YiVideoSurface.h>

#include "player/KalturaVideoPlayerPriv.h"

#include <platform/YiDevicePowerManagementBridge.h>
#include <platform/YiDeviceInformationBridge.h>
#include <platform/YiDeviceBridgeLocator.h>

static const CYIString TAG("KalturaVideoPlayer");

static const char *playerInitializedEvent = "playerInitialized";
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
static const char *playbackRateChangedEvent = "playbackRateChanged";
static const char *tracksAvailableEvent = "tracksAvailable";
static const char *videoTrackChangedEvent = "videoTrackChanged";
static const char *audioTrackChangedEvent = "audioTrackChanged";
static const char *textTrackChangedEvent = "textTrackChanged";
static const char *imageTrackChangedEvent = "imageTrackChanged";
static const char *thumbnailInfoResponseEvent = "thumbnailInfoResponse";
static const char *playbackInfoUpdatedEvent = "playbackInfoUpdated";
static const char *seekingEvent = "seeking";
static const char *seekedEvent = "seeked";
static const char *volumeChangedEvent = "volumeChanged";
static const char *loadedTimeRangesEvent = "loadedTimeRanges";

static const char *errorEvent = "error";
static const char *bookmarkErrorEvent = "bookmarkError";
static const char *concurrencyErrorEvent = "concurrencyError";

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
    YI_LOGD(TAG, "construct  KalturaVideoPlayer");
    m_pPriv = std::make_unique<KalturaVideoPlayerPriv>(this);
    CYIDeviceInformationBridge *pDeviceInformationBridge = CYIDeviceBridgeLocator::GetDeviceInformationBridge();
    if (pDeviceInformationBridge)
    {
        m_devicOSName = pDeviceInformationBridge->GetDeviceOSName();
    } else {
        m_devicOSName = "UNKNOWN";
    }
}

KalturaVideoPlayer::~KalturaVideoPlayer()
{
    YI_LOGD(TAG, "destroy ~KalturaVideoPlayer");
    InternalKeepDeviceScreenOn(false);
    m_pPriv->Stop_();
}

void KalturaVideoPlayer::Setup(int32_t partnerId, folly::dynamic options)
{
    uint64_t videoSurfaceWidth = 1920;
    uint64_t videoSurfaceHeight = 1080;

    if (options.find("videoSurfaceWidth") != options.items().end() && !options["videoSurfaceWidth"].isNull()) {
        videoSurfaceWidth = static_cast<uint64_t>(options["videoSurfaceWidth"].asInt());
    }
    if (options.find("videoSurfaceHeight") != options.items().end() && !options["videoSurfaceHeight"].isNull()) {
        videoSurfaceHeight = static_cast<uint64_t>(options["videoSurfaceHeight"].asInt());
    }
    YI_LOGD(TAG, "videoSurfaceWidth - %" PRIu64, videoSurfaceWidth);
    YI_LOGD(TAG, "videoSurfaceHeight - %" PRIu64, videoSurfaceHeight);

    SetVideoSurfaceSize(glm::ivec2(videoSurfaceWidth,videoSurfaceHeight));

    if (options.find("manageKeepScreenOnInternally") != options.items().end() && !options["manageKeepScreenOnInternally"].isNull()) {
        m_manageKeepScreenOnInternally = options["manageKeepScreenOnInternally"].asBool();
    }
    m_pPriv->Setup_(partnerId, options);
}

void KalturaVideoPlayer::LoadMedia(const CYIString &assetId, folly::dynamic options)
{
    InternalKeepDeviceScreenOn(false);
    m_pPriv->LoadMedia_(assetId, options);
}

void KalturaVideoPlayer::SetMedia(const CYIUrl &videoURI)
{
    InternalKeepDeviceScreenOn(false);
    m_pPriv->SetMedia_(videoURI);
}

void KalturaVideoPlayer::KeepDeviceScreenOn(const bool keepOn)
{
    YI_LOGD(TAG, "keepDeviceScreenOn - %d", keepOn);

    CYIDevicePowerManagementBridge *pPowerMgmtBridge = CYIDeviceBridgeLocator::GetDevicePowerManagementBridge();
    if (pPowerMgmtBridge) {
        pPowerMgmtBridge->KeepDeviceScreenOn(keepOn);
        KeepDeviceScreenOnUpdated.Emit(keepOn);
    }
}

void KalturaVideoPlayer::InternalKeepDeviceScreenOn(const bool keepOn)
{
    if (m_manageKeepScreenOnInternally) {
        KeepDeviceScreenOn(keepOn);
    }
}

void KalturaVideoPlayer::SetLogLevel(const CYIString &logLevel)
{
    m_pPriv->SetLogLevel_(logLevel);
}

void KalturaVideoPlayer::Replay()
{
    m_pPriv->Replay_();
}

void KalturaVideoPlayer::ChangePlaybackRate(float playbackRate)
{
    YI_LOGD(TAG, "ChangePlaybackRate %f", playbackRate);
    m_pPriv->ChangePlaybackRate_(playbackRate);
    PlaybackRateChangedEvent.Emit(playbackRate);
}

void KalturaVideoPlayer::SetPlayerZIndex(float zIndex)
{
    YI_LOGD(TAG, "SetPlayerZIndex %f", zIndex);
    m_pPriv->SetPlayerZIndex_(zIndex);
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

bool KalturaVideoPlayer::SelectImageTrack(uint32_t uID) {
    return m_pPriv->SelectImageTrack_(uID);
}

std::vector<KalturaVideoPlayer::ImageTrackInfo> KalturaVideoPlayer::GetImageTracks() {
    return m_pPriv->GetImageTracks_();
}

KalturaVideoPlayer::ImageTrackInfo KalturaVideoPlayer::GetActiveImageTrack() {
    return m_pPriv->GetActiveImageTrack_();
}

void KalturaVideoPlayer::RequestThumbnailInfo(uint64_t position)
{
    YI_LOGD(TAG, "timeUpdateEvent - %" PRIu64, position);
    m_pPriv->RequestThumbnailInfo_(position);
}


CYIString KalturaVideoPlayer::GetName_() const
{
    return m_pPriv->GetName_();
}

CYIString KalturaVideoPlayer::GetVersion_() const
{
    CYIString bridgeVersion = "0.0.1";
    return bridgeVersion;
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
    //YI_LOGD(TAG, "SetVideoRectangle %i,%i,%i,%i", rVideoRectangle.x, rVideoRectangle.y, rVideoRectangle.width, rVideoRectangle.height);
    m_pPriv->SetVideoRectangle(rVideoRectangle);
}

void KalturaVideoPlayer::BringToFront()
{
   YI_LOGD(TAG, "BringToFront");
    m_pPriv->BringToFront_();
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
    if (m_pStateManager->GetPlayerState().mediaState == CYIAbstractVideoPlayer::MediaState::Ready) {
        m_pPriv->Seek_(uSeekPositionMs);
    }
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
    return m_liveSeekableRanges;
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
    return m_isMuted;
}

void KalturaVideoPlayer::Mute_(bool bMute)
{
    YI_LOGD(TAG, "Mute_  = %d", bMute);

    if (bMute == m_isMuted) {
        return;
    }
    
    m_pPriv->Mute_(bMute);
    float volume;
    if (bMute) {
        m_isMuted = true;
        volume = 0.0f;
    } else {
        m_isMuted = false;
        volume = 1.0f;
    }
    
    if (m_devicOSName.Contains("iOS") || m_devicOSName.Contains("tvOS"))
    {
        YI_LOGD(TAG, "%s VolumeChanged isMuted = %d", m_devicOSName.GetData(), m_isMuted);
        VolumeChanged.Emit(volume);
    }
}

void KalturaVideoPlayer::DisableClosedCaptions_()
{
    m_pPriv->DisableClosedCaptions_();
}

void KalturaVideoPlayer::HandleEvent(const CYIString& name, folly::dynamic content)
{

    if (name.Compare(playerInitializedEvent) == 0)
    {
        YI_LOGD(TAG, "playerInitializedEvent");
    }
    else if (name.Compare(loadMediaSuccessEvent) == 0)
    {
        const CYIString id = content["id"].asString();
        YI_LOGD(TAG, "loadMediaSuccessEvent id = %s", id.GetData());
        LoadMediaSuccess.Emit(content);
        m_pStateManager->TransitionToMediaReady();
    }
    else if (name.Compare(loadMediaFailedEvent) == 0)
    {
        YI_LOGD(TAG, "loadMediaFailedEvent");

        CYIAbstractVideoPlayer::Error error;
        error.errorCode = CYIAbstractVideoPlayer::ErrorCode::InitializationError;

        CYIString code = "Unknown";
        CYIString message = "Unknown";

        if (!content.isNull()) {
            if (content.find("code") != content.items().end() && !content["code"].isNull()) {
                code = content["code"].asString();
            }
            if (content.find("message") != content.items().end() && !content["message"].isNull()) {
                message = content["message"].asString();
            }
        }
        YI_LOGD(TAG, "loadMediaFailedEvent code = %s message = %s", code.GetData(), message.GetData());
        error.nativePlayerErrorCode = code;
        error.message = message;

        ErrorOccurred.Emit(error);
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
        InternalKeepDeviceScreenOn(false);
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
       // YI_LOGE(TAG, "timeUpdateEvent - %" PRIu64, m_currentTimeMs);
        CurrentTimeUpdated.Emit(m_currentTimeMs);

        const auto currentBufferTime = content["bufferPosition"].asDouble();
        uint64_t currentBufferTimeMs = static_cast<uint64_t>(currentBufferTime * 1000);
        //YI_LOGE(TAG, "bufferPosition - %" PRIu64, currentBufferTimeMs);
        CurrentBufferTimeUpdated.Emit(currentBufferTimeMs);

        if (!content["currentProgramTime"].isNull()) {
            const auto currentProgramTime = content["currentProgramTime"].asDouble();
            uint64_t currentProgramTimeLong = static_cast<uint64_t>(currentProgramTime);
            //YI_LOGE(TAG, "currentProgramTime - %" PRIu64, currentProgramTimeLong);
            if (currentProgramTimeLong >= 0) {
                CurrentProgramTimeUpdated.Emit(currentProgramTimeLong);
            }
        }
    }
    else if (name.Compare(canPlayEvent) == 0)
    {
        YI_LOGD(TAG, "canPlayEvent");
    }
    else if (name.Compare(playingEvent) == 0)
    {
        YI_LOGD(TAG, "playingEvent");
        if (m_pStateManager->GetPlayerState().mediaState == CYIAbstractVideoPlayer::MediaState::Ready) {
            m_pStateManager->TransitionToPlaybackPlaying();
            InternalKeepDeviceScreenOn(true);
        }
    }
    else if (name.Compare(endedEvent) == 0)
    {
        YI_LOGD(TAG, "endedEvent");
        m_pStateManager->TransitionToPlaybackPaused();
        PlaybackComplete.Emit();
    }
    else if (name.Compare(stoppedEvent) == 0)
    {
        YI_LOGD(TAG, "stoppedEvent");
        PlayerStoppedEvent.Emit();
        InternalKeepDeviceScreenOn(false);
    }
    else if (name.Compare(replayEvent) == 0)
    {
        YI_LOGD(TAG, "replayEvent");
        PlayerReplayEvent.Emit();
    }
    else if (name.Compare(playbackRateChangedEvent) == 0)
    {
        float playbackRate = static_cast<float>(content["playbackRate"].asDouble());
        YI_LOGD(TAG, "playbackRateChangedEvent %f", playbackRate);
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
    else if (name.Compare(playbackInfoUpdatedEvent) == 0)
    {
        YI_LOGD(TAG, "playbackInfoUpdatedEvent");
        if (content.find("videoBitrate") != content.items().end() && !content["videoBitrate"].isNull()) {
            const auto videoBitrate = content["videoBitrate"].asDouble();
            float currentVideoBitrate = static_cast<float>(videoBitrate);
            VideoBitrateChanged.Emit(currentVideoBitrate);
        }
        if (content.find("audioBitrate") != content.items().end() && !content["audioBitrate"].isNull()) {
            const auto audioBitrate = content["audioBitrate"].asDouble();
            float currentAudioBitrate = static_cast<float>(audioBitrate);
            AudioBitrateChanged.Emit(currentAudioBitrate);
        }

        //For iOS - only totalBitrate value is sent
        if (content.find("totalBitrate") != content.items().end() && !content["totalBitrate"].isNull()) {
            const auto totalBitrate = content["totalBitrate"].asDouble();
            float currentTotalBitrate = static_cast<float>(totalBitrate);
            TotalBitrateChanged.Emit(currentTotalBitrate);
        }
    }
    else if (name.Compare(seekingEvent) == 0)
    {
        YI_LOGD(TAG, "seekingEvent");
        const auto targetPosition = content["targetPosition"].asDouble();
        uint64_t targetPositionMs = static_cast<uint64_t>(targetPosition * 1000);
        PlayerSeekingEvent.Emit(targetPositionMs);
    }
    else if (name.Compare(seekedEvent) == 0)
    {
        YI_LOGD(TAG, "seekedEvent");
        PlayerSeekedEvent.Emit();
    }
    else if (name.Compare(volumeChangedEvent) == 0) {
            YI_LOGD(TAG, "volumeChangedEvent");
            if (!content["volume"].isNull()) {
                const auto currentVolume = content["volume"].asDouble();
                float volume = static_cast<float>(currentVolume);

                if (volume <= 0.0f) {
                    m_isMuted = true;
                } else {
                    m_isMuted = false;
                }
                YI_LOGD(TAG, "%s VolumeChanged isMuted = %d", m_devicOSName.GetData(), m_isMuted);
                VolumeChanged.Emit(volume);
            }
    }
    else if (name.Compare(loadedTimeRangesEvent) == 0)
    {
        YI_LOGD(TAG, "loadedTimeRangesEvent - %s", JSONFromDynamic(content).c_str());

        if (!content["timeRanges"].isNull()) {
            m_liveSeekableRanges.clear();
            auto timeRanges = content["timeRanges"];
            for (auto& timeRange : timeRanges)
            {
               const auto start = timeRange["start"].asDouble();
               uint64_t startSeek = static_cast<uint64_t>(start * 1000);
               const auto end = timeRange["end"].asDouble();
               uint64_t endSeek = static_cast<uint64_t>(end * 1000);
               YI_LOGD(TAG, "loadedTimeRangesEvent %f %f", start, end);
               CYIAbstractVideoPlayer::SeekableRange seekableRange = CYIAbstractVideoPlayer::SeekableRange(startSeek,endSeek);
               m_liveSeekableRanges.push_back(seekableRange);
            }
        }
    }
    else if (name.Compare(errorEvent) == 0)
    {
        YI_LOGD(TAG, "errorEvent - %s", JSONFromDynamic(content).c_str());

        CYIAbstractVideoPlayer::Error error;
        error.errorCode = CYIAbstractVideoPlayer::ErrorCode::PlaybackError;
        error.message = JSONFromDynamic(content).c_str();

        CYIString errorType = content["errorType"].asString();
        CYIString errorSeverity = content["errorSeverity"].asString();
        error.nativePlayerErrorCode = errorType;
        ErrorOccurred.Emit(error);
        if (errorSeverity == FATAL) {
            YI_LOGD(TAG, "errorEvent fatal");
            m_pStateManager->TransitionToMediaUnloaded();
        }
    }
    else if (name.Compare(bookmarkErrorEvent) == 0)
    {
        YI_LOGD(TAG, "bookmarkErrorEvent - %s", JSONFromDynamic(content).c_str());

        CYIAbstractVideoPlayer::Error error;
        error.errorCode = CYIAbstractVideoPlayer::ErrorCode::PlaybackError;
        error.message = JSONFromDynamic(content).c_str();

        CYIString errorType = content["errorCode"].asString();
        error.nativePlayerErrorCode = errorType;

        ErrorOccurred.Emit(error);
        m_pStateManager->TransitionToMediaUnloaded();
    }
    else if (name.Compare(concurrencyErrorEvent) == 0)
    {
        YI_LOGD(TAG, "concurrencyErrorEvent - %s", JSONFromDynamic(content).c_str());

        CYIAbstractVideoPlayer::Error error;
        error.errorCode = CYIAbstractVideoPlayer::ErrorCode::PlaybackError;
        error.message = JSONFromDynamic(content).c_str();

        CYIString errorType = content["errorMessage"].asString();
        error.nativePlayerErrorCode = errorType;

        ErrorOccurred.Emit(error);
        m_pStateManager->TransitionToMediaUnloaded();
    }
    else if (name.Compare(adProgressEvent) == 0)
    {
        //YI_LOGD(TAG, "adProgressEvent");
    }
    else if (name.Compare(adCuepointsChangedEvent) == 0)
    {
        YI_LOGD(TAG, "adCuepointsChangedEvent");
    }
    else if (name.Compare(adStartedEvent) == 0)
    {
        YI_LOGD(TAG, "adStartedEvent");
        if (m_pStateManager->GetPlayerState().mediaState == CYIAbstractVideoPlayer::MediaState::Ready) {
            m_pStateManager->TransitionToPlaybackPlaying();
        }
    }
    else if (name.Compare(adCompletedEvent) == 0)
    {
        YI_LOGD(TAG, "adCompletedEvent");
    }
    else if (name.Compare(adPausedEvent) == 0)
    {
        YI_LOGD(TAG, "adPausedEvent");
        m_pStateManager->TransitionToPlaybackPaused();
        InternalKeepDeviceScreenOn(false);
    }
    else if (name.Compare(adResumedEvent) == 0)
    {
        YI_LOGD(TAG, "adResumedEvent");
        if (m_pStateManager->GetPlayerState().mediaState == CYIAbstractVideoPlayer::MediaState::Ready) {
            m_pStateManager->TransitionToPlaybackPlaying();
            InternalKeepDeviceScreenOn(true);
        }
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
        InternalKeepDeviceScreenOn(true);
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


