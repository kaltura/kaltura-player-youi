#ifndef _KALTURA_VIDEO_MODULE_H
#define _KALTURA_VIDEO_MODULE_H

#include <youireact/NativeModule.h>
#include <event/YiEventHandler.h>
#include <youireact/modules/EventEmitter.h>

class YI_RN_MODULE(KalturaVideoNativeModule, yi::react::EventEmitterModule)
{
public:
    KalturaVideoNativeModule();
    virtual ~KalturaVideoNativeModule() final;
    
    YI_RN_EXPORT_NAME(KalturaVideo);

    YI_RN_EXPORT_METHOD(ConnectToPlayer)(uint64_t tag);
    
    YI_RN_EXPORT_METHOD(setup)(uint64_t tag, int32_t partnerId, folly::dynamic options);
    YI_RN_EXPORT_METHOD(load)(uint64_t tag, std::string assetId, folly::dynamic options);

  private:
    void EmitEventPriv(const std::string& event, const folly::dynamic &obj);
};

#endif
