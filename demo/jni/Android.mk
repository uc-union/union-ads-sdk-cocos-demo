LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

COCOS2D_ROOT := $(LOCAL_PATH)/../../../cocos2d

$(call import-add-path,$(COCOS2D_ROOT))
$(call import-add-path,$(COCOS2D_ROOT)/external)
$(call import-add-path,$(COCOS2D_ROOT)/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES += src/main.cpp \
                   src/AppDelegate.cpp \
                   src/DemoScene.cpp \
                   src/ImageFetcher.cpp \

########################  sdk config begin  #################
SDK_SRC_CPP_ROOT := $(LOCAL_PATH)/../../sdk/src-cpp

LOCAL_C_INCLUDES += $(SDK_SRC_CPP_ROOT)/public \
                    $(SDK_SRC_CPP_ROOT)/

LOCAL_SRC_FILES +=  $(SDK_SRC_CPP_ROOT)/impl/AdListenerProxy.cpp \
                    $(SDK_SRC_CPP_ROOT)/impl/AdRequest.cpp \
                    $(SDK_SRC_CPP_ROOT)/impl/AdSDK.cpp \
                    $(SDK_SRC_CPP_ROOT)/impl/Banner.cpp \
                    $(SDK_SRC_CPP_ROOT)/impl/Interstitial.cpp \
                    $(SDK_SRC_CPP_ROOT)/impl/NativeAd.cpp \
                    $(SDK_SRC_CPP_ROOT)/impl/JNICommon.cpp \
                    $(SDK_SRC_CPP_ROOT)/impl/Util.cpp \
#------------------------------------------------------------

LOCAL_STATIC_LIBRARIES := cocos2dx_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
