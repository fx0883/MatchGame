LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libgame

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/Shared/rapidjson
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/Shared/rapidjson/internal 

LOCAL_SRC_FILES := main.cpp \
					../../Classes/AppDelegate.cpp \
					../../Classes/PlayMenu.cpp \
					../../Classes/FallingGem.cpp \
					../../Classes/GameLayer.cpp \
					../../Classes/FXLightning.cpp \
					../../Classes/GameState.cpp \
					../../Classes/GameUtils.cpp \
					../../Classes/GemSprite.cpp \
					../../Classes/ParseUtil.cpp \
					../../Classes/PauseLayer.cpp \
					../../Classes/SocialAlertView.cpp \
					../../Classes/EndGameLayer.cpp \
					../../Classes/perlin.cpp
				

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \


LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocos_extension_static cocosdenshion_static box2d_static curl_static_prebuilt
            
include $(BUILD_SHARED_LIBRARY)

$(call import-module,CocosDenshion/android) 
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,cocos2dx) 
$(call import-module,extensions)
