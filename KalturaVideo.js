import React from 'react';
import { findNodeHandle, NativeModules, NativeEventEmitter } from 'react-native';
import { Video } from '@youi/react-native-youi';

const PlayerEventEmitter = new NativeEventEmitter(NativeModules.KalturaVideo);

export default class KalturaVideo extends React.Component {

  constructor(props) {
    super(props)
    this.playerEventEmitter = null
    this.videoRef = React.createRef();
    this.sanitizeProps(props)
  }

  sanitizeProps(props) {
    this.childProps = { ...props };
    delete this.childProps.source;
  }

  componentDidMount() {
    // Must be called before any other method on the native module
    NativeModules.KalturaVideo.ConnectToPlayer(findNodeHandle(this.videoRef.current));

    this.eventEmitter = PlayerEventEmitter.addListener('KALTURA_AD_BREAK_STARTED_EVENT', () => {
      if (this.props.onAdBreakStartedEvent) {
        this.props.onAdBreakStartedEvent();
      }
    })

    this.eventEmitter = PlayerEventEmitter.addListener('KALTURA_AD_BREAK_ENDED_EVENT', () => {
      if (this.props.onAdBreakEndedEvent) {
        this.props.onAdBreakEndedEvent();
      }
    })

    this.eventEmitter = PlayerEventEmitter.addListener('KALTURA_AD_STARTED_EVENT', (adInfo) => {
      if (this.props.onAdStartedEvent) {
        this.props.onAdStartedEvent(adInfo);
      }
    })

    this.eventEmitter = PlayerEventEmitter.addListener('KALTURA_AD_COMPLETED_EVENT', () => {
      if (this.props.onAdCompletedEvent) {
        this.props.onAdCompletedEvent();
      }
    })

    this.eventEmitter = PlayerEventEmitter.addListener('KALTURA_AD_PAUSED_EVENT', () => {
      if (this.props.onAdPausedEvent) {
        this.props.onAdPausedEvent();
      }
    })

    this.eventEmitter = PlayerEventEmitter.addListener('KALTURA_AD_RESUMED_EVENT', () => {
      if (this.props.onAdResumedEvent) {
        this.props.onAdAdResumedEvent();
      }
    })

    this.eventEmitter = PlayerEventEmitter.addListener('KALTURA_AD_BUFFER_START_EVENT', () => {
      if (this.props.onAdBufferStartEvent) {
        this.props.onAdBufferStartEvent();
      }
    })

    this.eventEmitter = PlayerEventEmitter.addListener('KALTURA_AD_SKIPPED_EVENT', () => {
      if (this.props.onAdSkippedEvent) {
        this.props.onAdSkippedEvent();
      }
    })

    this.eventEmitter = PlayerEventEmitter.addListener('KALTURA_AD_CONTENT_PAUSE_REQUESTED_EVENT', () => {
      if (this.props.onAdContentPauseRequested) {
        this.props.onAdContentPauseRequested();
      }
    })

    this.eventEmitter = PlayerEventEmitter.addListener('KALTURA_AD_CONTENT_RESUME_REQUESTED_EVENT', () => {
      if (this.props.onAdContentResumeRequestedEvent) {
        this.props.onAdContentResumeRequestedEvent();
      }
    })

    this.eventEmitter = PlayerEventEmitter.addListener('KALTURA_AD_ALL_ADS_COMPLETED_EVENT', () => {
      if (this.props.onAllAdsCompletedEvent) {
        this.props.onAllAdsCompletedEvent();
      }
    })

    this.eventEmitter = PlayerEventEmitter.addListener('KALTURA_AD_PROGRESS_EVENT', (adPosition) => {
      if (this.props.onAdProgressEvent) {
        this.props.onAdProgressEvent(adPosition);
      }
    })

    this.eventEmitter = PlayerEventEmitter.addListener('KALTURA_AD_CUEPOINTS_CHANGED_EVENT', (cuePoints) => {
      if (this.props.onAdCuepointsChangedEvent) {
        this.props.onAdCuepointsChangedEvent(cuePoints);
      }
    })

    this.eventEmitter = PlayerEventEmitter.addListener('KALTURA_AD_CLICKED_EVENT', (clickThruUrl) => {
      if (this.props.onAdClickedEvent) {
        this.props.onAdClickedEvent(clickThruUrl);
      }
    })

    this.eventEmitter = PlayerEventEmitter.addListener('KALTURA_AD_REQUESTED_EVENT', (adTagUrl) => {
      if (this.props.onAdRequestedEvent) {
        this.props.onAdRequestedEvent(adTagUrl);
      }
    })

    this.eventEmitter = PlayerEventEmitter.addListener('KALTURA_PLAYING_EVENT', () => {
      if (this.props.onPlayingEvent) {
        this.props.onPlayingEvent();
      }
    })

    this.eventEmitter = PlayerEventEmitter.addListener('KALTURA_ENDED_EVENT', () => {
      if (this.props.onEndedEvent) {
        this.props.onEndedEvent();
      }
    })

    this.eventEmitter = PlayerEventEmitter.addListener('KALTURA_STOPPED_EVENT', () => {
      if (this.props.onStoppedEvent) {
        this.props.onStoppedEvent();
      }
    })

    this.eventEmitter = PlayerEventEmitter.addListener('KALTURA_REPLAY_EVENT', () => {
      if (this.props.onReplayEvent) {
        this.props.onReplayEvent();
      }
    })

    this.eventEmitter = PlayerEventEmitter.addListener('KALTURA_SEEKED_EVENT', () => {
      if (this.props.onSeekedEvent) {
        this.props.onSeekedEvent();
      }
    })

    this.eventEmitter = PlayerEventEmitter.addListener('KALTURA_SEEKING_EVENT', (targetPosition) => {
      if (this.props.onSeekingEvent) {
        this.props.onSeekingEvent(targetPosition);
      }
    })

    this.eventEmitter = PlayerEventEmitter.addListener('KALTURA_VOLUME_CHANGED', (event) => {
      if (this.props.onVolumeChanged) {
        this.props.onVolumeChanged(event.volume);
      }
    })

    this.eventEmitter = PlayerEventEmitter.addListener('KALTURA_AVAILABLE_VIDEO_TRACKS_CHANGED', (event) => {
      if (this.props.onAvailableVideoTracksChanged) {
        this.props.onAvailableVideoTracksChanged(event);
      }
    })

    NativeModules.KalturaVideo.Setup(this.props.ottPartnerId, this.props.initOptions)

    if (this.props.media) {
      this.loadMedia(this.props.media.id, this.props.media.asset);
    } else if (this.props.source) {
      this.setMedia(this.props.source.uri);
    }
  }

