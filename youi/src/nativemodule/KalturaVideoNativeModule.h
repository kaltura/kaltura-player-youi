#ifndef _KALTURA_VIDEO_MODULE_H
#define _KALTURA_VIDEO_MODULE_H

#include <youireact/NativeModule.h>
#include <event/YiEventHandler.h>
#include <youireact/modules/EventEmitter.h>

class KalturaVideoPlayer;

class YI_RN_MODULE(KalturaVideoNativeModule, yi::react::EventEmitterModule)
{
public:
    KalturaVideoNativeModule();
    virtual ~KalturaVideoNativeModule() final;

    YI_RN_EXPORT_NAME(KalturaVideo);

    YI_RN_EXPORT_METHOD(ConnectToPlayer)(uint64_t tag);

    YI_RN_EXPORT_METHOD(Setup)(int32_t partnerId, folly::dynamic options);
    YI_RN_EXPORT_METHOD(LoadMedia)(std::string assetId, folly::dynamic options);
    YI_RN_EXPORT_METHOD(SetMedia)(const CYIUrl &videoURI);
        
    YI_RN_EXPORT_METHOD(SelectVideoTrack)(uint32_t trackId);

  private:    
    void EmitEventPriv(const std::string& event, const folly::dynamic &obj);
    
    KalturaVideoPlayer *m_pPlayer;
};

#endif
