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

static const std::string KALTURA_AVAILABLE_VIDEO_TRACKS_CHANGED = "KALTURA_AVAILABLE_VIDEO_TRACKS_CHANGED";
static const std::string KALTURA_VOLUME_CHANGED = "KALTURA_VOLUME_CHANGED";
static const std::string KALTURA_AD_CONTENT_PAUSE_REQUESTED = "KALTURA_AD_CONTENT_PAUSE_REQUESTED";
static const std::string KALTURA_AD_CONTENT_RESUME_REQUESTED = "KALTURA_AD_CONTENT_RESUME_REQUESTED";

KalturaVideoNativeModule::KalturaVideoNativeModule()
{
    SetSupportedEvents
    ({
        KALTURA_AD_CONTENT_PAUSE_REQUESTED,
        KALTURA_AD_CONTENT_RESUME_REQUESTED,
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
            
            m_pPlayer->AdContentPauseRequested.Connect(*this, [this]() {
                this->EmitEventPriv(KALTURA_AD_CONTENT_PAUSE_REQUESTED, nullptr);
            });

            m_pPlayer->AdContentResumeRequested.Connect(*this, [this]() {
                this->EmitEventPriv(KALTURA_AD_CONTENT_RESUME_REQUESTED, nullptr);
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

YI_RN_DEFINE_EXPORT_METHOD(KalturaVideoNativeModule, SelectVideoTrack)(uint32_t trackId)
{
    if (m_pPlayer)
    {
        m_pPlayer->SelectVideoTrack(trackId);
    }
}

YI_RN_DEFINE_EXPORT_METHOD(KalturaVideoNativeModule, SetZIndex)(float zIndex)
{
    if (m_pPlayer)
    {
        m_pPlayer->SetZIndex(zIndex);
    }
}
