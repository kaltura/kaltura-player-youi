# Kaltura Player Bridge for You.i Platform

## iOS Frameworks

The iOS implementation of the bridge uses frameworks that are normally built on the fly using Cocoapods. Since the you.i SDK [doesn't support Cocoapods](https://stackoverflow.com/c/youi-tv/questions/3912), the frameworks have to be prebuilt.

### Building the Frameworks

This relies on Cocoapods with a plugin called [cocoapods-binary](https://github.com/leavez/cocoapods-binary). The plugin builds all frameworks during the `pod install` process. 

### Downloading the Frameworks

Building the frameworks takes some time, so it might make more sense to use prebuilt frameworks. They are stored in bintray. The script `download.sh` in `ios-libs` downloads the frameworks.

