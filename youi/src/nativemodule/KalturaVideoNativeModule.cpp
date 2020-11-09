#include "KalturaVideoNativeModule.h"

#include <youireact/NativeModuleRegistry.h>
#include <event/YiEventTarget.h>
#include <youireact/IBridge.h>
#include <youireact/ReactBridge.h>
#include <youireact/ReactBridge_inl.h>
#include <youireact/ShadowTree.h>
#include <player/YiAbstractVideoPlayer.h>
#include <youireact/nodes/ShadowVideo.h>

#include "player/KalturaVideoPlayer.h"

#define LOG_TAG "KalturaVideoNativeModule"

YI_RN_INSTANTIATE_MODULE(KalturaVideoNativeModule, yi::react::EventEmitterModule);
YI_RN_REGISTER_MODULE(KalturaVideoNativeModule);

#define TAG "KalturaVideoNativeModule"

static const std::string KALTURA_AD_BREAK_STARTED_EVENT = "KALTURA_AD_BREAK_STARTED_EVENT";
static const std::string KALTURA_AD_BREAK_ENDED_EVENT = "KALTURA_AD_BREAK_ENDED_EVENT";
static const std::string KALTURA_AD_STARTED_EVENT = "KALTURA_AD_STARTED_EVENT";
static const std::string KALTURA_AD_COMPLETED_EVENT = "KALTURA_AD_COMPLETED_EVENT";
static const std::string KALTURA_AD_PAUSED_EVENT = "KALTURA_AD_PAUSED_EVENT";
static const std::string KALTURA_AD_RESUMED_EVENT = "KALTURA_AD_RESUMED_EVENT";
static const std::string KALTURA_AD_BUFFER_START_EVENT = "KALTURA_AD_BUFFER_START_EVENT";
static const std::string KALTURA_AD_SKIPPED_EVENT = "KALTURA_AD_SKIPPED_EVENT";
static const std::string KALTURA_AD_CONTENT_PAUSE_REQUESTED_EVENT = "KALTURA_AD_CONTENT_PAUSE_REQUESTED_EVENT";
static const std::string KALTURA_AD_CONTENT_RESUME_REQUESTED_EVENT = "KALTURA_AD_CONTENT_RESUME_REQUESTED_EVENT";
static const std::string KALTURA_AD_ALL_ADS_COMPLETED_EVENT = "KALTURA_AD_ALL_ADS_COMPLETED_EVENT";

static const std::string KALTURA_AD_PROGRESS_EVENT = "KALTURA_AD_PROGRESS_EVENT";
static const std::string KALTURA_AD_CUEPOINTS_CHANGED_EVENT = "KALTURA_AD_CUEPOINTS_CHANGED_EVENT";
static const std::string KALTURA_AD_CLICKED_EVENT = "KALTURA_AD_CLICKED_EVENT";
static const std::string KALTURA_AD_REQUESTED_EVENT = "KALTURA_AD_REQUESTED_EVENT";

static const std::string KALTURA_CAN_PLAY_EVENT = "KALTURA_CAN_PLAY_EVENT";
static const std::string KALTURA_PLAYING_EVENT = "KALTURA_PLAYING_EVENT";
static const std::string KALTURA_ENDED_EVENT = "KALTURA_ENDED_EVENT";
static const std::string KALTURA_STOPPED_EVENT = "KALTURA_STOPPED_EVENT";
static const std::string KALTURA_REPLAY_EVENT = "KALTURA_REPLAY_EVENT";
static const std::string KALTURA_SEEKING_EVENT = "KALTURA_SEEKING_EVENT";
static const std::string KALTURA_SEEKED_EVENT = "KALTURA_SEEKED_EVENT";
static const std::string KALTURA_AVAILABLE_VIDEO_TRACKS_CHANGED = "KALTURA_AVAILABLE_VIDEO_TRACKS_CHANGED";
static const std::string KALTURA_VOLUME_CHANGED = "KALTURA_VOLUME_CHANGED";

