#include "../PKPlayer.h"

#import <Foundation/Foundation.h>
#import <apple/YiRootViewController.h>
#import <objc/runtime.h>

YI_RN_INSTANTIATE_MODULE(PKPlayerWrapper, yi::react::EventEmitterModule);
YI_RN_REGISTER_MODULE(PKPlayerWrapper);

#import "KalturaPlayerYI.h"

PKPlayerWrapper::PKPlayerWrapper()
{
    SetSupportedEvents(PKPLAYER_SUPPORTED_EVENTS);
}

PKPlayerWrapper::~PKPlayerWrapper() {
    [m_player destroy];
    m_player = NULL;
}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, setup)(int pid, dynamic options)
{
    NSLog(@"*** setup(%d, %s)", pid, folly::toJson(options).c_str());
    
    if (m_player) {
        return;
    }
    
    UIView* parentView = YiRootViewController.sharedInstance.view;
    parentView.backgroundColor = UIColor.clearColor;
    
//    EventSender* sender = [EventSender newWithWrapper:this];
//    m_player = [[KalturaPlayerYI alloc] initWithPartnerId:pid options:convertFollyDynamicToId(options)
//                                               parentView:parentView eventSender:sender];
}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, load)(std::string id, dynamic options)
{
    NSLog(@"*** load(%s, %s)", id.c_str(), folly::toJson(options).c_str());
        
    //[m_player loadAssetId:nsstring(id) options:convertFollyDynamicToId(options)];
}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, prepare)()
{
    [m_player prepare];
}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, play)()
{
    NSLog(@"*** play()");

    if (m_player) {
        [m_player play];
    }
}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, pause)()
{
    NSLog(@"*** pause()");
    
//    EmitEvent(MY_NAMED_EVENT, folly::dynamic::object("param", "pause"));

    if (m_player) {
        [m_player pause];
    }
}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, replay)()
{
    NSLog(@"*** replay()");

    if (m_player) {
        [m_player replay];
    }
}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, seekTo)(float position)
{
    NSLog(@"*** seekTo(%.3f)", position);

    if (m_player) {
        [m_player seekTo:position];
    }
}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, changeTrack)(std::string uniqueId)
{
    NSLog(@"*** changeTrack(%s)", uniqueId.c_str());

    if (m_player) {
        [m_player selectTrackWithTrackId:[NSString stringWithUTF8String:uniqueId.c_str()]];
    }
}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, changePlaybackRate)(float playbackRate)
{
    NSLog(@"*** changePlaybackRate(%.3f)", playbackRate);

    if (m_player) {
        [m_player changePlaybackRate:playbackRate];
    }
}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, stop)()
{
    NSLog(@"*** stop()");

    if (m_player) {
        [m_player stop];
    }
}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, destroy)()
{
    NSLog(@"*** destroy()");

    if (m_player) {
        [m_player destroy];
    }
}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, setAutoplay)(bool autoplay)
{
    NSLog(@"*** setAutoplay(%d)", autoplay);

    if (m_player) {
        [m_player setAutoplay:autoplay];
    }
}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, setKS)(std::string ks)
{
    NSLog(@"*** setKS(%s)", ks.c_str());

    if (m_player) {
        //[m_player setKS:nsstring(ks)];
    }
}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, setZIndex)(float index)
{
    [m_player setZIndex:index];
}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, setFrame)(int playerViewWidth, int playerViewHeight, int playerViewPosX, int playerViewPosY)
{
    CGRect frame = YiRootViewController.sharedInstance.view.bounds;
    if (playerViewWidth >= 0) {
        frame.size.width = playerViewWidth;
    }
    if (playerViewHeight >= 0) {
        frame.size.height = playerViewHeight;
    }
    if (playerViewPosX >= 0) {
        frame.origin.x = playerViewPosX;
    }
    if (playerViewPosY >= 0) {
        frame.origin.y = playerViewPosY;
    }
    
    [m_player setFrame:frame];
}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, setVolume)(float volume)
{
    //[m_player setVolume:volume];
}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, setLogLevel)(std::string logLevel)
{
    NSLog(@"*** setLogLevel(%s)", logLevel.c_str());
    
    //[KalturaPlayerYI setLogLevel:nsstring(logLevel)];
}
