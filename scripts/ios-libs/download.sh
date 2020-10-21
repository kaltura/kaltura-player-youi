#!/bin/bash

set -ex

VERSION=v1
BASE_URL="https://dl.bintray.com/kaltura/ios-prebuilt"

FILES='GoogleInteractiveMediaAds.zip
KalturaNetKit.zip
KalturaPlayer.zip
ObjcExceptionBridging.zip
PlayKit.zip
PlayKitKava.zip
PlayKitProviders.zip
PlayKitUtils.zip
PlayKitYoubora.zip
PlayKit_IMA.zip
SwiftyJSON.zip
SwiftyXMLParser.zip
XCGLogger.zip
YouboraLib.zip'

TARGET_DIR=$(cd ../../youi; pwd)/ios-libs
rm -rf "$TARGET_DIR" || true

mkdir "$TARGET_DIR"
cd "$TARGET_DIR"

for F in $FILES
do
  echo $F 
  curl -LO "$BASE_URL/$VERSION/$F"
  unzip $F
  rm $F
done
