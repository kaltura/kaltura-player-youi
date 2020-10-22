//
//  KalturaPlayerYI.m
//  KalturaPlayerTest
//
//  Created by Noam Tamim on 08/07/2020.
//

#import "KalturaPlayerYI.h"

#import <AVFoundation/AVFoundation.h>
#import <PlayKit/PlayKit-Swift.h>

#import <KalturaNetKit/KalturaNetKit-Swift.h>
#import <PlayKitProviders/PlayKitProviders-Swift.h>

#import <GoogleInteractiveMediaAds/GoogleInteractiveMediaAds.h>
#import <PlayKit_IMA/PlayKit_IMA-Swift.h>

#import <CoreData/CoreData.h>
#import <KalturaPlayer/KalturaPlayer-Swift.h>

#import <PlayKitYoubora/PlayKitYoubora-Swift.h>

static PlaybackContextType getPlaybackContextType(NSString *str) {
    if ([str caseInsensitiveCompare:@"playback"] == NSOrderedSame) return PlaybackContextTypePlayback;
    if ([str caseInsensitiveCompare:@"catchup"] == NSOrderedSame) return PlaybackContextTypeCatchup;
    if ([str caseInsensitiveCompare:@"trailer"] == NSOrderedSame) return PlaybackContextTypeTrailer;
    if ([str caseInsensitiveCompare:@"startOver"] == NSOrderedSame) return PlaybackContextTypeStartOver;
    
    return PlaybackContextTypeUnset;
}

static PlaybackContextType getAssetType(NSString *str) {
    if ([str caseInsensitiveCompare:@"media"] == NSOrderedSame) return AssetTypeMedia;
    if ([str caseInsensitiveCompare:@"recording"] == NSOrderedSame) return AssetTypeEpg;
    if ([str caseInsensitiveCompare:@"epg"] == NSOrderedSame) return AssetTypeEpg;
    
    return AssetTypeUnset;
}

static IMAConfig* getImaConfig(NSDictionary *dyn_config) {
    IMAConfig *config = [IMAConfig new];
    config.adTagUrl = dyn_config[@"adTagUrl"];
    config.alwaysStartWithPreroll = [dyn_config[@"alwaysStartWithPreroll"] boolValue];
    config.enableDebugMode = [dyn_config[@"enableDebugMode"] boolValue];
    
    return config;
}

static PluginConfig* createPluginConfig(NSDictionary *options) {
    NSMutableDictionary *pluginConfigDict = [NSMutableDictionary new];
    
    NSDictionary *dyn_imaConfig = options[@"ima"];
    NSDictionary *dyn_youboraConfig = options[@"youbora"];
    
    IMAConfig *imaConfig = getImaConfig(dyn_imaConfig);
    
    pluginConfigDict[IMAPlugin.pluginName] = imaConfig;
    pluginConfigDict[YouboraPlugin.pluginName] = dyn_youboraConfig;
    
    return [[PluginConfig alloc] initWithConfig:pluginConfigDict];
}

static void warmupUrl(NSString *str) {
    if (!str) return;
    
    NSURL *url = [NSURL URLWithString:str];
    if (!url) return;
    
    [[NSURLSession.sharedSession dataTaskWithURL:url completionHandler:^(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error) {
        // Ignore the response
    }] resume];
}

static void warmupUrls(NSString *backendUrl, NSArray<NSString*> *urls) {
    warmupUrl(backendUrl);
    
    for (NSString *str in urls) {
        warmupUrl(str);
    }
}

static id safeJsonValue(id _Nullable value) {
    return value == nil ? NSNull.null : value;
}

static NSDictionary* trackToDict(Track *track, NSString *selectedId) {
    return @{
        @"id": safeJsonValue(track.id),
        @"label": safeJsonValue(track.title),
        @"language": safeJsonValue(track.language),
        @"isAdaptive": @NO, // Audio and text tracks are never adaptive. Video tracks don't exist in HLS/AVPlayer.
        @"isSelected": @([selectedId isEqualToString:track.id])
    };
}

static NSDictionary* entryToDict(PKMediaEntry *entry) {
    NSMutableDictionary *dict = [NSMutableDictionary new];
    
    return dict;
}

#pragma mark - KalturaPlayerYI

@interface KalturaPlayerYI ()

