#!/bin/bash

set -ex

cd "$(dirname $0)"/YouiKalturaPlayerBridge

TARGET_DIR="$(cd ../../../youi; pwd)"/ios-libs

read -p "Remove '$TARGET_DIR'?" Y

if [ "$Y" != "y" ] && [ "$Y" != "Y" ]; then
    exit
fi


rm -rf "$TARGET_DIR" || true
mkdir "$TARGET_DIR"

pod deintegrate
rm -rf Pods || true
rm Podfile.lock || true

pod install --repo-update

rsync -av Pods/_Prebuild/GeneratedFrameworks/ "$TARGET_DIR"

cd "$TARGET_DIR"

mv GoogleAds-IMA-iOS-SDK GoogleInteractiveMediaAds
