#!/bin/bash


set -ex
VERSION=$(grep frameworksversion package.json  | awk -F" " '{ print $2 }' | sed 's/\"//g' |  sed 's/\,//g')
VER="v${VERSION}"
cd $1

curl -L -o KalturaPlayerYouiFrameworks-${VER}.zip "https://playkit.s3.amazonaws.com/prebuilt/kux/ios/${VER}/KalturaPlayerYouiFrameworks-${VER}.zip"


unzip -oq KalturaPlayerYouiFrameworks-${VER}.zip
rm KalturaPlayerYouiFrameworks-${VER}.zip

