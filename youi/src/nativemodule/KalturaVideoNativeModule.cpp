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

static const std::string PLAYER_EVENT = "PLAYER_EVENT";

KalturaVideoNativeModule::KalturaVideoNativeModule()
{
  SetSupportedEvents
    ({
       PLAYER_EVENT
    });
}

KalturaVideoNativeModule::~KalturaVideoNativeModule() = default;

void KalturaVideoNativeModule::EmitEventPriv(const folly::dynamic &obj)
{
    EmitEvent(PLAYER_EVENT, obj);
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
//            auto pPlayer = dynamic_cast<KalturaVideoPlayer *>(&pShadowVideo->GetPlayer());
//            pPlayer->KalturaSignal.Connect(*this, [this](bool bSomething) {
//
//              // how to we get this to the JS layer
//              this->EmitEventPriv(folly::dynamic::object("name", "KalturaSignal")("bSomething", bSomething));
//            });

        }
    }
}

YI_RN_DEFINE_EXPORT_METHOD(KalturaVideoNativeModule, setup)(uint64_t tag, int32_t partnerId, folly::dynamic options)
{
      // We need to find the player implementation
      auto &shadowRegistry = GetBridge().GetShadowTree().GetShadowRegistry();
      auto pComponent = shadowRegistry.Get(tag);
      if (pComponent)
      {
          yi::react::ShadowVideo *pShadowVideo = dynamic_cast<yi::react::ShadowVideo *>(pComponent);
          if (pShadowVideo)
          {
              auto pPlayer = dynamic_cast<KalturaVideoPlayer *>(&pShadowVideo->GetPlayer());
              pPlayer->Setup(partnerId, options);
          }
      }
}

YI_RN_DEFINE_EXPORT_METHOD(KalturaVideoNativeModule, load)(uint64_t tag, std::string assetId, folly::dynamic options)
{
      // We need to find the player implementation
      auto &shadowRegistry = GetBridge().GetShadowTree().GetShadowRegistry();
      auto pComponent = shadowRegistry.Get(tag);
      if (pComponent)
      {
          yi::react::ShadowVideo *pShadowVideo = dynamic_cast<yi::react::ShadowVideo *>(pComponent);
          if (pShadowVideo)
          {
              auto pPlayer = dynamic_cast<KalturaVideoPlayer *>(&pShadowVideo->GetPlayer());
              pPlayer->Load(assetId, options);
          }
      }
}
