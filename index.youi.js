import React, { Component } from "react";

import {
  AppRegistry,
  Text,
  View,
  SafeAreaView,
  StyleSheet,
  Button,
  TouchableOpacity
} from 'react-native';

import KalturaVideo from "./KalturaVideo";
import VideoGallery from './VideoGallery';

export default class YiReactApp extends Component {

  state = {
    isPlaying: false,
    duration: 0,
    currentTime: 0,
    sources: null,
    media: null,
    isMuted: false,
    videoSelected: false
  }

  videoRef = React.createRef()
  pauseBtnRef = React.createRef()

  onVideoSelected = (source) => {
    this.setState({sources: null, media: null, partnerId: null, initOptions: null});
    this.setState({...source});
    this.setState({ videoSelected: true });
  }

  loadBtnPressed = () => {
    if (this.state.videoSelected) {
      this.setState({ sources: null });
      this.setState({ videoSelected: false });
    }
  }

  pauseBtnPressed = () => this.state.isPlaying ? this.videoRef.current.pause() : this.videoRef.current.play();
  seekBtnPressed = () => this.videoRef.current.seek(this.state.currentTime + 10000);
  muteBtnPressed = () => this.setState({ 'isMuted': !this.state.isMuted });

  render() {
    const {isMuted, isPlaying, videoSelected, media, sources, partnerId, initOptions} = this.state;
    return (
      <SafeAreaView style={styles.container}>
        <Text style={styles.header}>Kaltura Video Sample</Text>
        {videoSelected ?
          <View style={styles.mainContainer}>
            <View style={styles.videoContainer}>
              <KalturaVideo
                ottPartnerId={partnerId}
                initOptions={initOptions}
                source={sources}
                media={media}
                muted={isMuted}
                style={styles.video}
                ref={this.videoRef}
                onPreparing={() => console.log("onPreparing called.")}
                onReady={() => console.log("onReady called.")}
                onPlaying={() => this.setState({ isPlaying: true })}
                onPaused={() => this.setState({ isPlaying: false })}
                onBufferingStarted={() => console.log("onBufferingStarted called.")}
                onBufferingEnded={() => console.log("onBufferingEnded called.")}
                onCurrentTimeUpdated={(currentTime) => {
                  console.log("currentTime " + currentTime)
                  this.setState({ currentTime: currentTime })
                }}
                onDurationChanged={(duration) => this.setState({ duration: duration })}
                onAvailableAudioTracksChanged={(tracks) => {
                  console.log("onAvailableAudioTracksChanged")
                  console.log(tracks.nativeEvent)
                }}
                onAvailableClosedCaptionsTracksChanged={(tracks) => {
                  console.log("onAvailableClosedCaptionsTracksChanged")
                  console.log(tracks.nativeEvent)
                }}
                onAvailableVideoTracksChanged={(tracks) => {
                  console.log("onAvailableVideoTracksChanged")
                  console.log(tracks.nativeEvent)
                }}
                onErrorOccurred={(error) => {
                  console.log("onErrorOccurred")
                  console.log(error.nativeEvent.message)
                }}/>
            </View>
            <View style={styles.buttonContainer}>
              <MyTouchableOpacity text={videoSelected ? 'Unload' : 'Load'} onPress={this.loadBtnPressed} />
              <MyTouchableOpacity text={isPlaying ? 'Pause' : 'Play'} onPress={this.pauseBtnPressed} disabled={!videoSelected} />
              <MyTouchableOpacity text={isMuted ? 'Unmute' : 'Mute'} onPress={this.muteBtnPressed} disabled={!videoSelected}/>
              <MyTouchableOpacity text={'Seek +10s'} onPress={this.seekBtnPressed} disabled={!videoSelected}/>
            </View>
          </View>
          : <VideoGallery onVideoSelected={this.onVideoSelected}/>}
      </SafeAreaView>
    );
  }
}

export function MyTouchableOpacity(props) {
  return <TouchableOpacity
      disabled={props.disabled}
      style={props.disabled ? { ...(props.style? props.style : styles.button), ...styles.buttonDisabled } : props.style? props.style : styles.button }
      onPress={props.onPress}>
      <Text style={styles.buttonText}>{props.text}</Text>
  </TouchableOpacity>;
}

const styles = StyleSheet.create({
  container: {
    backgroundColor: 'darkgrey',
    flex: 1
  },
  header: {
    color: 'white',
    fontWeight: 'bold',
    textAlign: 'center',
    flex: 1,
    marginBottom: 3
  },
  mainContainer: {
    backgroundColor: 'darkgrey',
    width: "100%",
    height: "100%"
  },
  videoContainer: {
    flex: 10,
    backgroundColor: 'black',
  },
  video: {
    width: "100%",
    height: "100%"
  },
  buttonContainer: {
    flex: 2,
    flexDirection: 'row',
    zIndex: 1000000
  },
  button: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: 'white',
    margin: 2,
    borderRadius: 3,
    maxWidth: 100
  },
  buttonDisabled: {
    backgroundColor: "#ccc",
    color: "#999"
  },
  buttonText: {
  }
})

AppRegistry.registerComponent("YiReactApp", () => YiReactApp);
