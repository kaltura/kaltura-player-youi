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
static const std::string KALTURA_PLAYER_INITIALIZED_EVENT = "KALTURA_PLAYER_INITIALIZED_EVENT";
static const std::string KALTURA_CAN_PLAY_EVENT = "KALTURA_CAN_PLAY_EVENT";
static const std::string KALTURA_REPLAY_EVENT = "KALTURA_REPLAY_EVENT";
static const std::string KALTURA_STOPPED_EVENT = "KALTURA_STOPPED_EVENT";

static const std::string KALTURA_PLAYBACK_RATE_CHANGED_EVENT = "KALTURA_PLAYBACK_RATE_CHANGED_EVENT";
static const std::string KALTURA_SEEKING_EVENT = "KALTURA_SEEKING_EVENT";
static const std::string KALTURA_SEEKED_EVENT = "KALTURA_SEEKED_EVENT";
static const std::string KALTURA_AVAILABLE_VIDEO_TRACKS_CHANGED = "KALTURA_AVAILABLE_VIDEO_TRACKS_CHANGED";
static const std::string KALTURA_CURRENT_PROGRAM_TIME_UPDATED = "KALTURA_CURRENT_PROGRAM_TIME_UPDATED";

static const std::string KALTURA_LOAD_MEDIA_SUCCESS = "KALTURA_LOAD_MEDIA_SUCCESS";
static const std::string KALTURA_VOLUME_CHANGED = "KALTURA_VOLUME_CHANGED";
static const std::string KALTURA_BUFFER_TIME_UPDATED = "KALTURA_BUFFER_TIME_UPDATED";
static const std::string KALTURA_KEEP_SCREEN_ON_CHANGED = "KALTURA_KEEP_SCREEN_ON_CHANGED";

KalturaVideoNativeModule::KalturaVideoNativeModule()
{
    SetSupportedEvents
    ({
        KALTURA_REPLAY_EVENT,
        KALTURA_STOPPED_EVENT,
        KALTURA_PLAYBACK_RATE_CHANGED_EVENT,
        KALTURA_SEEKING_EVENT,
        KALTURA_SEEKED_EVENT,
        KALTURA_LOAD_MEDIA_SUCCESS,
        KALTURA_AVAILABLE_VIDEO_TRACKS_CHANGED,
        KALTURA_CURRENT_PROGRAM_TIME_UPDATED,
        KALTURA_VOLUME_CHANGED,
        KALTURA_BUFFER_TIME_UPDATED,
        KALTURA_KEEP_SCREEN_ON_CHANGED
    });
}

KalturaVideoNativeModule::~KalturaVideoNativeModule() = default;

void KalturaVideoNativeModule::EmitEventPriv(const std::string& event, const folly::dynamic &value)
{
    if (!value.isObject() && !value.isArray())
    {
        EmitEvent(event, value);
    }
    else
    {
        // Wraps the folly object in a nativeEvent to match RN convention.
        folly::dynamic nativeEvent = folly::dynamic::object("nativeEvent", value);
        EmitEvent(event, nativeEvent);
    }
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

            m_pPlayer->PlayerReplayEvent.Connect(*this, [this]() {
                this->EmitEventPriv(KALTURA_REPLAY_EVENT, nullptr);
            });

            m_pPlayer->PlayerStoppedEvent.Connect(*this, [this]() {
                this->EmitEventPriv(KALTURA_STOPPED_EVENT, nullptr);
            });

            m_pPlayer->PlayerReplayEvent.Connect(*this, [this]() {
                this->EmitEventPriv(KALTURA_REPLAY_EVENT, nullptr);
            });

            m_pPlayer->PlaybackRateChangedEvent.Connect(*this, [this](float playbackRate) {
                this->EmitEventPriv(KALTURA_PLAYBACK_RATE_CHANGED_EVENT, playbackRate);
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

                auto tracksArray = ToDynamic(dynamicTracks);
                this->EmitEventPriv(KALTURA_AVAILABLE_VIDEO_TRACKS_CHANGED, tracksArray);
            });

            m_pPlayer->CurrentProgramTimeUpdated.Connect(*this, [this](uint64_t currentProgramTime) {
                this->EmitEventPriv(KALTURA_CURRENT_PROGRAM_TIME_UPDATED, currentProgramTime);
            });

            m_pPlayer->VolumeChanged.Connect(*this, [this](float volume) {
                this->EmitEventPriv(KALTURA_VOLUME_CHANGED, volume);
            });

            m_pPlayer->LoadMediaSuccess.Connect(*this, [this](folly::dynamic data) {
                this->EmitEventPriv(KALTURA_LOAD_MEDIA_SUCCESS, data);
            });

            m_pPlayer->VolumeChanged.Connect(*this, [this](folly::dynamic data) {
                this->EmitEventPriv(KALTURA_VOLUME_CHANGED, data);
            });
            
            m_pPlayer->CurrentBufferTimeUpdated.Connect(*this, [this](uint64_t bufferTime) {
                this->EmitEventPriv(KALTURA_BUFFER_TIME_UPDATED, bufferTime);
            });

            m_pPlayer->KeepDeviceScreenOnUpdated.Connect(*this, [this](bool keepOn) {
                this->EmitEventPriv(KALTURA_KEEP_SCREEN_ON_CHANGED, keepOn);
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

YI_RN_DEFINE_EXPORT_METHOD(KalturaVideoNativeModule, ChangePlaybackRate)(float playbackRate)
{
    if (m_pPlayer)
    {
        m_pPlayer->ChangePlaybackRate(playbackRate);
    }
}

YI_RN_DEFINE_EXPORT_METHOD(KalturaVideoNativeModule, SetPlayerZIndex)(float zIndex)
{
    if (m_pPlayer)
    {
        m_pPlayer->SetPlayerZIndex(zIndex);
    }
}

YI_RN_DEFINE_EXPORT_METHOD(KalturaVideoNativeModule, SetLogLevel)(std::string logLevel)
{
    if (m_pPlayer)
    {
        m_pPlayer->SetLogLevel(logLevel);
    }
}

YI_RN_DEFINE_EXPORT_METHOD(KalturaVideoNativeModule, KeepDeviceScreenOn)(bool keepOn)
{
    if (m_pPlayer)
    {
        m_pPlayer->KeepDeviceScreenOn(keepOn);
    }
}

YI_RN_DEFINE_EXPORT_METHOD(KalturaVideoNativeModule, SelectVideoTrack)(uint32_t trackId)
{
    if (m_pPlayer)
    {
        m_pPlayer->SelectVideoTrack(trackId);
    }
}

