# Source this file

cd $(dirname $0)

export ANDROID_NDK_HOME=$ANDROID_HOME/ndk-bundle
export ANDROID_SERIAL=emulator-5554
alias yGenAndroid='youi-tv generate -p android'
# alias buildAndroid='youi-tv build -p android'
alias yBuildAndroid='youi-tv build -p android'
alias yInstallAndroid='sh -c "cd youi/build/android/project && ./gradlew installDebug"'
alias yRunAndroid='sh -c "cd youi/build/android/project && ./gradlew startApplication"'
alias yStartServer='yarn start'
alias yLogcat='adb logcat --pid=`adb shell pidof -s tv.youi.myapp`'

yFailed() {say -vAlex "$1 failed"; false}
ySay() {say -vAlex "$1"}

yBuildAndRun() {
    (yBuildAndroid || yFailed build) && (ySay Installing && yInstallAndroid || yFailed install) && (yRunAndroid || yFailed run) && ySay Running && (yLogcat || yFailed logcat)
}

adbTunnel() {
    adb reverse tcp:8081 tcp:8081
}
