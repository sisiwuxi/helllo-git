
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE:= TestRandom

LOCAL_SRC_FILES:= \
    main.cpp

LOCAL_C_INCLUDES := \
    $(call include-path-for, corecg graphics) \
    $(TARGET_UTOPIA_LIBS_DIR)/include

LOCAL_CFLAGS += -DGL_GLEXT_PROTOTYPES -DEGL_EGLEXT_PROTOTYPES -DMSOS_TYPE_LINUX

LOCAL_SHARED_LIBRARIES := \
    libcutils \
    liblog \
    libandroidfw \
    libutils \
    libbinder \
    libutopia

include $(BUILD_EXECUTABLE)