KalturaVideoNativeModule::KalturaVideoNativeModule()
{
    SetSupportedEvents
    ({
        KALTURA_AD_BREAK_STARTED_EVENT,
        KALTURA_AD_BREAK_ENDED_EVENT,
        KALTURA_AD_STARTED_EVENT,
        KALTURA_AD_COMPLETED_EVENT,
        KALTURA_AD_PAUSED_EVENT,
        KALTURA_AD_RESUMED_EVENT,
        KALTURA_AD_BUFFER_START_EVENT,
        KALTURA_AD_SKIPPED_EVENT,
        KALTURA_AD_CONTENT_PAUSE_REQUESTED_EVENT,
        KALTURA_AD_CONTENT_RESUME_REQUESTED_EVENT,
        KALTURA_AD_ALL_ADS_COMPLETED_EVENT,

        KALTURA_AD_PROGRESS_EVENT,
        KALTURA_AD_CUEPOINTS_CHANGED_EVENT,
        KALTURA_AD_CLICKED_EVENT,
        KALTURA_AD_REQUESTED_EVENT,

        KALTURA_CAN_PLAY_EVENT,
        KALTURA_PLAYING_EVENT,
        KALTURA_ENDED_EVENT,
        KALTURA_STOPPED_EVENT,
        KALTURA_REPLAY_EVENT,
        KALTURA_SEEKING_EVENT,
        KALTURA_SEEKED_EVENT,
        KALTURA_AVAILABLE_VIDEO_TRACKS_CHANGED,
        KALTURA_VOLUME_CHANGED
    });
}

KalturaVideoNativeModule::~KalturaVideoNativeModule() = default;

void KalturaVideoNativeModule::EmitEventPriv(const std::string& event, const folly::dynamic &obj)
{
    EmitEvent(event, obj);
}