@property EventSender *eventSender;
@property UInt32 partnerId;
@property KalturaOTTPlayer *kalturaPlayer;
@property PlayerOptions *playerOptions;

@end

@implementation KalturaPlayerYI

-(instancetype)initWithPartnerId:(UInt32)pid
                         options:(NSDictionary *)dyn_options
                      parentView:(UIView *)parentView
                     eventSender:(EventSender *)sender {
    
    self = [super init];
    if (!self) return nil;
    
    self.partnerId = pid;
    self.eventSender = sender;
    
    NSString *serverUrl = dyn_options[@"serverUrl"];
    NSLog(@"serverUrl: %@", serverUrl);
    
    PluginConfig *pc = createPluginConfig(dyn_options[@"plugins"]);
    
    [KalturaOTTPlayer setupWithPartnerId:pid serverURL:serverUrl referrer: dyn_options[@"referrer"]];
    
    PlayerOptions *options = [PlayerOptions new];
    options.pluginConfig = pc;
    options.preload = [dyn_options[@"preload"] boolValue];
    options.autoPlay = [dyn_options[@"autoplay"] boolValue];
    options.ks = dyn_options[@"ks"];
    
    warmupUrls(serverUrl, dyn_options[@"warmupUrls"]);
    
    self.playerOptions = options;
    
    KalturaOTTPlayer *player = [[KalturaOTTPlayer alloc] initWithOptions:options];
    
    if (dyn_options[@"allowFairPlayOnExternalScreens"] != nil) {
        player.settings.allowFairPlayOnExternalScreens = [dyn_options[@"allowFairPlayOnExternalScreens"] boolValue];
    }
    
    if (dyn_options[@"shouldPlayImmediately"] != nil) {
        player.settings.shouldPlayImmediately = [dyn_options[@"shouldPlayImmediately"] boolValue];
    }
    
    NSDictionary *trackSelectionDictionary = dyn_options[@"trackSelection"];
    if (trackSelectionDictionary != nil) {
        if (trackSelectionDictionary[@"textLanguage"] != nil && trackSelectionDictionary[@"textMode"] != nil) {
            NSString *textMode = trackSelectionDictionary[@"textMode"];
            NSInteger textModeType = TrackSelectionModeOff;
            
            if ([textMode isEqualToString:@"OFF"]) {
                textModeType = TrackSelectionModeOff;
            } else if ([textMode isEqualToString:@"AUTO"]) {
                textModeType = TrackSelectionModeAuto;
            } else if ([textMode isEqualToString:@"SELECTION"]) {
                textModeType = TrackSelectionModeSelection;
            }
            
            [player.settings.trackSelection setTextSelectionMode: textModeType];
            [player.settings.trackSelection setTextSelectionLanguage: trackSelectionDictionary[@"textLanguage"]];
        }
        
        if (trackSelectionDictionary[@"audioMode"] != nil && trackSelectionDictionary[@"audioLanguage"] != nil) {
            NSString *audioMode = trackSelectionDictionary[@"audioMode"];
            NSInteger audioModeType = TrackSelectionModeOff;
            
            if ([audioMode isEqualToString:@"OFF"]) {
                audioModeType = TrackSelectionModeOff;
            } else if ([audioMode isEqualToString:@"AUTO"]) {
                audioModeType = TrackSelectionModeAuto;
            } else if ([audioMode isEqualToString:@"SELECTION"]) {
                audioModeType = TrackSelectionModeSelection;
            }
            
            [player.settings.trackSelection setAudioSelectionMode: audioModeType];
            [player.settings.trackSelection setAudioSelectionLanguage: trackSelectionDictionary[@"audioLanguage"]];
        }
    }
    
    NSDictionary *abrSettingsDictionary = dyn_options[@"abrSettings"];
    if (abrSettingsDictionary != nil) {
        if (abrSettingsDictionary[@"maxVideoBitrate"] != nil) {
            [player.settings.network setPreferredPeakBitRate: [abrSettingsDictionary[@"maxVideoBitrate"] doubleValue]];
        }
    }
    
    NSDictionary *networkDictionary = dyn_options[@"networkSettings"];
    if (networkDictionary != nil) {
        if (networkDictionary[@"autoBuffer"] != nil) {
            [player.settings.network setAutoBuffer: [networkDictionary[@"autoBuffer"] boolValue]];
        }
        
        if (networkDictionary[@"preferredForwardBufferDuration"] != nil) {
            [player.settings.network setPreferredForwardBufferDuration: [networkDictionary[@"preferredForwardBufferDuration"] doubleValue]];
        }
        
        if (networkDictionary[@"automaticallyWaitsToMinimizeStalling"] != nil) {
            [player.settings.network setAutomaticallyWaitsToMinimizeStalling: [networkDictionary[@"automaticallyWaitsToMinimizeStalling"] boolValue]];
        }
    }
    
    KalturaPlayerView *playerView = [[KalturaPlayerView alloc] initWithFrame:parentView.bounds];
    playerView.contentMode = UIViewContentModeScaleAspectFit;
    playerView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
    [parentView insertSubview:playerView atIndex:0];
    player.view = playerView;
    
    self.kalturaPlayer = player;
    
    [self observeAllEvents];
    
    return self;
}

