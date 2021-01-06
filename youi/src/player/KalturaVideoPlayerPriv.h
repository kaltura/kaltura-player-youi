#ifndef _KALTURA_VIDEO_PLAYER_PRIV_H_
#define _KALTURA_VIDEO_PLAYER_PRIV_H_

#include <folly/dynamic.h>

#include "player/KalturaVideoPlayer.h"

#if __OBJC__
@class KalturaPlayerYI;
#else
typedef void KalturaPlayerYI;
#endif

#if YI_ANDROID
// Exposes the JNI environment.
#include <jni.h>
#include <folly/json.h>
#endif

class KalturaVideoPlayerPriv
{
public:
    KalturaVideoPlayerPriv(KalturaVideoPlayer *pPub);
    virtual ~KalturaVideoPlayerPriv();

    void Setup_(int32_t partnerId, folly::dynamic options);
    void LoadMedia_(const CYIString &assetId, folly::dynamic options);
    void SetMedia_(const CYIUrl &videoURI);
    void Replay_();
    void ChangePlaybackRate_(float playbackRate);
    void SetLogLevel_(const CYIString &logLevel);

    bool SelectVideoTrack_(uint32_t uID);
    std::vector<KalturaVideoPlayer::VideoTrackInfo> GetVideoTracks_() const;
    KalturaVideoPlayer::VideoTrackInfo GetActiveVideoTrack_() const;
    
    void Emit_(const std::string &event, const folly::dynamic &content);

    void SetVideoRectangle(const YI_RECT_REL &rVideoRectangle);

    CYIString GetName_() const;
    CYIString GetVersion_() const;
    CYIAbstractVideoPlayer::Statistics GetStatistics_() const;
    std::unique_ptr<CYIVideoSurface> CreateSurface_();
    bool SupportsFormat_(CYIAbstractVideoPlayer::StreamingFormat eFormat, CYIAbstractVideoPlayer::DRMScheme eDRMScheme) const;
    void Prepare_(const CYIUrl &videoURI, CYIAbstractVideoPlayer::StreamingFormat eFormatHint);
    void Play_();
    void Pause_();
    void Stop_();
    uint64_t GetDurationMs_() const;
    uint64_t GetCurrentTimeMs_() const;
    void Seek_(uint64_t uSeekPositionMs);

    bool SelectAudioTrack_(uint32_t uID);
    std::vector<CYIAbstractVideoPlayer::AudioTrackInfo> GetAudioTracks_() const;
    CYIAbstractVideoPlayer::AudioTrackInfo GetActiveAudioTrack_() const;

    std::vector<CYIAbstractVideoPlayer::SeekableRange> GetLiveSeekableRanges_() const;

    bool SelectClosedCaptionsTrack_(uint32_t uID);
    std::vector<CYIAbstractVideoPlayer::ClosedCaptionsTrackInfo> GetClosedCaptionsTracks_() const;
    CYIAbstractVideoPlayer::ClosedCaptionsTrackInfo GetActiveClosedCaptionsTrack_() const;
    void Mute_(bool bMute);
    void BringToFront_();
    void DisableClosedCaptions_();

    void SetMaxBitrate_(uint64_t uMaxBitrate);

    void HandleEvent(const CYIString& name, folly::dynamic content);

    KalturaVideoPlayer *m_pPub;
    bool m_bringToFront;

private:
#ifdef YI_ANDROID
    void LoadIDs();
#endif

#ifdef YI_IOS
    KalturaPlayerYI *m_player;
#endif

#ifdef YI_ANDROID
    jclass playerWrapperBridgeClass = NULL;
    
    jmethodID setupMethodID = 0;
    jmethodID loadMediaMethodID = 0;
    jmethodID setMediaMethodID = 0;
    jmethodID prepareMethodID = 0;
    jmethodID playMethodID = 0;
    jmethodID pauseMethodID = 0;
    jmethodID replayMethodID = 0;
    jmethodID destroyMethodID = 0;
    jmethodID stopMethodID = 0;
    jmethodID seekToMethodID = 0;
    jmethodID changeTrackMethodID = 0;
    jmethodID changePlaybackRateMethodID = 0;
    jmethodID setAutoplayMethodID = 0;
    jmethodID setKSMethodID = 0;
    jmethodID setZIndexMethodID = 0;
    jmethodID setFrameMethodID = 0;
    jmethodID setVolumeMethodID = 0;
    jmethodID setLogLevelMethodID = 0;
#endif
};

#endif //ifndef _KALTURA_VIDEO_PLAYER_PRIV_H_
