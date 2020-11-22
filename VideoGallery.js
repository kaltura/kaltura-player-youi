import React from 'react';
import { View, StyleSheet, Text, SectionList } from 'react-native';
import {MyTouchableOpacity} from './index.youi';

export const SOURCE_TYPE = {
    source: "source",
    media: "media"
};

const DATA = [
    {
        title: 'External source',
        playerConfig: {
            partnerId: 3009,
            initOptions : {
                "serverUrl": "https://rest-us.ott.kaltura.com/v4_5/api_v3/",
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
        },
        data: [{
            id: 'bd7acbea-c1b1-46c2-aed5-3ad53abb28ba',
            title: 'Source',
            type: SOURCE_TYPE.source,
            data: {
                sources: {
                    mediaInfo: {
                        uri: "https://cdnapisec.kaltura.com/p/2222401/sp/2222401/playManifest/entryId/1_f93tepsn/flavorIds/1_g0o8nje1,1_nt6egawf,1_7cgwjy2a,1_xc3jlgr7,1_cn83nztu,1_pgoeohrs/deliveryProfileId/5372/protocol/https/format/applehttp/a.m3u8",
                        mediaType: "vod",
                        mediaFormat: "hls",
                        drmData: {
                            licenseUri: "https://udrmv3.kaltura.com/fps/license?custom_data=eyJjYV9zeXN0ZW0iOiJPVlAiLCJ1c2VyX3Rva2VuIjoiZGpKOE1qSXlNalF3TVh3d1J2eV9JY3pYZUl5ckpoYXJjTEx0ek9QUW1Hc21Ba1dtaHRjY0F4VGNUUk9YSkVPTmJZb0M0T1dPbVpKUXNDa0w3LW5rSWtlOUY1UHowU092Q2xpdUY1d01DMkNyRzV6YVFPMXU0SnBURVE9PSIsImFjY291bnRfaWQiOjIyMjI0MDEsImNvbnRlbnRfaWQiOiIxX2Y5M3RlcHNuIiwiZmlsZXMiOiIxX2cwbzhuamUxLDFfbnQ2ZWdhd2YsMV83Y2d3ankyYSwxX3hjM2psZ3I3LDFfY244M256dHUsMV9wZ29lb2hycyJ9&signature=5YLMpxBeupk3lXRewpEuAz1rjbw%3D",
                            fpsCertificate: "MIIFETCCA/mgAwIBAgIISWLo8KcYfPMwDQYJKoZIhvcNAQEFBQAwfzELMAkGA1UEBhMCVVMxEzARBgNVBAoMCkFwcGxlIEluYy4xJjAkBgNVBAsMHUFwcGxlIENlcnRpZmljYXRpb24gQXV0aG9yaXR5MTMwMQYDVQQDDCpBcHBsZSBLZXkgU2VydmljZXMgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwHhcNMTYwMjAxMTY0NTQ0WhcNMTgwMjAxMTY0NTQ0WjCBijELMAkGA1UEBhMCVVMxKDAmBgNVBAoMH1ZJQUNPTSAxOCBNRURJQSBQUklWQVRFIExJTUlURUQxEzARBgNVBAsMClE5QU5HR0w4TTYxPDA6BgNVBAMMM0ZhaXJQbGF5IFN0cmVhbWluZzogVklBQ09NIDE4IE1FRElBIFBSSVZBVEUgTElNSVRFRDCBnzANBgkqhkiG9w0BAQEFAAOBjQAwgYkCgYEA2YmfdPWM86+te7Bbt4Ic6FexXwMeL+8AmExIj8jAaNxhKbfVFnUnuXzHOajGC7XDbXxsFbPqnErqjw0BqUoZhs+WVMy+0X4AGqHk7uRpZ4RLYganel+fqitL9rz9w3p41x8JfLV+lAej+BEN7zNeqQ2IsC4BxkViu1gA6K22uGsCAwEAAaOCAgcwggIDMB0GA1UdDgQWBBQK+Gmarl2PO3jtLP6A6TZeihOL3DAMBgNVHRMBAf8EAjAAMB8GA1UdIwQYMBaAFGPkR1TLhXFZRiyDrMxEMWRnAyy+MIHiBgNVHSAEgdowgdcwgdQGCSqGSIb3Y2QFATCBxjCBwwYIKwYBBQUHAgIwgbYMgbNSZWxpYW5jZSBvbiB0aGlzIGNlcnRpZmljYXRlIGJ5IGFueSBwYXJ0eSBhc3N1bWVzIGFjY2VwdGFuY2Ugb2YgdGhlIHRoZW4gYXBwbGljYWJsZSBzdGFuZGFyZCB0ZXJtcyBhbmQgY29uZGl0aW9ucyBvZiB1c2UsIGNlcnRpZmljYXRlIHBvbGljeSBhbmQgY2VydGlmaWNhdGlvbiBwcmFjdGljZSBzdGF0ZW1lbnRzLjA1BgNVHR8ELjAsMCqgKKAmhiRodHRwOi8vY3JsLmFwcGxlLmNvbS9rZXlzZXJ2aWNlcy5jcmwwDgYDVR0PAQH/BAQDAgUgMEgGCyqGSIb3Y2QGDQEDAQH/BDYBZ2diOGN5bXpsb21vdXFqb3p0aHg5aXB6dDJ0bThrcGdqOGNwZGlsbGVhMWI1aG9saWlyaW8wPQYLKoZIhvdjZAYNAQQBAf8EKwF5aHZlYXgzaDB2Nno5dXBqcjRsNWVyNm9hMXBtam9zYXF6ZXdnZXFkaTUwDQYJKoZIhvcNAQEFBQADggEBAIaTVzuOpZhHHUMGd47XeIo08E+Wb5jgE2HPsd8P/aHwVcR+9627QkuAnebftasV/h3FElahzBXRbK52qIZ/UU9nRLCqqKwX33eS2TiaAzOoMAL9cTUmEa2SMSzzAehb7lYPC73Y4VQFttbNidHZHawGp/844ipBS7Iumas8kT8G6ZmIBIevWiggd+D5gLdqXpOFI2XsoAipuxW6NKnnlKnuX6aNReqzKO0DmQPDHO2d7pbd3wAz5zJmxDLpRQfn7iJKupoYGqBs2r45OFyM14HUWaC0+VSh2PaZKwnSS8XXo4zcT/MfEcmP0tL9NaDfsvIWnScMxHUUTNNsZIp3QXA=",
                            scheme: "fairplay"
                        }
                    }
                    
                }
            }
        }]
    },
    {
        title: 'Account 3009',
        playerConfig: {
            partnerId: 3009,
            initOptions : {
                "serverUrl": "https://rest-us.ott.kaltura.com/v4_5/api_v3/",
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
                        "accountCode": "kalturatest",
                        "username": "aaa",
                        "userEmail": "aaa@gmail.com",
                        "userType": "paid",       // optional any string - free / paid etc.
                        "houseHoldId": "qwerty",   // optional which device is used to play
                        "httpSecure": true,        // youbora events will be sent via https
                        "appName": "YouiBridgeTesdtApp",
                        "appReleaseVersion": "v1.0.0",
                    }
                }
            }
        },
        data: [{
            id: '3ac68afc-c605-48d3-a4f8-fbd91aa97f63',
            title: 'Media',
            type: SOURCE_TYPE.media,
            data: {
                media: {
                    id: "548576",
                    asset: {
                        "format": "Mobile_Main",
                        "assetType": "media",
                        "protocol": "http",
                        "playbackContextType": "playback",
                        "urlType": "DIRECT",
                        "startPosition": 0,
                        "plugins": {
                            // "ima": {
                            //     "adTagUrl": "https://pubads.g.doubleclick.net/gampad/ads?sz=640x480&iu=/124319096/external/single_ad_samples&ciu_szs=300x250&impl=s&gdfp_req=1&env=vp&output=vast&unviewed_position_start=1&cust_params=deployment%3Ddevsite%26sample_ct%3Dskippablelinear&correlator=",
                            //     "alwaysStartWithPreroll": true,
                            //     "enableDebugMode": false
                            // },
                            "youbora": {
                                "extraParam1": "param1",
                                "extraParam2": "param2"
                            }
                        }
                    }
                }
            }
        }, {
            id: '3ac68afc-c605-48d3-a4f8-fbd91aa97f63',
            title: 'Media 2',
            type: SOURCE_TYPE.media,
            data: {
                media: {
                    id: "548579",
                    asset: {
                        "format": "Mobile_Main",
                        "assetType": "media",
                        "protocol": "http",
                        "playbackContextType": "playback",
                        "urlType": "DIRECT",
                        "startPosition": 0,
                        "plugins": {
                            "youbora": {
                                "extraParam1": "param3",
                                "extraParam2": "param4"
                            }
                        }
                    }
                }
            }
        }]
    }
];

function Item({ title, data, onPress }) {
    return (
        <View style={styles.item}>
            <MyTouchableOpacity text={title} style={styles.title} onPress={() => {onPress(data)}}/>
        </View>
    );
}

function VideoGallery(props) {
    return (
        <SectionList
            style={styles.list}
            sections={DATA}
            renderItem={({ item, section }) => <Item title={item.title} data={{...section.playerConfig, ...item.data}} onPress={props.onVideoSelected}/>}
            renderSectionHeader={({ section: { title } }) => (
                <Text style={styles.header}>{title}</Text>
            )}
            keyExtractor={(item, index) => item + index}
        />
    );
}

const styles = StyleSheet.create({
    list: {
        backgroundColor: 'gray'
    },
    header: {
        fontSize: 20,
        textAlign: 'center',
    },
    item: {
        backgroundColor: 'white',
        padding: 6,
        marginVertical: 4,
        marginHorizontal: 16,
    },
    title: {
        fontSize: 16,
    },
});

export default VideoGallery;
