#!/bin/bash

###Run this scripts from scripts folder


set -ex
VERSION=$(grep version ../package.json  | awk -F" " '{ print $2 }' | sed 's/\"//g' |  sed 's/\,//g')
VER="v${VERSION}"
#cd $1
echo "Building KalturaPlayerYouiFrameworks-$VER.zip" 
echo "buils ios-libs"
cd ios-libs
sh -x ./build.sh &
echo "build tvos-libs"
cd ../tvos-libs
sh -x ./build.sh &
mkdir -p ../outputs
cd ../outputs

rm -f KalturaPlayerYouiFrameworks-$VER.zip
zip -r KalturaPlayerYouiFrameworks-$VER.zip ../../youi/ios-libs ../../youi/tvos-libs
cd ../../youi
\rm -rf ./ios-libs
\rm -rf ./tvos-libs
OUTPUT=$(pwd)
echo "KalturaPlayerYouiFrameworks-$VER.zip is located in $OUTPUT"

