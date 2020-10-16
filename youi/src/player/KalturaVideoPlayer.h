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
    KalturaVideoPlayer();
    virtual ~KalturaVideoPlayer();

    void Setup(int32_t partnerId, folly::dynamic options);
    void Load(std::string assetId, folly::dynamic options);

    CYISignal<bool> CustomSignal;
    
    virtual void SetVideoRectangle(const YI_RECT_REL &rVideoRectangle) override;

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
    
    std::unique_ptr<KalturaVideoPlayerPriv> m_pPriv;

    YI_TYPE_BASES(KalturaVideoPlayer, CYIAbstractVideoPlayer)
};

#endif //ifndef _KALTURA_VIDEO_PLAYER_H_
