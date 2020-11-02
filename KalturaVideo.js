import React from 'react';
import { findNodeHandle, NativeModules, NativeEventEmitter } from 'react-native';
import { Video } from '@youi/react-native-youi';

const PlayerEventEmitter = new NativeEventEmitter(NativeModules.KalturaVideo);

export default class KalturaVideo extends React.Component {

  constructor(props) {
    super(props)
    this.playerEventEmitter = null
    this.videoRef = React.createRef();
    this.childProps = { ...props };
    delete this.childProps.source;
  }

  componentDidMount() {
    // Must be called before any other method on the native module
    NativeModules.KalturaVideo.ConnectToPlayer(findNodeHandle(this.videoRef.current));

    this.eventEmitter = PlayerEventEmitter.addListener('KALTURA_AD_CONTENT_PAUSE_REQUESTED', () => {
      if (this.props.onAdContentPauseRequested) {
        this.props.onAdContentPauseRequested();
      }
    })

    this.eventEmitter = PlayerEventEmitter.addListener('KALTURA_AD_CONTENT_RESUME_REQUESTED', () => {
      if (this.props.onAdContentResumeRequested) {
        this.props.onAdContentResumeRequested();
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
    
    // Custom prop init
    if (this.props.zIndex) {
      NativeModules.KalturaVideo.SetZIndex(this.props.zIndex)
    }
    
    if (this.props.media) {
      this.loadMedia(this.props.media.id, this.props.media.asset);
    } else if (this.props.source) {
      this.setMedia(this.props.source.uri);
    }
  }

  componentDidUpdate(prevProps) {
    //Pass along updated props
    newChildProps = {...this.props};
    delete newChildProps.source;
    this.childProps = {...newChildProps};

    //Handle custom props
    if (this.props.selectedVideoTrack !== prevProps.selectedVideoTrack) {
      NativeModules.KalturaVideo.SelectVideoTrack(this.props.selectedVideoTrack);
    }

    if (this.props.zIndex !== prevProps.zIndex) {
      NativeModules.KalturaVideo.SetZIndex(this.props.zIndex)
    }

    if (this.props.media !== prevProps.media) {
      this.loadMedia(this.props.media.id, this.props.media.asset);
    }

    if (this.props.source !== prevProps.source) {
      this.setMedia(this.props.source.uri);
    }
  }

  render() {
    return <Video ref={this.videoRef} {...this.childProps} />
  }

  // Kaltura custom functions
  loadMedia = (assetId, options) => {
    NativeModules.KalturaVideo.LoadMedia(assetId, options)
  }

  setMedia = (url) => {
    NativeModules.KalturaVideo.SetMedia(url)
  }

  setZIndex = (zIndex) => {
    NativeModules.KalturaVideo.SetZIndex(zIndex)
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
