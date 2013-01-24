#-------------------------------------------------
#
# Project created by QtCreator 2012-11-25T23:57:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GameDemoEditor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        Cocos2dxThread.cpp \
        ../Classes/Action.cpp \
        ../Classes/AppDelegate.cpp \
        ../Classes/BattleGroundScene.cpp \
        ../Classes/BattleScene.cpp \
        ../Classes/Box2DEx.cpp \
        ../Classes/BulletSprite.cpp \
        ../Classes/ChildOfGameUnit.cpp \
        ../Classes/DemoScene.cpp \
        ../Classes/DotaScene.cpp \
        ../Classes/GameChapterScene.cpp \
        ../Classes/GameCtrl.cpp \
        ../Classes/GameDisplay.cpp \
        ../Classes/GameFile.cpp \
        ../Classes/GameLevel.cpp \
        ../Classes/GameLogic.cpp \
        ../Classes/GameResControl.cpp \
        ../Classes/GLES-Render.cpp \
        ../Classes/HomeScene.cpp \
        ../Classes/MainScene.cpp \
        ../Classes/MissionInfo.cpp \
        ../Classes/Package.cpp \
        ../Classes/PlayerInfo.cpp \
        ../Classes/Prop.cpp \
        ../Classes/PropUI.cpp \
        ../Classes/Skill.cpp \
        ../Classes/SkillInfo.cpp \
        ../Classes/SkillUI.cpp \
        ../Classes/SomeScene.cpp \
        ../Classes/TankSprite.cpp \
        ../Classes/TechTreeScene.cpp \
        ../Classes/TestScene.cpp \
        ../Classes/Tower.cpp \
        ../Classes/Unit.cpp \
        ../Classes/UnitEditorScene.cpp \
        ../Classes/UnitInfo.cpp \
        ../Classes/UnitShowScene.cpp \
        ../Classes/WHomeScene.cpp \
    FileEditor.cpp \
    UipEditor.cpp \
    UipEditorSkill.cpp

HEADERS += mainwindow.h \
        CommInc.h \
        Cocos2dxThread.h \
        ../Classes/CommInc.h \
        ../Classes/CommDef.h \
        ../Classes/AppMacros.h \
        ../Classes/Action.h \
        ../Classes/AppDelegate.h \
        ../Classes/BattleGroundScene.h \
        ../Classes/BattleScene.h \
        ../Classes/Box2DEx.h \
        ../Classes/BulletSprite.h \
        ../Classes/ChildOfGameUnit.h \
        ../Classes/DemoScene.h \
        ../Classes/DotaScene.h \
        ../Classes/GameChapterScene.h \
        ../Classes/GameCtrl.h \
        ../Classes/GameDisplay.h \
        ../Classes/GameFile.h \
        ../Classes/GameLevel.h \
        ../Classes/GameLogic.h \
        ../Classes/GameResControl.h \
        ../Classes/GLES-Render.h \
        ../Classes/HomeScene.h \
        ../Classes/MainScene.h \
        ../Classes/MissionInfo.h \
        ../Classes/Package.h \
        ../Classes/PlayerInfo.h \
        ../Classes/Prop.h \
        ../Classes/PropUI.h \
        ../Classes/Skill.h \
        ../Classes/SkillInfo.h \
        ../Classes/SkillUI.h \
        ../Classes/SomeScene.h \
        ../Classes/TankSprite.h \
        ../Classes/TechTreeScene.h \
        ../Classes/TestScene.h \
        ../Classes/Tower.h \
        ../Classes/Unit.h \
        ../Classes/UnitEditorScene.h \
        ../Classes/UnitInfo.h \
        ../Classes/UnitShowScene.h \
        ../Classes/WHomeScene.h \
    FileEditor.h \
    UipEditor.h \
    UipEditorSkill.h

FORMS += mainwindow.ui \
    UipEditor.ui \
    UipEditorSkill.ui

RC_FILE += GameDemoEditor.rc

INCLUDEPATH += $$PWD/../../cocos2dx \
    $$PWD/../../cocos2dx/include \
    $$PWD/../../cocos2dx/kazmath/include \
    $$PWD/../../cocos2dx/platform/win32 \
    $$PWD/../../cocos2dx/platform/third_party/win32 \
    $$PWD/../../cocos2dx/platform/third_party/win32/libxml2 \
    $$PWD/../../cocos2dx/platform/third_party/win32/iconv \
    $$PWD/../../cocos2dx/platform/third_party/win32/OGLES \
    $$PWD/../../CocosDenshion/Include \
    $$PWD/../../external \
    $$PWD/../../external/chipmunk/include \
    $$PWD/../../extensions \
    $$PWD/../Classes

COCOS2DX_LIBS = -llibcocos2d -llibBox2d -llibCocosDenshion -llibExtensions -lopengl32 -lglew32 -llibxml2 -llibiconv

win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Debug.win32/ $$COCOS2DX_LIBS
else:win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Release.win32/ $$COCOS2DX_LIBS
else:unix: LIBS += -L$$PWD/../../Debug.win32/ $$COCOS2DX_LIBS

win32:CONFIG(debug, debug|release): DEFINES += QTPROJ WIN32 _DEBUG _WINDOWS COCOS2D_DEBUG=1 _CRT_SECURE_NO_WARNINGS _SCL_SECURE_NO_WARNINGS
else:win32:CONFIG(release, debug|release): DEFINES += QTPROJ WIN32 NDEBUG _WINDOWS _CRT_SECURE_NO_WARNINGS _SCL_SECURE_NO_WARNINGS

CONFIG += warn_off

RESOURCES += \
    GameDemoEditor.qrc

OTHER_FILES += \
    GameDemoEditor.rc

