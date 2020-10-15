import React, { Component } from "react";
import { AppRegistry, Button, BackHandler, Image, StyleSheet, View, NativeEventEmitter, Text } from "react-native";
import { FormFactor } from "@youi/react-native-youi";
import { NativeModules } from 'react-native';

//OTT 3009
const PhoenixBaseUrl = "https://rest-us.ott.kaltura.com/v4_5/api_v3/";
const OttPartnerId = 3009;
const OttMediaId = "548576";
const OttMediaFormat = "Mobile_Main";
const OttMediaProtocol = "http"; // "https"
const OttAssetType = "media";
const OttPlaybackContextType = "playback";

const Player = NativeModules.PKPlayerWrapper;
const Downloader = NativeModules.PKDownloadWrapper;

const downloadEventEmitter = new NativeEventEmitter(NativeModules.PKDownloadWrapper);
const playerEventEmitter = new NativeEventEmitter(NativeModules.PKPlayerWrapper);

var initOptions = {
  "serverUrl": PhoenixBaseUrl,
  "autoplay": true,
  "preload": true,
  "allowCrossProtocolRedirect": true,
  "allowFairPlayOnExternalScreens" : true,
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
  "urlType": "PLAYMANIFEST",
  "startPosition": 0,
  "plugins" : {
    "ima" : {
      "adTagUrl" : "https://pubads.g.doubleclick.net/gampad/ads?sz=640x480&iu=/124319096/external/single_ad_samples&ciu_szs=300x250&impl=s&gdfp_req=1&env=vp&output=vast&unviewed_position_start=1&cust_params=deployment%3Ddevsite%26sample_ct%3Dskippablelinear&correlator=",
      "alwaysStartWithPreroll" : true,
      "enableDebugMode" : false
    },
    "youbora" : {
      "accountCode" : "kalturatest",
      "username": "aaa",
      "userEmail" : "aaa@gmail.com",
      "userType" : "paid",       // optional any string - free / paid etc.
      "houseHoldId": "qwerty",   // optional which device is used to play
      "httpSecure": true,        // youbora events will be sent via https
      "appName": "YouiBridgeTesdtApp",
      "appReleaseVersion": "v1.0.0",
      "extraParam1" : "param1",
      "extraParam2" : "param2"
    }
  }
}

export default class YiReactApp extends Component {

  state = {
    playing: false
  };