-(void)observeAllEvents {
    __weak EventSender *weakSender = self.eventSender;
    __weak KalturaPlayer *weakPlayer = self.kalturaPlayer;
    
    [self.kalturaPlayer addObserver:self event:PlayerEvent.canPlay block:^(PKEvent * _Nonnull event) {
        [weakSender sendEvent:@"canPlay" payload:@{}];
    }];
    [self.kalturaPlayer addObserver:self event:PlayerEvent.playing block:^(PKEvent * _Nonnull event) {
        [weakSender sendEvent:@"playing" payload:@{}];
    }];
    [self.kalturaPlayer addObserver:self event:PlayerEvent.play block:^(PKEvent * _Nonnull event) {
        [weakSender sendEvent:@"play" payload:@{}];
    }];
    [self.kalturaPlayer addObserver:self event:PlayerEvent.pause block:^(PKEvent * _Nonnull event) {
        [weakSender sendEvent:@"pause" payload:@{}];
    }];
    [self.kalturaPlayer addObserver:self event:PlayerEvent.stopped block:^(PKEvent * _Nonnull event) {
        [weakSender sendEvent:@"stopped" payload:@{}];
    }];
    
    [self.kalturaPlayer addObserver:self event:PlayerEvent.durationChanged block:^(PKEvent * _Nonnull event) {
        [weakSender sendEvent:@"durationChanged" payload:@{@"duration": event.duration}];
    }];
    [self.kalturaPlayer addObserver:self event:PlayerEvent.playheadUpdate block:^(PKEvent * _Nonnull event) {
        [weakSender sendEvent:@"timeUpdate" payload:@{@"position": event.currentTime}];
    }];
    [self.kalturaPlayer addObserver:self event:PlayerEvent.tracksAvailable block:^(PKEvent * _Nonnull event) {
        
        NSMutableArray *audioTracks = [NSMutableArray array];
        NSString *selectedAudioTrackId = [weakPlayer currentAudioTrack];
        for (Track *track in event.tracks.audioTracks) {
            [audioTracks addObject:trackToDict(track, selectedAudioTrackId)];
        }
        NSMutableArray *textTracks = [NSMutableArray array];
        NSString *selectedTextTrackId = [weakPlayer currentTextTrack];
        for (Track *track in event.tracks.textTracks) {
            [textTracks addObject:trackToDict(track, selectedTextTrackId)];
        }
        
        [weakSender sendEvent:@"tracksAvailable" payload:@{@"audio": audioTracks.copy, @"text": textTracks.copy}];
    }];
    [self.kalturaPlayer addObserver:self event:PlayerEvent.videoTrackChanged block:^(PKEvent * _Nonnull event) {
        [weakSender sendEvent:@"videoTrackChanged" payload:@{@"bitrate": event.bitrate}];
    }];
    [self.kalturaPlayer addObserver:self event:PlayerEvent.audioTrackChanged block:^(PKEvent * _Nonnull event) {
        [weakSender sendEvent:@"audioTrackChanged"
                      payload:trackToDict(event.selectedTrack, event.selectedTrack.id)];
    }];
    [self.kalturaPlayer addObserver:self event:PlayerEvent.textTrackChanged block:^(PKEvent * _Nonnull event) {
        [weakSender sendEvent:@"textTrackChanged"
                      payload:trackToDict(event.selectedTrack, event.selectedTrack.id)];
    }];
    [self.kalturaPlayer addObserver:self event:PlayerEvent.error block:^(PKEvent * _Nonnull event) {
        [weakSender sendEvent:@"error" payload:@{@"errorType": @(event.error.code)}];   // TODO more details
    }];
    [self.kalturaPlayer addObserver:self event:PlayerEvent.stateChanged block:^(PKEvent * _Nonnull event) {
        PlayerState state = event.newState;
        NSString *stateName = nil;
        switch (state) {
            case PlayerStateBuffering:
                stateName = @"BUFFERING";
                break;
            case PlayerStateIdle:
                stateName = @"IDLE";
                break;
            case PlayerStateReady:
                stateName = @"READY";
                break;
            default:
                return; // discard event
        }
        [weakSender sendEvent:@"stateChanged" payload:@{@"newState": stateName}];
    }];
    [self.kalturaPlayer addObserver:self event:PlayerEvent.seeking block:^(PKEvent * _Nonnull event) {
        [weakSender sendEvent:@"seeking" payload:@{@"targetPosition": event.targetSeekPosition}];
    }];
    [self.kalturaPlayer addObserver:self event:PlayerEvent.seeked block:^(PKEvent * _Nonnull event) {
        [weakSender sendEvent:@"seeked" payload:@{}];
    }];
    
    [self.kalturaPlayer addObserver:self event:AdEvent.adDidProgressToTime block:^(PKEvent * _Nonnull event) {
        [weakSender sendEvent:@"adProgress" payload:@{@"currentAdPosition": event.adMediaTime}];
    }];
    [self.kalturaPlayer addObserver:self event:AdEvent.adStarted block:^(PKEvent * _Nonnull event) {
        [weakSender sendEvent:@"adStarted" payload:@{}];
    }];
    [self.kalturaPlayer addObserver:self event:AdEvent.adComplete block:^(PKEvent * _Nonnull event) {
        [weakSender sendEvent:@"adCompleted" payload:@{}];
    }];
    [self.kalturaPlayer addObserver:self event:AdEvent.adPaused block:^(PKEvent * _Nonnull event) {
        [weakSender sendEvent:@"adPaused" payload:@{}];
    }];
    [self.kalturaPlayer addObserver:self event:AdEvent.adResumed block:^(PKEvent * _Nonnull event) {
        [weakSender sendEvent:@"adResumed" payload:@{}];
    }];
    [self.kalturaPlayer addObserver:self event:AdEvent.adCuePointsUpdate block:^(PKEvent * _Nonnull event) {
        PKAdCuePoints *cuePoints = event.adCuePoints;
        [weakSender sendEvent:@"adCuepointsChanged" payload:@{
            @"adPluginName": @"ima",
            @"cuePoints": safeJsonValue(cuePoints.cuePoints),
            @"hasPreRoll": @(cuePoints.hasPreRoll),
            @"hasMidRoll": @(cuePoints.hasMidRoll),
            @"hasPostRoll": @(cuePoints.hasPostRoll)
        }];
    }];
    [self.kalturaPlayer addObserver:self event:AdEvent.adStartedBuffering block:^(PKEvent * _Nonnull event) {
        [weakSender sendEvent:@"adBufferStart" payload:@{}];
    }];
    [self.kalturaPlayer addObserver:self event:AdEvent.adClicked block:^(PKEvent * _Nonnull event) {
        [weakSender sendEvent:@"adClicked" payload:@{@"clickThruUrl": event.data[@"clickThroughUrl"]}];
    }];
    [self.kalturaPlayer addObserver:self event:AdEvent.adSkipped block:^(PKEvent * _Nonnull event) {
        [weakSender sendEvent:@"adSkipped" payload:@{}];
    }];
    [self.kalturaPlayer addObserver:self event:AdEvent.adsRequested block:^(PKEvent * _Nonnull event) {
        [weakSender sendEvent:@"adRequested" payload:@{@"adTagUrl": event.adTagUrl}];
    }];
    [self.kalturaPlayer addObserver:self event:AdEvent.adDidRequestContentPause block:^(PKEvent * _Nonnull event) {
        [weakSender sendEvent:@"adContentPauseRequested" payload:@{}];
    }];
    [self.kalturaPlayer addObserver:self event:AdEvent.adDidRequestContentResume block:^(PKEvent * _Nonnull event) {
        [weakSender sendEvent:@"adContentResumeRequested" payload:@{}];
    }];
    [self.kalturaPlayer addObserver:self event:AdEvent.allAdsCompleted block:^(PKEvent * _Nonnull event) {
        [weakSender sendEvent:@"allAdsCompleted" payload:@{}];
    }];
    [self.kalturaPlayer addObserver:self event:AdEvent.error block:^(PKEvent * _Nonnull event) {
        [weakSender sendEvent:@"adError" payload:@{@"errorType": @(event.error.code)}];
    }];
}

