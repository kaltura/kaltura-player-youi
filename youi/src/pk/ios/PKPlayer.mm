#include "../PKPlayer.h"

#import <Foundation/Foundation.h>
#import <apple/YiRootViewController.h>
#import <objc/runtime.h>

YI_RN_INSTANTIATE_MODULE(PKPlayerWrapper, yi::react::EventEmitterModule);
YI_RN_REGISTER_MODULE(PKPlayerWrapper);

#import "KalturaPlayerYI.h"

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


static NSString* nsstring(dynamic str) {
    if (str.isString()) {
        return [NSString stringWithUTF8String:str.c_str()];
    }
    return nil;
}

static NSString* nsstring(std::string str) {
    return [NSString stringWithUTF8String:str.c_str()];
}

@implementation EventSender

+(instancetype)newWithWrapper:(PKPlayerWrapper*)wrapper {
    EventSender* sender = [EventSender new];
    sender->wrapper = wrapper;
    return sender;
}

-(void)sendEvent:(NSString*)name payload:(id _Nullable)payload {
    dynamic dynPayload = convertIdToFollyDynamic(payload);
    reinterpret_cast<PKPlayerWrapper*>(wrapper)->Emit(name.UTF8String, dynPayload);
}

@end




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
    
    EventSender* sender = [EventSender newWithWrapper:this];
    m_player = [[KalturaPlayerYI alloc] initWithPartnerId:pid options:convertFollyDynamicToId(options) 
                                               parentView:parentView eventSender:sender];
}

YI_RN_DEFINE_EXPORT_METHOD(PKPlayerWrapper, load)(std::string id, dynamic options)
{
    NSLog(@"*** load(%s, %s)", id.c_str(), folly::toJson(options).c_str());
        
    [m_player loadAssetId:nsstring(id) options:convertFollyDynamicToId(options)];
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
        [m_player setKS:nsstring(ks)];
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
    
    [KalturaPlayerYI setLogLevel:nsstring(logLevel)];
}
