#include "player/KalturaVideoPlayerPriv.h"

#include "player/KalturaVideoSurface.h"
#import "player/ios/KalturaPlayerYI.h"

#import <apple/YiRootViewController.h>
#include <player/YiVideoPlayerStateManager.h>

#import <Foundation/Foundation.h>
#import <objc/runtime.h>
#import <UIKit/UIKit.h>

static const CYIString TAG("KalturaVideoPlayerPriv");

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

#pragma mark -

@implementation EventSender

+ (instancetype)newWithWrapper:(KalturaVideoPlayerPriv *)wrapper {
    EventSender *sender = [EventSender new];
    sender->wrapper = wrapper;
    return sender;
}

- (void)sendEvent:(NSString *)name payload:(id _Nullable)payload {
    folly::dynamic dynPayload = convertIdToFollyDynamic(payload);
    reinterpret_cast<KalturaVideoPlayerPriv*>(wrapper)->Emit_(name.UTF8String, dynPayload);
}

@end

#pragma mark - KalturaVideoPlayerPriv

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

    UIView *parentView = YiRootViewController.sharedInstance.view;
    parentView.backgroundColor = UIColor.clearColor;

    EventSender *sender = [EventSender newWithWrapper:this];
    m_player = [[KalturaPlayerYI alloc] initWithPartnerId:partnerId
                                                  options:convertFollyDynamicToId(options)
                                               parentView:parentView
                                              eventSender:sender];
}

void KalturaVideoPlayerPriv::LoadMedia_(const CYIString &assetId, folly::dynamic options)
{
    NSLog(@"*** LoadMedia_(%s, %s)", assetId.GetData(), JSONFromDynamic(options).c_str());

    if (m_pPub->GetPlayerState().mediaState == CYIAbstractVideoPlayer::MediaState::Ready)
    {
        m_pPub->m_pStateManager->TransitionToMediaUnloaded();
    }

    m_pPub->m_pStateManager->TransitionToMediaPreparing();
    [m_player loadMedia:assetId.ToNSString() options:convertFollyDynamicToId(options)];
}

void KalturaVideoPlayerPriv::SetMedia_(const CYIUrl &videoURI)
{
    if (m_pPub->GetPlayerState().mediaState == CYIAbstractVideoPlayer::MediaState::Ready)
    {
        m_pPub->m_pStateManager->TransitionToMediaUnloaded();
    }
    m_pPub->m_pStateManager->TransitionToMediaPreparing();
    NSURL *url = [NSURL URLWithString:videoURI.ToString().ToNSString()];
    NSLog(@"*** SetMedia_(%s)", videoURI.ToString().ToStdString().c_str());
    [m_player setMedia:url];
}

void KalturaVideoPlayerPriv::Emit_(const std::string &name, const folly::dynamic &content)
{
    m_pPub->HandleEvent(name, content);
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

void KalturaVideoPlayerPriv::Replay_()
{
    if (m_player)
    {
        [m_player replay];
    }
}

void KalturaVideoPlayerPriv::ChangePlaybackRate_(float playbackRate)
{
    if (m_player)
    {
        [m_player changePlaybackRate:playbackRate];
    }
}

uint64_t KalturaVideoPlayerPriv::GetDurationMs_() const
{
    return m_pPub->m_durationMs;
}

uint64_t KalturaVideoPlayerPriv::GetCurrentTimeMs_() const
{
    return m_pPub->m_currentTimeMs;
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

    const auto &track = m_pPub->m_audioTracks[uID];
    [m_player selectTrackWithTrackId:track.uniqueId.ToNSString()];
    return true;
}

std::vector<CYIAbstractVideoPlayer::AudioTrackInfo> KalturaVideoPlayerPriv::GetAudioTracks_() const
{
    std::vector<CYIAbstractVideoPlayer::AudioTrackInfo> result(m_pPub->m_audioTracks.begin(), m_pPub->m_audioTracks.end());
    return result;
}

CYIAbstractVideoPlayer::AudioTrackInfo KalturaVideoPlayerPriv::GetActiveAudioTrack_() const
{
    if (m_pPub->m_selectedAudioTrack >= 0)
    {
        return m_pPub->m_audioTracks[m_pPub->m_selectedAudioTrack];
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

    const auto &track = m_pPub->m_audioTracks[uID];
    [m_player selectTrackWithTrackId:track.uniqueId.ToNSString()];
    return true;
}

std::vector<CYIAbstractVideoPlayer::ClosedCaptionsTrackInfo> KalturaVideoPlayerPriv::GetClosedCaptionsTracks_() const
{
    std::vector<CYIAbstractVideoPlayer::ClosedCaptionsTrackInfo> result(m_pPub->m_closedCaptionsTracks.begin(), m_pPub->m_closedCaptionsTracks.end());
    return result;
}

CYIAbstractVideoPlayer::ClosedCaptionsTrackInfo KalturaVideoPlayerPriv::GetActiveClosedCaptionsTrack_() const
{
    if (m_pPub->m_selectedClosedCaptionTrack >= 0)
    {
        return m_pPub->m_closedCaptionsTracks[m_pPub->m_selectedClosedCaptionTrack];
    }

    return CYIAbstractVideoPlayer::ClosedCaptionsTrackInfo();
}

bool KalturaVideoPlayerPriv::IsMuted_() const
{
    return false;
}

void KalturaVideoPlayerPriv::Mute_(bool bMute)
{
    if (bMute) {
        [m_player setVolume:0];
    } else {
        [m_player setVolume:1];
    }
}

void KalturaVideoPlayerPriv::DisableClosedCaptions_()
{
}
