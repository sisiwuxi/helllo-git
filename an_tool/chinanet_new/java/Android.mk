#<M Software>

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := samples  
LOCAL_MODULE_TAGS := optional

LOCAL_CERTIFICATE := platform

LOCAL_SRC_FILES := $(call all-java-files-under, src)

LOCAL_PACKAGE_NAME :=  FA_Certification

LOCAL_JNI_SHARED_LIBRARIES := libMctit_jni

LOCAL_JAVA_LIBRARIES := \
         com.M.android

LOCAL_PROGUARD_ENABLED:= disabled

LOCAL_MULTILIB := 32

include $(BUILD_PACKAGE)
include $(call all-makefiles-under,$(LOCAL_PATH))
