debug:
	echo ">>> COCOS BUILD ..." && \
		./build_native.sh && \
		echo ">>> ANT BUILD ..." && \
		ant debug -Dsdk.dir=/Users/michaelcontento/android-sdk-macosx && \
		ant installd -Dsdk.dir=/Users/michaelcontento/android-sdk-macosx && \
		~/android-sdk-macosx/platform-tools/adb logcat

debugfull:
	echo ">>> COCOS BUILD ..." && \
		./build_native.sh && \
		echo ">>> IMPORT AVALON LIBS ..." && \
		cp -r ../Vendors/avalon/avalon/platform/android-amazon/{gamecenter,payment}/java/* . && \
		cp -r ../Vendors/avalon/avalon/platform/android/{ads/provider,utils,ui}/java/* . && \
		rm -rf libs/samsungadhub.jar && \
		rm -rf src/com/avalon/ads/SamsungAdHubBridge.java && \
		echo ">>> ANT BUILD ..." && \
		ant debug -Dsdk.dir=/Users/michaelcontento/android-sdk-macosx && \
		ant installd -Dsdk.dir=/Users/michaelcontento/android-sdk-macosx && \
		~/android-sdk-macosx/platform-tools/adb logcat

release:
	echo ">>> COCOS BUILD ..." && \
		./build_native.sh && \
		echo ">>> ANT BUILD ..." && \
		ant release -Dsdk.dir=/Users/michaelcontento/android-sdk-macosx && \
		ant installr -Dsdk.dir=/Users/michaelcontento/android-sdk-macosx && \
		~/android-sdk-macosx/platform-tools/adb logcat

releasefull:
	echo ">>> COCOS BUILD ..." && \
		./build_native.sh && \
		echo ">>> IMPORT AVALON LIBS ..." && \
		cp -r ../Vendors/avalon/avalon/platform/android-amazon/{gamecenter,payment}/java/* . && \
		cp -r ../Vendors/avalon/avalon/platform/android/{ads/provider,utils,ui}/java/* . && \
		rm -rf libs/samsungadhub.jar && \
		rm -rf src/com/avalon/ads/SamsungAdHubBridge.java && \
		echo ">>> ANT BUILD ..." && \
		ant release -Dsdk.dir=/Users/michaelcontento/android-sdk-macosx && \
		ant installr -Dsdk.dir=/Users/michaelcontento/android-sdk-macosx && \
		~/android-sdk-macosx/platform-tools/adb logcat