  componentDidMount() {
    console.log("*** componentDidMount");

    BackHandler.addEventListener("hardwareBackPress", () => {
      console.log("*** BackButton HAS been pressed");
      //NativeModules.PKPlayerWrapper.destroy();
      //playerEventEmitter && playerEventEmitter.removeAllListeners();
      //downloadEventEmitter && downloadEventEmitter.removeAllListeners();
      });

    this.playerEventSubscriptionLoadMediaSuccess = playerEventEmitter.addListener("loadMediaSuccess", payload => {
      console.log("*** loadMediaSuccess PlayerEvent: " + JSON.stringify(payload));
      // {
      //   "duration": 156000,
      //   "id": "548576",
      //   "isVRMediaType": false,
      //   "mediaType": "Vod",
      //   "metadata": {
      //     "Parental Rating": "R",
      //     "entryId": "1_8imv40i1",
      //     "Runtime": "88",
      //     "Episode name": "",
      //       "Director": "Andrew J. Cohen",
      //     "name": "The House",
      //     "Country": "United States of America",
      //     "Catchup allowed": "false",
      //     "SeriesId": "",
      //     "description": "A dad convinces his friends to start an illegal casino in his basement after he and his wife spend their daughter's college fund.",
      //     "528X310": "http://IMAGES-OR.OTT.KALTURA.COM/Service.svc/GetImage/p/3009/entry_id/3eed93103f02470da563f8efc1d332e1_1/version/13/width/528/height/310/quality/100",
      //     "Release Year": "2017",
      //     "Main cast": "Ryan Simpkins",
      //     "free": "false",
      //     "External ID": "TMDB_345914",
      //     "PPV Module": "OTTTAGPPV_Module",
      //     "assetIds": "548576",
      //     "Provider": "OTTTAGProvider",
      //     "Provider ID": "OTTTAGProvider_ID",
      //     "Genre": "Comedy"
      //   },
      //   "name": "The House",
      //   "sources": [
      //     {
      //       "id": "1199753",
      //       "mediaFormat": "hls",
      //       "url": "https://rest-us.ott.kaltura.com/api_v3/service/assetFile/action/playManifest/partnerId/3009/assetId/548576/assetType/media/assetFileId/1199753/contextType/PLAYBACK/a.m3u8"
      //     }
      //   ]
      // }
      this.setState({
        logText: "loadMediaSuccess"
      });
    }
    );

    this.playerEventSubscriptionLoadMediaFailed = playerEventEmitter.addListener("loadMediaFailed", payload => {
      console.log("*** loadMediaFailed PlayerEvent: " + JSON.stringify(payload));
      // {
      //   "code": "500007",
      //   "extra": "KalturaAPIException",
      //   "message": "Asset not found",
      //   "name": "OTTError"
      // }
      this.setState({
        logText: "loadMediaFailed" + " " + payload.message
      });
    }
    );

    this.playerEventSubscriptionEnded = playerEventEmitter.addListener("ended", payload => {
      console.log("*** ended PlayerEvent: " + JSON.stringify(payload));
      this.setState({
        logText: "ended" + " " + payload
      });
    }
    );

    this.playerEventSubscriptionCanPlay = playerEventEmitter.addListener("canPlay", payload => {
      console.log("*** canPlay PlayerEvent: " + JSON.stringify(payload));
      this.setState({
        logText: "canPlay" + " " + payload
      });
    }
    );

    this.playerEventSubscriptionPlaying = playerEventEmitter.addListener("playing", payload => {
      console.log("*** playing PlayerEvent: " + JSON.stringify(payload));
      this.setState({
        logText: "playing" + " " + payload
      });
    }
    );

    this.playerEventSubscriptionPlay = playerEventEmitter.addListener("play", payload => {
      console.log("*** play PlayerEvent: " + JSON.stringify(payload));
      this.setState({
        logText: "play" + " " + payload
      });
    }
    );

    this.playerEventSubscriptionPause = playerEventEmitter.addListener("pause", payload => {
      console.log("*** pause PlayerEvent: " + JSON.stringify(payload));
      this.setState({
        logText: "pause" + " " + payload
      });
    }
    );

    this.playerEventSubscriptionStopped = playerEventEmitter.addListener("stopped", payload => {
      console.log("*** stopped PlayerEvent: " + JSON.stringify(payload));
      this.setState({
        logText: "stopped" + " " + payload
      });
    }
    );

    this.playerEventSubscriptionDurationChanged = playerEventEmitter.addListener("durationChanged", payload => {
      console.log("*** durationChanged PlayerEvent: " + JSON.stringify(payload));
      //{ "duration": 156.131 }
      this.setState({
        logText: "durationChanged" + " " + payload.duration
      });
    }
    );

    this.playerEventSubscriptionTimeUpdate = playerEventEmitter.addListener("timeUpdate", payload => {
      console.log("*** timeUpdate PlayerEvent: " + JSON.stringify(payload));
      //{ "position": 58.542 }
      this.setState({
        logText: "timeUpdate" + " " + payload.position
      });
    }
    );

    this.playerEventSubscriptionStateChanged = playerEventEmitter.addListener("stateChanged", payload => {
      console.log("*** stateChanged PlayerEvent: " + JSON.stringify(payload));
     //{ "newState": "BUFFERING" }
     //{ "newState": "LOADING" }
     //{ "newState": "READY" }
     //{ "newState": "IDLE" }
      this.setState({
        logText: "stateChanged" + " " + payload.newState
      });
    }
    );

    this.playerEventSubscriptionTracksAvailable = playerEventEmitter.addListener("tracksAvailable", payload => {
      console.log("*** tracksAvailable PlayerEvent: " + JSON.stringify(payload));
      console.log("*** tracksAvailable length: " + Object.keys(payload).length);
      //{"audio":[],"text":[],"video":[{"id":"Video:0,0,-1","height":0,"isAdaptive":true,"isSelected":true,"bitrate":0,"width":0},{"id":"Video:0,0,0","height":360,"isAdaptive":false,"isSelected":false,"bitrate":508450,"width":640},{"id":"Video:0,0,1","height":360,"isAdaptive":false,"isSelected":false,"bitrate":713517,"width":640},{"id":"Video:0,0,2","height":360,"isAdaptive":false,"isSelected":false,"bitrate":966712,"width":640},{"id":"Video:0,0,3","height":720,"isAdaptive":false,"isSelected":false,"bitrate":1343366,"width":1280}]}
      this.setState({
        logText: "tracksAvailable" + " " + Object.keys(payload).length
      });
    }
    );

    this.playerEventSubscriptionVideoTrackChanged = playerEventEmitter.addListener("videoTrackChanged", payload => {
      console.log("*** videoTrackChanged PlayerEvent: " + JSON.stringify(payload));  
      //{"id":"Video:0,0,-1", "width":0, "height":0, "isAdaptive":true, "isSelected":true, "bitrate":0 }
    }
    );

    this.playerEventSubscriptionAudioTrackChanged = playerEventEmitter.addListener("audioTrackChanged", payload => {
      //{"id": "Audio:0,0,-1", "bitrate": 1234, "language": "en", "label": "English", "channelCount": 1, "isSelected": true}
      console.log("*** audioTrackChanged PlayerEvent: " + JSON.stringify(payload));
      this.setState({
        logText: "audioTrackChanged" + " " + payload.id
      });
    }
    );

    this.playerEventSubscriptionTextTrackChanged = playerEventEmitter.addListener("textTrackChanged", payload => {
      console.log("*** textTrackChanged PlayerEvent: " + JSON.stringify(payload));
       //{"id": "Text:0,0,-1", "language": "en", "label": "English", "isSelected": true}
      this.setState({
        logText: "textTrackChanged" + " " + payload.id
      });
    }
    );

    this.playerEventSubscriptionSeeking = playerEventEmitter.addListener("seeking", payload => {
      console.log("*** seeking PlayerEvent: " + JSON.stringify(payload));
      //{ "targetPosition":  100 }
      this.setState({
        logText: "seeking" + " " + payload.targetPosition
      });
    }
    );

    this.playerEventSubscriptionSeeked = playerEventEmitter.addListener("seeked", payload => {
      console.log("*** seeked PlayerEvent: " + JSON.stringify(payload));
      this.setState({
        logText: "seeked" + " " + payload
      });
    }
    );

    this.playerEventSubscriptionError = playerEventEmitter.addListener("error", payload => {
      console.log("*** error PlayerEvent: " + JSON.stringify(payload));
      //{"errorType": "SOURCE_ERROR","errorSeverity": "Fatal","errorMessage": "com.kaltura.android.exoplayer2.upstream.HttpDataSource$HttpDataSourceException: Unable to connect","errorCause":"com.kaltura.android.exoplayer2.upstream.HttpDataSource$HttpDataSourceException: Unable to connect"}
      this.setState({
        logText:  "error" + " " + payload.errorType
      });
    }
    );

    //ADS

    this.playerEventSubscriptionAdProgress = playerEventEmitter.addListener("adProgress", payload => {
      console.log("*** adProgress AdEvent: " + JSON.stringify(payload));
      //{ "currentAdPosition": 2 }
      this.setState({
        logText: "adProgress" + " " + payload.currentAdPosition
      });
    }
    );

    this.playerEventSubscriptionAdCuepointsChanged = playerEventEmitter.addListener("adCuepointsChanged", payload => {
      console.log("*** adCuepointsChanged AdEvent: " + JSON.stringify(payload));
      //{"adPluginName":"ima","cuePoints":[0],"hasPreRoll":true,"hasMidRoll":false,"hasPostRoll":false}
      this.setState({
        logText: "adCuepointsChanged" + " " + payload.hasPostRoll
      });
    }
    );

    this.playerEventSubscriptionAdStarted = playerEventEmitter.addListener("adStarted", payload => {
      console.log("*** adStarted AdEvent: " + JSON.stringify(payload));
      this.setState({
        logText: "adStarted" + " " + payload
      });
    }
    );

    this.playerEventSubscriptionAdCompleted= playerEventEmitter.addListener("adCompleted", payload => {
      console.log("*** adCompleted AdEvent: " + JSON.stringify(payload));
      this.setState({
        logText:  "adCompleted" + " " + payload
      });
    }
    );

    this.playerEventSubscriptionAdPaused = playerEventEmitter.addListener("adPaused", payload => {
      console.log("*** adPaused AdEvent: " + JSON.stringify(payload));
      this.setState({
        logText: "adPaused" + " " + payload
      });
    }
    );

    this.playerEventSubscriptionAdResumed = playerEventEmitter.addListener("adResumed", payload => {
      console.log("*** adResumed AdEvent: " + JSON.stringify(payload));
      this.setState({
        logText: "adResumed" + " " + payload
      });
    }
    );

    this.playerEventSubscriptionAdBufferStart = playerEventEmitter.addListener("adBufferStart", payload => {
      console.log("*** adBufferStart AdEvent: " + JSON.stringify(payload));
      this.setState({
        logText:  "adBufferStart" + " " + payload
      });
    }
    );

    this.playerEventSubscriptionAdSkipped = playerEventEmitter.addListener("adSkipped", payload => {
      console.log("*** adSkipped AdEvent: " + JSON.stringify(payload));
      this.setState({
        logText: "adSkipped" + " " + payload
      });
    }
    );

    this.playerEventSubscriptionAdClicked = playerEventEmitter.addListener("adClicked", payload => {
      console.log("*** adClicked AdEvent: " + JSON.stringify(payload));
      //{ "clickThruUrl":  "http://google.com" }
      this.setState({
        logText: "adClicked" + " " + payload.clickThruUrl
      });
    }
    );

    this.playerEventSubscriptionAdRequested = playerEventEmitter.addListener("adRequested", payload => {
      console.log("*** adRequested AdEvent: " + JSON.stringify(payload));
      //{ "adTagUrl": "https://pubads.g.doubleclick.net/gampad/ads?sz=640x480&iu=/124319096/external/single_ad_samples&ciu_szs=300x250&impl=s&gdfp_req=1&env=vp&output=vast&unviewed_position_start=1&cust_params=deployment%3Ddevsite%26sample_ct%3Dskippablelinear&correlator=" }
      this.setState({
        logText: "adRequested" + " " + payload.adTagUrl
      });
    }
    );

    this.playerEventSubscriptionAdContentPauseRequested = playerEventEmitter.addListener("adContentPauseRequested", payload => {
      console.log("*** adContentPauseRequested AdEvent: " + JSON.stringify(payload));
      this.setState({
        logText: "adContentPauseRequested" + " " + payload
      });
    }
    );

    this.playerEventSubscriptionAdContentResumeRequested = playerEventEmitter.addListener("adContentResumeRequested", payload => {
      console.log("*** adContentResumeRequested AdEvent: " + JSON.stringify(payload));
      this.setState({
        logText: "adContentResumeRequested" + " " + payload
      });
    }
    );

    this.playerEventSubscriptionAllAdsCompleted= playerEventEmitter.addListener("allAdsCompleted", payload => {
      console.log("*** allAdsCompleted AdEvent: " + JSON.stringify(payload));
      this.setState({
        logText: "allAdsCompleted" + " " + payload
      });
    }
    );

    this.playerEventSubscriptionAdError = playerEventEmitter.addListener("adError", payload => {
      console.log("*** adError AdEvent: " + JSON.stringify(payload));
      //{ "errorType":"FAILED_TO_REQUEST_ADS", "errorSeverity":"Fatal", "errorMessage":"There was a problem requesting ads from the server. Caused by: 6 adTagUrl=https://google.123.pubads.g.doubleclick.net/gampad/ads?sz=640x480&iu=/124319096/external/single_ad_samples&ciu_szs=300x250&impl=s&gdfp_req=1&env=vp&output=vast&unviewed_position_start=1&cust_params=deployment%3Ddevsite%26sample_ct%3Dskippablelinear&correlator=", "errorCause":"null"}
      this.setState({
        logText: "adError" + " " + payload.errorType
      });
    }
    );

    ///OFFLINE

    this.downloadEventSubscriptionOfflineManagerStarted = downloadEventEmitter.addListener("offlineManagerStarted", payload => {
      console.log("*** offlineManagerStarted DownloadEvent: " + JSON.stringify(payload));
      this.setState({
        logText:  "offlineManagerStarted" + " " + payload
      });
    }
    );

    this.downloadEventSubscriptionOnAssetDownloadFailed = downloadEventEmitter.addListener("onAssetDownloadFailed", payload => {
      //{ "id": + "assetId", "errorMessage":  "errorMessage" }
      console.log("*** onAssetDownloadFailed DownloadEvent: " + JSON.stringify(payload));
      this.setState({
        logText:  "onAssetDownloadFailed" + " " + payload.id
      });
    }
    );

    this.downloadEventSubscriptionOnAssetDownloadComplete = downloadEventEmitter.addListener("onAssetDownloadComplete", payload => {
      //{ "id": "assetId" }
      console.log("*** onAssetDownloadComplete DownloadEvent: " + JSON.stringify(payload));
      this.setState({
        logText: "onAssetDownloadComplete" + " " + payload.id
      });
    }
    );

    this.downloadEventSubscriptionOnAssetDownloadPending = downloadEventEmitter.addListener("onAssetDownloadPending", payload => {
       //{ "id": "assetId" }
      console.log("*** onAssetDownloadPending DownloadEvent: " + JSON.stringify(payload));
      this.setState({
        logText: "onAssetDownloadPending" + " " + payload.id
      });
    }
    );

    this.downloadEventSubscriptionOnAssetDownloadPaused = downloadEventEmitter.addListener("onAssetDownloadPaused", payload => {
       //{ "id": "assetId" }
      console.log("*** onAssetDownloadPaused DownloadEvent: " + JSON.stringify(payload));
      this.setState({
        logText: "onAssetDownloadPaused" + " " + payload.id
      });
    }
    );

    this.downloadEventSubscriptionOnAssetRegistered = downloadEventEmitter.addListener("onAssetRegistered", payload => {
      //{ "id": + "assetId", "currentRemainingTime":  secondsLeft }
      console.log("*** onAssetRegistered DownloadEvent: " + JSON.stringify(payload));
      this.setState({
        logText:  "onAssetRegistered" + " " + payload.id
      });
    }
    );

    this.downloadEventSubscriptionOnAssetRegisterError = downloadEventEmitter.addListener("onAssetRegisterError", payload => {
      //{ "id": + "assetId", "errorMessage":  "errorMessage" }
      console.log("*** onAssetRegisterError DownloadEvent: " + JSON.stringify(payload));
      this.setState({
        logText: "onAssetRegisterError" + " " + payload.id
      });
    }
    );

    this.downloadEventSubscriptionOnAssetStateChanged = downloadEventEmitter.addListener("onAssetStateChanged", payload => {
      // {
      //   "id": "assetId",
      //   "downloadedState": "started", // none, prepared, started, completed, failed, removing, paused
      //   "bytesDownloaded": 100,
      //   "totalBytesEstimated": 1000
      // }
      console.log("*** onAssetStateChanged DownloadEvent: " + JSON.stringify(payload));
      this.setState({
        logText: "onAssetStateChanged" + " " + payload.id
      });
    }
    );

    this.downloadEventSubscriptionOnAssetRemoved = downloadEventEmitter.addListener("onAssetRemoved", payload => {
      //{ "id": "assetId" }
      console.log("*** onAssetRemoved DownloadEvent: " + JSON.stringify(payload));
      this.setState({
        logText:  "onAssetRemoved" + " " + payload.id
      });
    }
    );

    this.downloadEventSubscriptionOnDownloadProgress = downloadEventEmitter.addListener("onDownloadProgress", payload => {
      // {
      //   "id": "assetId",
      //   "bytesDownloaded": 100,
      //   "totalBytesEstimated": 1000
      // }
      console.log("*** onDownloadProgress DownloadEvent: " + JSON.stringify(payload));
      this.setState({
        logText:  "onDownloadProgress" + " " + payload.id
      });
    }
    );
  }