YI_RN_DEFINE_EXPORT_METHOD(KalturaVideoNativeModule, ConnectToPlayer)(uint64_t tag)
{
    auto &shadowRegistry = GetBridge().GetShadowTree().GetShadowRegistry();
    auto pComponent = shadowRegistry.Get(tag);
    if (pComponent)
    {
        yi::react::ShadowVideo *pShadowVideo = dynamic_cast<yi::react::ShadowVideo *>(pComponent);
        if (pShadowVideo)
        {
            m_pPlayer = dynamic_cast<KalturaVideoPlayer *>(&pShadowVideo->GetPlayer());

            m_pPlayer->AdBreakStartedEvent.Connect(*this, [this]() {
                this->EmitEventPriv(KALTURA_AD_BREAK_STARTED_EVENT, nullptr);
            });

            m_pPlayer->AdBreakEndedEvent.Connect(*this, [this]() {
                this->EmitEventPriv(KALTURA_AD_BREAK_ENDED_EVENT, nullptr);
            });

            m_pPlayer->AdStartedEvent.Connect(*this, [this](folly::dynamic adInfo) {
                this->EmitEventPriv(KALTURA_AD_STARTED_EVENT, adInfo);
            });

            m_pPlayer->AdCompletedEvent.Connect(*this, [this]() {
                this->EmitEventPriv(KALTURA_AD_COMPLETED_EVENT, nullptr);
            });

            m_pPlayer->AdPausedEvent.Connect(*this, [this]() {
                this->EmitEventPriv(KALTURA_AD_PAUSED_EVENT, nullptr);
            });

            m_pPlayer->AdResumedEvent.Connect(*this, [this]() {
                this->EmitEventPriv(KALTURA_AD_RESUMED_EVENT, nullptr);
            });

            m_pPlayer->AdBufferStartEvent.Connect(*this, [this]() {
                this->EmitEventPriv(KALTURA_AD_BUFFER_START_EVENT, nullptr);
            });

            m_pPlayer->AdSkippedEvent.Connect(*this, [this]() {
                this->EmitEventPriv(KALTURA_AD_SKIPPED_EVENT, nullptr);
            });

            m_pPlayer->AdContentPauseRequested.Connect(*this, [this]() {
                this->EmitEventPriv(KALTURA_AD_CONTENT_PAUSE_REQUESTED_EVENT, nullptr);
            });

            m_pPlayer->AdContentResumeRequestedEvent.Connect(*this, [this]() {
                this->EmitEventPriv(KALTURA_AD_CONTENT_RESUME_REQUESTED_EVENT, nullptr);
            });

            m_pPlayer->AllAdsCompletedEvent.Connect(*this, [this]() {
                this->EmitEventPriv(KALTURA_AD_ALL_ADS_COMPLETED_EVENT, nullptr);
            });

            m_pPlayer->AdProgressEvent.Connect(*this, [this](uint64_t adPosition) {
                this->EmitEventPriv(KALTURA_AD_PROGRESS_EVENT, adPosition);
            });

            m_pPlayer->AdCuepointsChangedEvent.Connect(*this, [this](folly::dynamic cuePoints) {
                this->EmitEventPriv(KALTURA_AD_CUEPOINTS_CHANGED_EVENT, cuePoints);
            });

            m_pPlayer->AdClickedEvent.Connect(*this, [this](folly::dynamic clickThruUrl) {
                this->EmitEventPriv(KALTURA_AD_CLICKED_EVENT, clickThruUrl);
            });

            m_pPlayer->AdRequestedEvent.Connect(*this, [this](folly::dynamic adTagUrl) {
                this->EmitEventPriv(KALTURA_AD_REQUESTED_EVENT, adTagUrl);
            });

            m_pPlayer->PlayerCanPlayEvent.Connect(*this, [this]() {
                this->EmitEventPriv(KALTURA_CAN_PLAY_EVENT, nullptr);
            });

            m_pPlayer->PlayerPlayingEvent.Connect(*this, [this]() {
                this->EmitEventPriv(KALTURA_PLAYING_EVENT, nullptr);
            });

            m_pPlayer->PlayerEndedEvent.Connect(*this, [this]() {
                this->EmitEventPriv(KALTURA_ENDED_EVENT, nullptr);
            });

            m_pPlayer->PlayerStoppedEvent.Connect(*this, [this]() {
                this->EmitEventPriv(KALTURA_STOPPED_EVENT, nullptr);
            });

            m_pPlayer->PlayerReplayEvent.Connect(*this, [this]() {
                this->EmitEventPriv(KALTURA_REPLAY_EVENT, nullptr);
            });

            m_pPlayer->PlayerSeekingEvent.Connect(*this, [this](uint64_t targetPosition) {
                this->EmitEventPriv(KALTURA_SEEKING_EVENT, targetPosition);
            });

            m_pPlayer->PlayerSeekedEvent.Connect(*this, [this]() {
                this->EmitEventPriv(KALTURA_SEEKED_EVENT, nullptr);
            });

            m_pPlayer->AvailableVideoTracksChanged.Connect(*this, [this](std::vector<KalturaVideoPlayer::VideoTrackInfo> tracks) {
                std::vector<folly::dynamic> dynamicTracks;
                for (const auto& track : tracks)
                {
                    dynamicTracks.push_back(track.ToDynamic());
                }
                this->EmitEventPriv(KALTURA_AVAILABLE_VIDEO_TRACKS_CHANGED, ToDynamic(dynamicTracks));
            });
            
            m_pPlayer->VolumeChanged.Connect(*this, [this](folly::dynamic data) {
                this->EmitEventPriv(KALTURA_VOLUME_CHANGED, data);
            });
        }
    }
    else
    {
        YI_LOGE(TAG, "Invalid tag provided to connectToPlayer - %" PRIu64, tag);
    }
}

YI_RN_DEFINE_EXPORT_METHOD(KalturaVideoNativeModule, Setup)(int32_t partnerId, folly::dynamic options)
{
    if (m_pPlayer)
    {
        m_pPlayer->Setup(partnerId, options);
    }
}

YI_RN_DEFINE_EXPORT_METHOD(KalturaVideoNativeModule, LoadMedia)(std::string assetId, folly::dynamic options)
{
    if (m_pPlayer)
    {
        m_pPlayer->LoadMedia(assetId, options);
    }
}

YI_RN_DEFINE_EXPORT_METHOD(KalturaVideoNativeModule, SetMedia)(const CYIUrl &videoURI)
{
    if (m_pPlayer)
    {
        m_pPlayer->SetMedia(videoURI);
    }
}

YI_RN_DEFINE_EXPORT_METHOD(KalturaVideoNativeModule, Replay)()
{
    if (m_pPlayer)
    {
        m_pPlayer->Replay();
    }
}

YI_RN_DEFINE_EXPORT_METHOD(KalturaVideoNativeModule, SelectVideoTrack)(uint32_t trackId)
{
    if (m_pPlayer)
    {
        m_pPlayer->SelectVideoTrack(trackId);
    }
}
