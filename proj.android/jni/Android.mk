LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := gamedemo_shared

LOCAL_MODULE_FILENAME := libgamedemo

LOCAL_SRC_FILES := gamedemo/main.cpp \
../../Classes/Action.cpp \
../../Classes/AppDelegate.cpp \
../../Classes/BattleGroundScene.cpp \
../../Classes/BattleScene.cpp \
../../Classes/Box2DEx.cpp \
../../Classes/BulletSprite.cpp \
../../Classes/ChildOfGameUnit.cpp \
../../Classes/ComplexUnit.cpp \
../../Classes/DemoScene.cpp \
../../Classes/DotaScene.cpp \
../../Classes/GameChapterScene.cpp \
../../Classes/GameCtrl.cpp \
../../Classes/GameDisplay.cpp \
../../Classes/GameFile.cpp \
../../Classes/GameLevel.cpp \
../../Classes/GameLogic.cpp \
../../Classes/GLES-Render.cpp \
../../Classes/HomeScene.cpp \
../../Classes/MainScene.cpp \
../../Classes/MissionInfo.cpp \
../../Classes/Package.cpp \
../../Classes/PlayerInfo.cpp \
../../Classes/Prop.cpp \
../../Classes/PropUI.cpp \
../../Classes/Skill.cpp \
../../Classes/SkillInfo.cpp \
../../Classes/SkillUI.cpp \
../../Classes/SomeScene.cpp \
../../Classes/TankSprite.cpp \
../../Classes/TechTreeScene.cpp \
../../Classes/TestScene.cpp \
../../Classes/Tower.cpp \
../../Classes/Unit.cpp \
../../Classes/UnitInfo.cpp \
../../Classes/UnitShowScene.cpp \
../../Classes/WHomeScene.cpp
                   
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../cocos2dx/include \
                    $(LOCAL_PATH)/../../../cocos2dx \
                    $(LOCAL_PATH)/../../../cocos2dx/platform/android \
                    $(LOCAL_PATH)/../../../cocos2dx/kazmath/include \
                    $(LOCAL_PATH)/../../../external \
                    $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static cocos_extension_static box2d_static
            
include $(BUILD_SHARED_LIBRARY)

$(call import-module,CocosDenshion/android) \
$(call import-module,cocos2dx) \
$(call import-module,extensions) \
$(call import-module,external/Box2D)
