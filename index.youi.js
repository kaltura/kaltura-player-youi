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

export default class YiReactApp extends Component {

  state = {
    isPlaying: false,
    duration: 0,
    currentTime: 0
  }

  videoRef = React.createRef()
  pauseBtnRef = React.createRef()

  pauseBtnPressed = () => {
    if (this.state.isPlaying) {
      this.videoRef.current.pause()
    } else {
      this.videoRef.current.play()
    }
  }  

  seekBtnPressed = () => {
    this.videoRef.current.seek(this.state.currentTime + 10000)
  }

  render() {
    return (
      <SafeAreaView style={styles.container}>

        <Text style={styles.header}>Kaltura Video Sample</Text>

        <KalturaVideo
          style={styles.video}
          ref={this.videoRef}
          ottPartnerId={OttPartnerId}
          initOptions={initOptions}
          // source={{
          //   uri: "https://playertest.longtailvideo.com/adaptive/eleph-audio/playlist.m3u8",
          //   type: 'HLS'
          // }}
          source={{
            id: OttMediaId,
            asset: asset
          }}
          onPreparing={() => console.log("onPreparing called.") }
	        onReady={() => console.log("onReady called.")}
          onPlaying={() => this.setState({isPlaying: true})}
          onPaused={() => this.setState({isPlaying: false})}
          onBufferingStarted={() => console.log("onBufferingStarted called.")}
          onBufferingEnded={() => console.log("onBufferingEnded called.")}
          onCurrentTimeUpdated={(currentTime) => {
            console.log("currentTime " + currentTime)
            this.setState({ currentTime: currentTime })}
          }
          onDurationChanged={(duration) => this.setState({ duration: duration })}
          onAdError={(data) => console.log(data)}
          onAvailableAudioTracksChanged={(tracks) => {
            console.log("onAvailableAudioTracksChanged")
            console.log(tracks)
          }}
          onAvailableClosedCaptionsTracksChanged={(tracks) => {
            console.log("onAvailableClosedCaptionsTracksChanged")
            console.log(tracks)
          }}
          onErrorOccurred={(error) =>  {
            console.log("onErrorOccurred")
            console.log(error.nativeEvent.message)
          }}
        />

        <View style={styles.buttonContainer}>
          <TouchableOpacity style={styles.button} onPress={this.pauseBtnPressed}>
            <Text style={styles.buttonText}>{this.state.isPlaying ? 'Pause' : 'Play'}</Text>
          </TouchableOpacity>
          <TouchableOpacity style={styles.button} onPress={this.seekBtnPressed}>
            <Text style={styles.buttonText}>{'Seek +10s'}</Text>
          </TouchableOpacity>
        </View>

      </SafeAreaView>
    );
  }
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
  video: {
    flex: 10
  },
  buttonContainer: {
    flex: 2,
    flexDirection: 'row'
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
  buttonText: {
  }
})

AppRegistry.registerComponent("YiReactApp", () => YiReactApp);


//OTT 3009

//not working
// const PhoenixBaseUrl = "https://3207.frs1.ott.kaltura.com/api_v3/";
// const OttPartnerId = 3207;
// const OttMediaId = "2305718";
// const OttMediaFormat = "STB_Devices_Main_HLS";
// const OttMediaProtocol = "https"; // "https"

const PhoenixBaseUrl = "https://rest-us.ott.kaltura.com/v4_5/api_v3/";
const OttPartnerId = 3009;
const OttMediaId = "548576";
const OttMediaFormat = "Mobile_Main";
const OttMediaProtocol = "http"; // "https"
const OttAssetType = "media";
const OttPlaybackContextType = "playback";

var initOptions = {
  "serverUrl": PhoenixBaseUrl,
  "autoplay": true,
  "preload": true,
  "allowCrossProtocolRedirect": true,
  "allowFairPlayOnExternalScreens": true,
  "shouldPlayImmediately": true,
  "networkSettings": {
    "autoBuffer": true,
    "preferredForwardBufferDuration": 30000,
    "automaticallyWaitsToMinimizeStalling": true
  },
  "abrSettings": {
    "minVideoBitrate": 600000,
    "maxVideoBitrate": 1500000
  },
  "trackSelection": {
    "textMode": "AUTO",
    "textLanguage": "en",
    "audioMode": "AUTO",
    "audioLanguage": "en",
  },
  "plugins": {
    "ima": {},
    "youbora": {
      "accountCode": "kalturatest"
    }
  }
}

var asset = {
  "format": OttMediaFormat,
  "assetType": OttAssetType,
  "protocol": OttMediaProtocol,
  "playbackContextType": OttPlaybackContextType,
  "urlType": "DIRECT",
  "startPosition": 0,
  "plugins": {
    "ima": {
      "adTagUrl": "https://pubads.g.doubleclick.net/gampad/ads?sz=640x480&iu=/124319096/external/single_ad_samples&ciu_szs=300x250&impl=s&gdfp_req=1&env=vp&output=vast&unviewed_position_start=1&cust_params=deployment%3Ddevsite%26sample_ct%3Dskippablelinear&correlator=",
      "alwaysStartWithPreroll": true,
      "enableDebugMode": false
    },
    "youbora": {
      "accountCode": "kalturatest",
      "username": "aaa",
      "userEmail": "aaa@gmail.com",
      "userType": "paid",       // optional any string - free / paid etc.
      "houseHoldId": "qwerty",   // optional which device is used to play
      "httpSecure": true,        // youbora events will be sent via https
      "appName": "YouiBridgeTesdtApp",
      "appReleaseVersion": "v1.0.0",
      "extraParam1": "param1",
      "extraParam2": "param2"
    }
  }
}