#ifndef _KALTURA_VIDEO_PLAYER_PRIV_H_
#define _KALTURA_VIDEO_PLAYER_PRIV_H_

#include <player/YiAbstractVideoPlayer.h>
#include <utility/FollyDynamicUtilities.h>

#if defined(YI_IOS)
    #include "../pk/ios/KalturaPlayerYI.h"
#endif

#if defined(ANDROID)
    #include "AndroidCommon.h"
#endif

class KalturaVideoPlayer;

class KalturaVideoPlayerPriv
{
public:
    KalturaVideoPlayerPriv(KalturaVideoPlayer *pPub);
    virtual ~KalturaVideoPlayerPriv();
    
    void Setup_(int32_t partnerId, folly::dynamic options);
    void Load_(std::string assetId, folly::dynamic options);
    void Emit_(const std::string &event, const folly::dynamic &content); //TODO Handle events
    
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
    std::vector<CYIAbstractVideoPlayer::ClosedCaptionsTrackInfo> GetClosedCaptionsTracks_()const;
    CYIAbstractVideoPlayer::ClosedCaptionsTrackInfo GetActiveClosedCaptionsTrack_() const;
    bool IsMuted_() const;
    void Mute_(bool bMute);
    void DisableClosedCaptions_();

    void SetMaxBitrate_(uint64_t uMaxBitrate);
    
    #if defined(YI_IOS)
        KalturaPlayerYI *m_player;
    #endif

    KalturaVideoPlayer *m_pPub;
    
    uint64_t m_durationMs = 0;
    uint64_t m_currentTimeMs = 0;

    #if defined(ANDROID)
        jmethodID _constructor;
        jmethodID _prepareVideo;
        jmethodID _playVideo;
        jmethodID _pauseVideo;
        jmethodID _seek;
        jmethodID _stop;
        jmethodID _setVideoRectangle;

        jclass playerClass;
        jobject playerObject;
    #endif

};

#endif //ifndef _KALTURA_VIDEO_PLAYER_PRIV_H_
