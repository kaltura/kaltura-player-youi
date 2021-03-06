#ifndef _KALTURA_VIDEO_PLAYER_H_
#define _KALTURA_VIDEO_PLAYER_H_

#include <player/YiAbstractVideoPlayer.h>
#include <utility/FollyDynamicUtilities.h>

#include "IVideoSurfaceListener.h"

class KalturaVideoPlayerPriv;

class KalturaVideoPlayer : public CYIAbstractVideoPlayer, IVideoSurfaceListener
{
    friend class KalturaVideoPlayerPriv;
    
public:
    //{"id":"Video:0,0,0","height":360,"isAdaptive":false,"isSelected":false,"bitrate":508450,"width":640}
    struct VideoTrackInfo
    {
        uint32_t id = 0;
        CYIString uniqueId;
        uint64_t bitrate = 0;
        uint32_t width = 0;
        uint32_t height = 0;
        bool isAdaptive = false;
        bool isSelected = false;

        VideoTrackInfo()
        {
        }

        VideoTrackInfo(uint32_t id, const CYIString &uID, uint64_t bitrate, uint32_t width, uint32_t height, bool isAdaptive, bool isSelected)
            : id(id)
            , uniqueId(uID)
            , bitrate(bitrate)
            , width(width)
            , height(height)
            , isAdaptive(isAdaptive)
            , isSelected(isSelected)
        {
        }
        
        folly::dynamic ToDynamic() const
        {
            auto object = folly::dynamic::object
                ("id", id)
                ("uniqueId", uniqueId.GetData())
                ("bitrate", bitrate)
                ("width", width)
                ("height", height)
                ("isAdaptive", isAdaptive)
                ("isSelected", isSelected);
            return object;
        }
    };    

    struct KalturaAudioTrack : public CYIAbstractVideoPlayer::AudioTrackInfo
    {
        CYIString uniqueId;

        KalturaAudioTrack(uint32_t id, const CYIString &uID, const CYIString &name, const CYIString &language)
                : AudioTrackInfo(id, name, language)
                , uniqueId(uID)
        {
        }
    };

    struct KalturaClosedCaptionTrack : public CYIAbstractVideoPlayer::ClosedCaptionsTrackInfo
    {
        CYIString uniqueId;

        KalturaClosedCaptionTrack(uint32_t id, const CYIString &uID, const CYIString &name, const CYIString &language)
            : ClosedCaptionsTrackInfo(id, name, language)
            , uniqueId(uID)
        {
        }
    };

    KalturaVideoPlayer();
    virtual ~KalturaVideoPlayer();

    void Setup(int32_t partnerId, folly::dynamic options);
    void LoadMedia(const CYIString &assetId, folly::dynamic options);
    void SetMedia(const CYIUrl &videoURI);
    void Replay();
    void ChangePlaybackRate(float playbackRate);
    void SetPlayerZIndex(float zIndex);
    void SetLogLevel(const CYIString &logLevel);
    void KeepDeviceScreenOn(bool keepOn);

    bool SelectVideoTrack(uint32_t uID);
    std::vector<VideoTrackInfo> GetVideoTracks();
    VideoTrackInfo GetActiveVideoTrack();
    bool isValidJsonKey(folly::dynamic content, const CYIString &keyName);

    CYISignal<> PlayerReplayEvent;
    CYISignal<> PlayerStoppedEvent;
    CYISignal<float> PlaybackRateChangedEvent;

    CYISignal<uint64_t> PlayerSeekingEvent;
    CYISignal<> PlayerSeekedEvent;

    CYISignal<std::vector<VideoTrackInfo>> AvailableVideoTracksChanged;
  
    CYISignal<uint64_t>  CurrentProgramTimeUpdated;

    CYISignal<folly::dynamic> LoadMediaSuccess;
    CYISignal<float> VolumeChanged;
    CYISignal<uint64_t> CurrentBufferTimeUpdated;
    CYISignal<bool> KeepDeviceScreenOnUpdated;

    virtual void SetVideoRectangle(const YI_RECT_REL &rVideoRectangle) override;
    virtual void BringToFront() override;

private:
    virtual CYIString GetName_() const override;
    virtual CYIString GetVersion_() const override;
    virtual Statistics GetStatistics_() const override;
    virtual std::unique_ptr<CYIVideoSurface> CreateSurface_() override;
    virtual bool SupportsFormat_(CYIAbstractVideoPlayer::StreamingFormat eFormat, CYIAbstractVideoPlayer::DRMScheme eDRMScheme) const override;
    virtual void Prepare_(const CYIUrl &videoURI, CYIAbstractVideoPlayer::StreamingFormat eFormatHint) override;
    virtual void Play_() override;
    virtual void Pause_() override;
    virtual void Stop_() override;
    virtual uint64_t GetDurationMs_() const override;
    virtual uint64_t GetCurrentTimeMs_() const override;
    virtual void Seek_(uint64_t uSeekPositionMs) override;

    virtual bool SelectAudioTrack_(uint32_t uID) override;
    virtual std::vector<AudioTrackInfo> GetAudioTracks_() const override;
    virtual AudioTrackInfo GetActiveAudioTrack_() const override;

    virtual std::vector<SeekableRange> GetLiveSeekableRanges_() const override;
    
    virtual bool SelectClosedCaptionsTrack_(uint32_t uID) override;
    virtual std::vector<ClosedCaptionsTrackInfo> GetClosedCaptionsTracks_()const override;
    virtual ClosedCaptionsTrackInfo GetActiveClosedCaptionsTrack_() const override;
    virtual bool IsMuted_() const override;
    virtual void Mute_(bool bMute) override;
    virtual void DisableClosedCaptions_() override;

    virtual void SetMaxBitrate_(uint64_t uMaxBitrate) override;

    void HandleEvent(const CYIString& name, folly::dynamic content);

    const CYIString FATAL = "Fatal";
    const CYIString RECOVERABLE = "Recoverable";
    void InternalKeepDeviceScreenOn(bool keepOn);

    std::unique_ptr<KalturaVideoPlayerPriv> m_pPriv;
    
    CYIString m_devicOSName;
    uint64_t m_durationMs = 0;
    uint64_t m_currentTimeMs = 0;
    bool m_isMuted = false;
    bool m_manageKeepScreenOnInternally = true;

    std::vector<KalturaVideoPlayer::VideoTrackInfo> m_videoTracks;
    int32_t m_selectedVideoTrack = -1;

    std::vector<KalturaAudioTrack> m_audioTracks;
    int32_t m_selectedAudioTrack = -1;

    std::vector<KalturaClosedCaptionTrack> m_closedCaptionsTracks;
    int32_t m_selectedClosedCaptionTrack = -1;
    std::vector<CYIAbstractVideoPlayer::SeekableRange> m_liveSeekableRanges;

    YI_TYPE_BASES(KalturaVideoPlayer, CYIAbstractVideoPlayer)
};

#endif //ifndef _KALTURA_VIDEO_PLAYER_H_