  componentDidUpdate(prevProps) {
    //Pass along updated props
    // let newChildProps = {...this.props};
    // delete newChildProps.source;
    // this.childProps = {...newChildProps};


    //Handle custom props
    if (this.props.selectedVideoTrack !== prevProps.selectedVideoTrack) {
      NativeModules.KalturaVideo.SelectVideoTrack(this.props.selectedVideoTrack);
    }

    if (this.props.media !== prevProps.media && this.props.media) {
      this.loadMedia(this.props.media.id, this.props.media.asset);
    }

    if (this.props.source !== prevProps.source && this.props.source) {
      this.setMedia(this.props.source.uri);
    }
  }

  render() {
    this.sanitizeProps(this.props);
    return <Video ref={this.videoRef} {...this.childProps} />
  }

  // Kaltura custom functions
  loadMedia = (assetId, options) => {
    NativeModules.KalturaVideo.LoadMedia(assetId, options)
  }

  setMedia = (url) => {
    NativeModules.KalturaVideo.SetMedia(url)
  }

  // Passthrough functions
  play = () => {
    this.videoRef.current.play()
  }

  pause = () => {
    this.videoRef.current.pause()
  }

  stop = () => {
    this.videoRef.current.stop()
  }

  replay = () => {
    NativeModules.KalturaVideo.Replay()
  }

  seek = (time) => {
    this.videoRef.current.seek(time)
  }

  getStatistics = () => {
    return this.videoRef.current.getStatistics()
  };

  getPlayerInformation = () => {
    return this.videoRef.current.getPlayerInformation()
  };

  getBufferLength = () => {
    return this.videoRef.current.getBufferLength()
  };

  getLiveSeekableRanges = () => {
    return this.videoRef.current.getLiveSeekableRanges()
  };
}
