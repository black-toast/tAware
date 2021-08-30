# tAware
A thread monitoring library for Android

## Build

* Download [Android NDK r16b](https://developer.android.com/ndk/downloads/revision_history.html), set environment PATH. (support for armeabi has been removed since r17)

* Build and install the native libraries.

```
git submodule update --init

cd xHook

./build_libs.sh
./install_libs.sh

cp libxhook/libs ../sample/lib-taware/libs/
```
