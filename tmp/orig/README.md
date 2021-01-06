# Kaltura Player Bridge for You.i Platform

## iOS tvOS Frameworks

The iOS implementation of the bridge uses frameworks that are normally built on the fly using Cocoapods. Since the you.i SDK [doesn't support Cocoapods](https://stackoverflow.com/c/youi-tv/questions/3912), the frameworks have to be prebuilt.

### Building the Frameworks

This relies on Cocoapods with a plugin called [cocoapods-binary](https://github.com/leavez/cocoapods-binary). The plugin builds all frameworks during the `pod install` process. 

Follow the steps from the repo's [ReadMe guide](https://github.com/leavez/cocoapods-binary). Verify you got version 0.4.4.  
In order for the tvOS to work a fork has been created and the change has been done in branch [tvOS_Support](https://github.com/x-NR-x/cocoapods-binary/tree/tvOS_Support). Take the file 'build_framework.rb' from the branch and replace it with the installed one, which can be found in this path:  
/Library/Ruby/Gems/2.6.0/gems/cocoapods-binary-0.4.4/lib/cocoapods-binary/rome

A script has been created in order to generate the frameworks per OS, the script can be found under the ‘scripts’ folder in the bridge repo per OS, within ‘ios-libs’ for the iOS frameworks and within ‘tvos-libs’ for the tvOS frameworks. The ‘build.sh' script works with the ‘Podfile’ in the project located in the same folder.

The build script will clean everything first, generate the frameworks and add them to the ‘ios-libs’ and ‘tvos-libs’ accordingly within the ‘youi’ folder.

### Downloading the Frameworks (Depricated for now)

Building the frameworks takes some time, so it might make more sense to use prebuilt frameworks. They are stored in bintray. The script `download.sh` in `ios-libs` downloads the frameworks.

## Installing or updating the changes to your project.

A script can be found in the 'scripts' folder, 'updatePlaykitSdkBridge'.  
Run the script from the 'scripts' folder, the path to your main folder is needed.  
For example:

```swift
./updatePlaykitSdkBridge.pl /Users/xxx/Documents/Projects/[main_folder]
```

The script will copy all the needed files and frameworks from the bridge repo into your project.