-(void)loadAssetId:(NSString *)assetId options:(NSDictionary *)dyn_options {
    OTTMediaOptions *options = [OTTMediaOptions new];
    options.assetId = assetId;
    options.ks = dyn_options[@"ks"];
    options.formats = @[dyn_options[@"format"]];
    options.assetType = getAssetType(dyn_options[@"assetType"]);
    options.networkProtocol = dyn_options[@"protocol"];
    options.playbackContextType = getPlaybackContextType(dyn_options[@"playbackContextType"]);
    options.startTime = [dyn_options[@"startPosition"] doubleValue];
    
    
    id youboraConfig = dyn_options[@"plugins"][@"youbora"];
    if (youboraConfig) {
        [self.kalturaPlayer updatePluginConfigWithPluginName:YouboraPlugin.pluginName config:youboraConfig];
    }
    
    id imaConfig = dyn_options[@"plugins"][@"ima"];
    if (imaConfig) {
        [self.kalturaPlayer updatePluginConfigWithPluginName:IMAPlugin.pluginName config:getImaConfig(imaConfig)];
    }
    
    __weak EventSender *weakSender = self.eventSender;
    [self.kalturaPlayer loadMediaWithOptions:options callback:^(NSError * _Nullable error) {
        if (error) {
            NSLog(@"Error in loadMedia: %@", error);
            [weakSender sendEvent:@"loadMediaFailed" payload:nil];
        } else {
            [weakSender sendEvent:@"loadMediaSuccess" payload:nil];
        }
    }];
    
    // Player will automatically prepare and play on success
}