  componentWillUnmount() {
    console.log("*** componentWillUnmount");

    playerEventEmitter && playerEventEmitter.removeAllListeners();
    downloadEventEmitter && downloadEventEmitter.removeAllListeners();
  }

  constructor() {
    super();
    console.log("*** constructor called");

    console.log("*** HELLO JS ***");

    this.state = {
      logText: 'My Original Text'
    }

    NativeModules.PKDownloadWrapper.setup(OttPartnerId, PhoenixBaseUrl);
    if (initOptions.autoplay) {
      this.state.playing = true;
    }
    NativeModules.PKPlayerWrapper.setup(OttPartnerId, initOptions);
  }

  onMyButtonPress = () => {
    this.setState({
      playing: !this.state.playing
    });

    if (this.state.playing) {
      NativeModules.PKPlayerWrapper.pause();
      //NativeModules.PKPlayerWrapper.seekTo(12);
    } else {
      NativeModules.PKPlayerWrapper.play();
    }
  };

  onDownloadPress = () => {
    NativeModules.PKDownloadWrapper.offline_start(OttMediaId, asset);
  }

  onLoadAsset = () => {
    //NativeModules.PKPlayerWrapper.setAutoplay(true);
    //NativeModules.PKPlayerWrapper.setKS("SSSSSSSS");
    //NativeModules.PKPlayerWrapper.setLogLevel("VERBOSE");
    NativeModules.PKPlayerWrapper.load(OttMediaId, asset);
  }

