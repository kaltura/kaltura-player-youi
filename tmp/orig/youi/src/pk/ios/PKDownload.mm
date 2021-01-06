#include "../PKDownload.h"

YI_RN_INSTANTIATE_MODULE(PKDownloadWrapper, yi::react::EventEmitterModule);
YI_RN_REGISTER_MODULE(PKDownloadWrapper);

PKDownloadWrapper::PKDownloadWrapper()
{
    SetSupportedEvents(PKDOWNLOAD_SUPPORTED_EVENTS);
}

PKDownloadWrapper::~PKDownloadWrapper()
{
}

YI_RN_DEFINE_EXPORT_METHOD(PKDownloadWrapper, setup)(int partnerId, std::string serverUrl)
{
}

YI_RN_DEFINE_EXPORT_METHOD(PKDownloadWrapper, offline_start)(std::string assetId, dynamic data)
{
}

YI_RN_DEFINE_EXPORT_METHOD(PKDownloadWrapper, offline_pause)(std::string assetId)
{
}

YI_RN_DEFINE_EXPORT_METHOD(PKDownloadWrapper, offline_resume)(std::string assetId)
{
}

YI_RN_DEFINE_EXPORT_METHOD(PKDownloadWrapper, offline_remove)(std::string assetId)
{
}