-(void)prepare {
    [self.kalturaPlayer prepare];
}

-(void)play {
    [self.kalturaPlayer play];
}

-(void)pause {
    [self.kalturaPlayer pause];
}

-(void)replay {
    [self.kalturaPlayer replay];
}

-(void)destroy {
    [self.kalturaPlayer removeObserver:self events:PlayerEvent.allEventTypes];
    [self.kalturaPlayer removeObserver:self events:AdEvent.allEventTypes];
    [self.kalturaPlayer destroy];
}

-(void)stop {
    [self.kalturaPlayer stop];
}

-(void)seekTo:(NSTimeInterval)position {
    [self.kalturaPlayer seekTo:position];
}

-(void)selectTrackWithTrackId:(NSString *)uniqueId {
    [self.kalturaPlayer selectTrackWithTrackId:uniqueId];
}

-(void)changePlaybackRate:(float)playbackRate {
    self.kalturaPlayer.rate = playbackRate;
}

-(void)setAutoplay:(bool)autoplay {
    self.playerOptions.autoPlay = autoplay;
    [self.kalturaPlayer updatePlayerOptions:self.playerOptions];
}

-(void)setKS:(NSString *)ks {
    self.playerOptions.ks = ks;
    [self.kalturaPlayer updatePlayerOptions:self.playerOptions];
}

-(void)setZIndex:(float)index {
    self.kalturaPlayer.view.layer.zPosition = index;
}

-(void)setFrame:(CGRect)frame {
    self.kalturaPlayer.view.frame = frame;
}

-(void)setVolume:(float)volume {
    //[self.kalturaPlayer setVolume:volume];
}

+(void)setLogLevel:(NSString *)logLevel {
    PKLogLevel level = PKLogLevelError;
    
    if ([logLevel  isEqual: @"verbose"]) {
        level = PKLogLevelVerbose;
    } else if ([logLevel  isEqual: @"debug"]) {
        level = PKLogLevelDebug;
    } else if ([logLevel  isEqual: @"info"]) {
        level = PKLogLevelInfo;
    } else if ([logLevel  isEqual: @"warning"]) {
        level = PKLogLevelWarning;
    } else if ([logLevel  isEqual: @"error"]) {
        level = PKLogLevelError;
    } else {
        NSLog(@"*** Unknown log level: %@", logLevel);
    }
    
    PlayKitManager.logLevel = level;
}

@end