  render() {
    const { playing } = this.state;

    return (
      <View style={[styles.mainContainer]}>
        <View style={[styles.headerContainer, styles.header]}>
          <Text style={styles.bodyText}>Log: {this.state.logText}</Text>
          <View
            style={styles.imageContainer}
            focusable={true}>
            <Image style={styles.image} source={{ uri: `res://drawable/default1/kaltura-logo1.png` }} />
          </View>
          <View
            focusable={true}
            style={{
              flexDirection: "row",
              height: 100,
              padding: 20
            }}
          >
            <Button
              onPress={this.onMyButtonPress}
              title={`${playing ? "Pause" : "Play"}`}
              textStyle={styles.textStyle}
              buttonStyle={styles.button}
              defaultFocus={true}
            />
            <Button
              onPress={this.onLoadAsset}
              title="Load Asset"
              textStyle={styles.textStyle}
              buttonStyle={styles.button}
            />
            <Button
              onPress={this.onDownloadPress}
              title="Download"
              textStyle={styles.textStyle}
              buttonStyle={styles.button}
            />
          </View>
        </View>
      </View>
    );
  }
}

const styles = StyleSheet.create({
  mainContainer: {
    backgroundColor: 'transparent',
    flex: 1
  },
  headerContainer: {
    backgroundColor: 'transparent',
    justifyContent: "center",
    alignItems: "center",
    flex: 2
  },
  imageContainer: {
    justifyContent: "center",
    alignItems: "center",
    flex: 2
  },
  image: {
    height: FormFactor.isTV ? 225 : 75,
    width: FormFactor.isTV ? 225 : 75,
    resizeMode: "contain"
  },
  buttonContainer: {
    flex: 1,
    justifyContent: "center",
    alignItems: "center"
  },
  bodyContainer: {
    backgroundColor: 'transparent',
    alignItems: "center",
    justifyContent: "center",
    flex: 1
  },
  headlineText: {
    marginBottom: 10,
    color: "#333333",
    textAlign: "center"
  },
  bodyText: {
    color: "#333333",
    textAlign: "center"
  }
});

AppRegistry.registerComponent("YiReactApp", () => YiReactApp);
