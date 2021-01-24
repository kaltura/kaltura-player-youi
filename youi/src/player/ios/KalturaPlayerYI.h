//
//  KalturaPlayerYI.h
//
//  Created by Noam Tamim on 08/07/2020.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface EventSender: NSObject {
    void *wrapper;
}

- (void)sendEvent:(NSString *)name payload:(id _Nullable)payload;

@end

#pragma mark -

@interface KalturaPlayerYI: NSObject

- (instancetype)initWithPartnerId:(UInt32)pid
                          options:(NSDictionary *)options
                       parentView:(UIView *)parentView
                      eventSender:(EventSender *)sender;

- (void)setMedia:(NSURL *)contentUrl;
- (void)loadMedia:(NSString *)assetId options:(NSDictionary *)options;

- (void)prepare;
- (void)play;
- (void)pause;
- (void)replay;
- (void)destroy;
- (void)stop;
- (void)seekTo:(NSTimeInterval)position;

- (void)selectTrackWithTrackId:(NSString *)uniqueId;
- (void)changePlaybackRate:(float)playbackRate;
- (void)setAutoplay:(bool)autoplay;
- (void)setKS:(NSString *)ks;
- (void)setZIndex:(float)index;
- (void)setFrame:(CGRect)frame;
- (void)setVolume:(float)volume;

+ (void)setLogLevel:(NSString*)level;

@end

NS_ASSUME_NONNULL_END
