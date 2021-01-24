#!/bin/bash


set -ex
VERSION=$(grep version package.json  | awk -F" " '{ print $2 }' | sed 's/\"//g' |  sed 's/\,//g')
VER="v${VERSION}"
cd $1

curl -L -o KalturaPlayerYouiFrameworks-${VER}.zip "https://bintray.com/kaltura/ios-prebuilt/download_file?file_path=kaltura%2Fios-prebuilt%2FKalturaPlayerYouiFrameworks%2F${VER}%2FKalturaPlayerYouiFrameworks-${VER}.zip"

unzip -oq KalturaPlayerYouiFrameworks-${VER}.zip
rm KalturaPlayerYouiFrameworks-${VER}.zip

