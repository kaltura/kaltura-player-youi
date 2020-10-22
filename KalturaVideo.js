import React from 'react';
import { findNodeHandle, NativeModules, NativeEventEmitter } from 'react-native';
import { Video } from '@youi/react-native-youi';

const PlayerEventEmitter = new NativeEventEmitter(NativeModules.KalturaVideo);

export default class KalturaVideo extends React.Component {

  constructor(props) {
    super(props)
    this.playerEventEmitter = null
    this.videoRef = React.createRef()
  }

  componentDidMount() {
    NativeModules.KalturaVideo.ConnectToPlayer(findNodeHandle(this.videoRef.current));

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
  }

  render() {
    return <Video ref={this.videoRef} {...this.props} />
  }

  // Kaltura custom functions
  setup = (partnerId, options) => {
    NativeModules.KalturaVideo.setup(findNodeHandle(this.videoRef.current), partnerId, options)
  }

  load = (assetId, options) => {
    NativeModules.KalturaVideo.load(findNodeHandle(this.videoRef.current), assetId, options)
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
