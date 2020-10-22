
#import "player/KalturaVideoPlayer.h"
#import "player/KalturaVideoSurface.h"

#include <player/YiVideoPlayerStateManager.h>
#include <framework/YiAppContext.h>
#include <glm/glm.hpp>
#include <renderer/YiSurface.h>
#include <framework/YiScreen.h>

#import <apple/YiRootViewController.h>

#import <Foundation/Foundation.h>
#import <objc/runtime.h>
#import <UIKit/UIKit.h>

#include "player/KalturaVideoPlayerPriv.h"

static const CYIString TAG("KalturaVideoPlayer");

YI_TYPE_DEF(KalturaVideoPlayer, CYIAbstractVideoPlayer)

KalturaVideoPlayer::KalturaVideoPlayer()
{
    m_pPriv = std::make_unique<KalturaVideoPlayerPriv>(this);
}

KalturaVideoPlayer::~KalturaVideoPlayer()
{
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

/*
 KalturaVideoPlayerPriv
 */

// Copied from https://github.com/facebook/react-native/blob/master/React/CxxUtils/RCTFollyConvert.mm (MIT)
id convertFollyDynamicToId(const folly::dynamic &dyn)
{
    // I could imagine an implementation which avoids copies by wrapping the
    // dynamic in a derived class of NSDictionary.  We can do that if profiling
    // implies it will help.

    switch (dyn.type()) {
        case folly::dynamic::NULLT:
            return (id)kCFNull;
        case folly::dynamic::BOOL:
            return dyn.getBool() ? @YES : @NO;
        case folly::dynamic::INT64:
            return @(dyn.getInt());
        case folly::dynamic::DOUBLE:
            return @(dyn.getDouble());
        case folly::dynamic::STRING:
            return [[NSString alloc] initWithBytes:dyn.c_str() length:dyn.size() encoding:NSUTF8StringEncoding];
        case folly::dynamic::ARRAY: {
            NSMutableArray *array = [[NSMutableArray alloc] initWithCapacity:dyn.size()];
            for (auto &elem : dyn) {
                [array addObject:convertFollyDynamicToId(elem)];
            }
            return array;
        }
        case folly::dynamic::OBJECT: {
            NSMutableDictionary *dict = [[NSMutableDictionary alloc] initWithCapacity:dyn.size()];
            for (auto &elem : dyn.items()) {
                dict[convertFollyDynamicToId(elem.first)] = convertFollyDynamicToId(elem.second);
            }
            return dict;
        }
    }
}

folly::dynamic convertIdToFollyDynamic(id json)
{
    if (json == nil || json == (id)kCFNull) {
        return nullptr;
    } else if ([json isKindOfClass:[NSNumber class]]) {
        const char *objCType = [json objCType];
        switch (objCType[0]) {
                // This is a c++ bool or C99 _Bool.  On some platforms, BOOL is a bool.
            case _C_BOOL:
                return (bool)[json boolValue];
            case _C_CHR:
                // On some platforms, objc BOOL is a signed char, but it
                // might also be a small number.  Use the same hack JSC uses
                // to distinguish them:
                // https://phabricator.intern.facebook.com/diffusion/FBS/browse/master/fbobjc/xplat/third-party/jsc/safari-600-1-4-17/JavaScriptCore/API/JSValue.mm;b8ee03916489f8b12143cd5c0bca546da5014fc9$901
                if ([json isKindOfClass:[@YES class]]) {
                    return (bool)[json boolValue];
                } else {
                    return [json longLongValue];
                }
            case _C_UCHR:
            case _C_SHT:
            case _C_USHT:
            case _C_INT:
            case _C_UINT:
            case _C_LNG:
            case _C_ULNG:
            case _C_LNG_LNG:
            case _C_ULNG_LNG:
                return [json longLongValue];

            case _C_FLT:
            case _C_DBL:
                return [json doubleValue];

                // default:
                //   fall through
        }
    } else if ([json isKindOfClass:[NSString class]]) {
        NSData *data = [json dataUsingEncoding:NSUTF8StringEncoding];
        return std::string(reinterpret_cast<const char *>(data.bytes), data.length);
    } else if ([json isKindOfClass:[NSArray class]]) {
        folly::dynamic array = folly::dynamic::array;
        for (id element in json) {
            array.push_back(convertIdToFollyDynamic(element));
        }
        return array;
    } else if ([json isKindOfClass:[NSDictionary class]]) {
        __block folly::dynamic object = folly::dynamic::object();

        [json enumerateKeysAndObjectsUsingBlock:^(NSString *key, NSString *value, __unused BOOL *stop) {
            object.insert(convertIdToFollyDynamic(key), convertIdToFollyDynamic(value));
        }];

        return object;
    }

    return nil;
}

static NSString* nsstring(folly::dynamic str) {
    if (str.isString()) {
        return [NSString stringWithUTF8String:str.c_str()];
    }
    return nil;
}

static NSString* nsstring(std::string str) {
    return [NSString stringWithUTF8String:str.c_str()];
}

@implementation EventSender

+(instancetype)newWithWrapper:(KalturaVideoPlayerPriv*)wrapper {
    EventSender* sender = [EventSender new];
    sender->wrapper = wrapper;
    return sender;
}

-(void)sendEvent:(NSString*)name payload:(id _Nullable)payload {
    folly::dynamic dynPayload = convertIdToFollyDynamic(payload);
    reinterpret_cast<KalturaVideoPlayerPriv*>(wrapper)->Emit_(name.UTF8String, dynPayload);
}

@end

KalturaVideoPlayerPriv::KalturaVideoPlayerPriv(KalturaVideoPlayer *pPub)
: m_pPub(pPub)
{
}

KalturaVideoPlayerPriv::~KalturaVideoPlayerPriv()
{
    if (m_player)
    {
        [m_player destroy];
        m_player = nullptr;
    }
}

void KalturaVideoPlayerPriv::Setup_(int32_t partnerId, folly::dynamic options)
{
    NSLog(@"*** setup(%d, %s)", partnerId, JSONFromDynamic(options).c_str());

    if (m_player) {
        return;
    }

    UIView* parentView = YiRootViewController.sharedInstance.view;
    parentView.backgroundColor = UIColor.clearColor;

    EventSender* sender = [EventSender newWithWrapper:this];
    m_player = [[KalturaPlayerYI alloc] initWithPartnerId:partnerId options:convertFollyDynamicToId(options)
                                               parentView:parentView eventSender:sender];
}

void KalturaVideoPlayerPriv::LoadMedia_(std::string assetId, folly::dynamic options)
{
    NSLog(@"*** LoadMedia_(%s, %s)", assetId.c_str(), JSONFromDynamic(options).c_str());

    m_pPub->m_pStateManager->TransitionToMediaPreparing();
    [m_player loadMedia:nsstring(assetId) options:convertFollyDynamicToId(options)];
}

void KalturaVideoPlayerPriv::SetMedia_(const CYIUrl &videoURI)
{
    m_pPub->m_pStateManager->TransitionToMediaPreparing();
    NSURL *url = [NSURL URLWithString:videoURI.ToString().ToNSString()];
    NSLog(@"*** SetMedia_(%s)", videoURI.ToString().ToStdString().c_str());
    [m_player setMedia:url];
}

void KalturaVideoPlayerPriv::Emit_(const std::string &name, const folly::dynamic &content)
{
    if (name == "playerInitialized")
    {
        YI_LOGD(TAG, "playerInitialized");
    }
    else if (name == "loadMediaSuccess")
    {
        YI_LOGD(TAG, "loadMediaSuccess");
        m_pPub->m_pStateManager->TransitionToMediaReady();
    }
    else if (name == "stateChanged")
    {
        const std::string& state = content["newState"].asString();
        if (state == "READY")
        {
            YI_LOGD(TAG, "stateChanged READY");
            m_pPub->m_pStateManager->TransitionToPlaybackPlaying();
        }
        else if (state == "BUFFERING")
        {
            YI_LOGD(TAG, "stateChanged BUFFERING");
            m_pPub->m_pStateManager->TransitionToPlaybackBuffering();
        }
    }
    else if (name == "play")
    {
        YI_LOGD(TAG, "play");
    }
    else if (name == "pause")
    {
        YI_LOGD(TAG, "pause");
        m_pPub->m_pStateManager->TransitionToPlaybackPaused();
    }
    else if (name == "durationChanged")
    {
        const auto duration = content["duration"].asDouble();
        m_durationMs = static_cast<uint64_t>(duration * 1000);
        m_pPub->DurationChanged.Emit(m_durationMs);
    }
    else if (name == "timeUpdate")
    {
        const auto currentTime = content["position"].asDouble();
        m_currentTimeMs = static_cast<uint64_t>(currentTime * 1000);
        m_pPub->CurrentTimeUpdated.Emit(m_currentTimeMs);
    }
    if (name == "canPlay")
    {
        YI_LOGD(TAG, "canPlay");
    }
    else if (name == "playing")
    {
        YI_LOGD(TAG, "playing");
        m_pPub->m_pStateManager->TransitionToPlaybackPlaying();
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
        
        if (content.find("video") != content.items().end() && !content["video"].isNull())
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
            
            m_pPub->AvailableVideoTracksChanged.Emit(m_videoTracks);
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
        m_pPub->VolumeChanged.Emit(content);
    }
    else if (name == "error")
    {
        YI_LOGD(TAG, "error - %s", JSONFromDynamic(content).c_str());

        CYIAbstractVideoPlayer::Error error;
        error.errorCode = CYIAbstractVideoPlayer::ErrorCode::PlaybackError;
        error.message = JSONFromDynamic(content).c_str();

        m_pPub->ErrorOccurred.Emit(error);

        //m_pPub->m_pStateManager->TransitionToMediaUnloaded();
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

        m_pPub->ErrorOccurred.Emit(error);
    }
    else
    {
        YI_LOGW(TAG, "Unhandled event received - %s", name.c_str());
    }
}

CYIString KalturaVideoPlayerPriv::GetName_() const
{
    return "Kaltura Video Player";
}

CYIString KalturaVideoPlayerPriv::GetVersion_() const
{
    return "1";
}

CYIAbstractVideoPlayer::Statistics KalturaVideoPlayerPriv::GetStatistics_() const
{
    CYIAbstractVideoPlayer::Statistics stats;
    return stats;
}

std::unique_ptr<CYIVideoSurface> KalturaVideoPlayerPriv::CreateSurface_()
{
    return std::unique_ptr<CYIVideoSurface>(new KalturaVideoSurface(m_pPub));
}

void KalturaVideoPlayerPriv::SetVideoRectangle(const YI_RECT_REL &rVideoRectangle)
{
    // The rectangle coming in is defined in the coordinate system of the You.i Engine view
    // This is not necessarily screen space, because we position the view below the status bar on iOS.
    // We need to adjust the rectangle to actually be in screen space, accounting for the origin point of the renderSurfaceView.
    CGPoint renderSurfaceOrigin = [YiRootViewController sharedInstance].renderSurfaceView.frame.origin;

    float fScreenScale = [UIScreen mainScreen].scale;

    CGRect frame;
    frame.origin.x = (rVideoRectangle.x / fScreenScale) + renderSurfaceOrigin.x;
    frame.origin.y = (rVideoRectangle.y / fScreenScale) + renderSurfaceOrigin.y;
    frame.size.width = rVideoRectangle.width / fScreenScale;
    frame.size.height = rVideoRectangle.height / fScreenScale;

    //we disable animations in this block so the video frame snaps directly where we want it
    [CATransaction begin];
    [CATransaction setAnimationDuration:0.0];

    [m_player setFrame:frame];

    [CATransaction commit];
}

bool KalturaVideoPlayerPriv::SupportsFormat_(CYIAbstractVideoPlayer::StreamingFormat eFormat, CYIAbstractVideoPlayer::DRMScheme eDRMScheme) const
{
    YI_UNUSED(eDRMScheme);
    return true;
}

void KalturaVideoPlayerPriv::Prepare_(const CYIUrl &videoURI, CYIAbstractVideoPlayer::StreamingFormat /*eFormatHint*/)
{
    YI_UNUSED(videoURI);
}

void KalturaVideoPlayerPriv::Play_()
{
    if (m_player)
    {
        [m_player play];
    }
}

void KalturaVideoPlayerPriv::Pause_()
{
    if (m_player)
    {
        [m_player pause];
    }
}

void KalturaVideoPlayerPriv::Stop_()
{
    if (m_player)
    {
        [m_player stop];
    }
}

uint64_t KalturaVideoPlayerPriv::GetDurationMs_() const
{
    return m_durationMs;
}

uint64_t KalturaVideoPlayerPriv::GetCurrentTimeMs_() const
{
    return m_currentTimeMs;
}

void KalturaVideoPlayerPriv::Seek_(uint64_t uSeekPositionMs)
{
    if (m_player)
    {
        double seekTime = static_cast<double>(uSeekPositionMs) / 1000.f;
        [m_player seekTo:seekTime];
    }
}

void KalturaVideoPlayerPriv::SetMaxBitrate_(uint64_t uMaxBitrate)
{

}

bool KalturaVideoPlayerPriv::SelectVideoTrack_(uint32_t uID)
{
    YI_UNUSED(uID);
    YI_LOGW(TAG, "SelectVideoTrack is not available on iOS");
    return false;
}

std::vector<KalturaVideoPlayer::VideoTrackInfo> KalturaVideoPlayerPriv::GetVideoTracks_() const
{
    YI_LOGW(TAG, "GetVideoTracks is not available on iOS");
    return std::vector<KalturaVideoPlayer::VideoTrackInfo>();
}

KalturaVideoPlayer::VideoTrackInfo KalturaVideoPlayerPriv::GetActiveVideoTrack_() const
{
    YI_LOGW(TAG, "GetActiveVideoTrack is not available on iOS");
    return KalturaVideoPlayer::VideoTrackInfo();
}

bool KalturaVideoPlayerPriv::SelectAudioTrack_(uint32_t uID)
{
    if (!m_player)
    {
        return false;
    }

    const auto &track = m_audioTracks[uID];
    [m_player selectTrackWithTrackId:track.uniqueId.ToNSString()];
    return true;
}

std::vector<CYIAbstractVideoPlayer::AudioTrackInfo> KalturaVideoPlayerPriv::GetAudioTracks_() const
{
    std::vector<CYIAbstractVideoPlayer::AudioTrackInfo> result(m_audioTracks.begin(), m_audioTracks.end());
    return result;
}

CYIAbstractVideoPlayer::AudioTrackInfo KalturaVideoPlayerPriv::GetActiveAudioTrack_() const
{
    if (m_selectedAudioTrack >= 0)
    {
        return m_audioTracks[m_selectedAudioTrack];
    }

    return CYIAbstractVideoPlayer::AudioTrackInfo();
}

std::vector<CYIAbstractVideoPlayer::SeekableRange> KalturaVideoPlayerPriv::GetLiveSeekableRanges_() const
{
    std::vector<CYIAbstractVideoPlayer::SeekableRange> ranges;
    return ranges;
}

bool KalturaVideoPlayerPriv::SelectClosedCaptionsTrack_(uint32_t uID)
{
    if (!m_player)
    {
        return false;
    }

    const auto &track = m_audioTracks[uID];
    [m_player selectTrackWithTrackId:track.uniqueId.ToNSString()];
    return true;
}

std::vector<CYIAbstractVideoPlayer::ClosedCaptionsTrackInfo> KalturaVideoPlayerPriv::GetClosedCaptionsTracks_() const
{
    std::vector<CYIAbstractVideoPlayer::ClosedCaptionsTrackInfo> result(m_closedCaptionsTracks.begin(), m_closedCaptionsTracks.end());
    return result;
}

CYIAbstractVideoPlayer::ClosedCaptionsTrackInfo KalturaVideoPlayerPriv::GetActiveClosedCaptionsTrack_() const
{
    if (m_selectedClosedCaptionTrack >= 0)
    {
        return m_closedCaptionsTracks[m_selectedClosedCaptionTrack];
    }

    return CYIAbstractVideoPlayer::ClosedCaptionsTrackInfo();
}

bool KalturaVideoPlayerPriv::IsMuted_() const
{
    return false;
}

void KalturaVideoPlayerPriv::Mute_(bool bMute)
{
}

void KalturaVideoPlayerPriv::DisableClosedCaptions_()
{
}
