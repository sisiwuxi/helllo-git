#!/bin/sh

[ -n "$ANDROID_BUILD_TOP" ] && [ ! -e $ANDROID_BUILD_TOP/hardware/porting ] && ln -s `pwd` $ANDROID_BUILD_TOP/hardware/porting
