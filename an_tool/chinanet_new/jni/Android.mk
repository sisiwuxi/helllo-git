LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := samples

LOCAL_MODULE := libMctit_jni

LOCAL_CERTIFICATE := platform

JNI_SRC_PATH := jni
DTV_SRC_PATH := src

LOCAL_MULTILIB := 32
LOCAL_C_INCLUDES := $(JNI_SRC_PATH)/\
                    $(LOCAL_PATH)/../inc\
                    $(TOP)/bionic\
		            $(TOP)/external/stlport/stlport \
		            $(TOP)/external/sqlite/dist/ \

LOCAL_SRC_FILES := com_CTC_ChinaNet_android_tm_Native.cpp \
                  
LOCAL_SHARED_LIBRARIES := libnativehelper \
						  libbinder \
						  libutils \
						  liblog \
						  libcutils \
						  libhardware \
						  librmclient \
						  libz \
						  libstlport \
						  libandroid_runtime \
                          libutopia \
						  libMctitotp \
						  

                                

base := $(LOCAL_PATH)/../..

LOCAL_PRELINK_MODULE := false  

include $(BUILD_SHARED_LIBRARY)

