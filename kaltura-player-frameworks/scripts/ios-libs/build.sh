#!/bin/bash

set -ex

TARGET_DIR="$1"/ios-libs

cd "$1"/scripts/ios-libs/YouiKalturaPlayerBridge

if [ -d "$TARGET_DIR" ]
then
    echo "Directory $TARGET_DIR already exists."
    exit 0
else
    echo "Directory $TARGET_DIR does not exist, building frameworks."
fi

mkdir "$TARGET_DIR"

pod deintegrate
rm -rf Pods || true
rm Podfile.lock || true

pod install --repo-update

rsync -av Pods/_Prebuild/GeneratedFrameworks/ "$TARGET_DIR"

cd "$TARGET_DIR"

mv GoogleAds-IMA-iOS-SDK GoogleInteractiveMediaAds

#Fixing BP SmartLib paths and removing unneeded files
mv SmartLib-v3/Pod/* SmartLib-v3
rm -rf SmartLib-v3/Pod
rm -rf SmartLib-v3/LICENSE.txt
rm -rf SmartLib-v3/README.md
mv SmartLib-v3 SmartLib
