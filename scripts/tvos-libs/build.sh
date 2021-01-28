#!/bin/bash

set -ex

cd "$(dirname $0)"/YouiKalturaPlayerBridgetvOS

TARGET_DIR="$(cd ../../../youi; pwd)"/tvos-libs


rm -rf "$TARGET_DIR" || true
mkdir "$TARGET_DIR"

pod deintegrate
rm -rf Pods || true
rm Podfile.lock || true

pod install --repo-update

rsync -av "$(dirname $0)"/Pods/_Prebuild/GeneratedFrameworks/ "$TARGET_DIR"

cd "$TARGET_DIR"

mv GoogleAds-IMA-tvOS-SDK GoogleInteractiveMediaAds

#Fixing BP SmartLib paths and removing unneeded files
mv SmartLib-v3/Pod/* SmartLib-v3
rm -rf SmartLib-v3/Pod
rm -rf SmartLib-v3/LICENSE.txt
rm -rf SmartLib-v3/README.md
mv SmartLib-v3 SmartLib_tvOS
