EXECUTABLE = SimpleGame

INCLUDES = -I.. -I../Classes -I../Vendors/avalon

SOURCES = main.cpp \
        ../Classes/Alert.cpp \
        ../Classes/AppDelegate.cpp \
        ../Classes/buttons/AnswerButton.cpp \
        ../Classes/buttons/BackButton.cpp \
        ../Classes/buttons/HintButton.cpp \
        ../Classes/buttons/LevelButton.cpp \
        ../Classes/buttons/ToggleButton.cpp \
        ../Classes/GameScene.cpp \
        ../Classes/GameTimer.cpp \
        ../Classes/PageManager.cpp \
        ../Classes/pages/AboutPage.cpp \
        ../Classes/pages/CategoryPage.cpp \
        ../Classes/pages/GamePage.cpp \
        ../Classes/pages/LockedCategoryPage.cpp \
        ../Classes/pages/MainPage.cpp \
        ../Classes/pages/MoreGamesPage.cpp \
        ../Classes/pages/Page.cpp \
        ../Classes/pages/SettingsPage.cpp \
        ../Classes/utils/android_fixes.cpp \
        ../Classes/utils/color.cpp \
        ../Classes/utils/config.cpp \
        ../Classes/utils/fonts.cpp \
        ../Classes/utils/helper.cpp \
        ../Classes/utils/MyFlurry.cpp \
        ../Classes/utils/QuestionString.cpp \
        ../Classes/utils/user.cpp \
        ../Vendors/avalon/avalon/i18n/Language.cpp \
        ../Vendors/avalon/avalon/i18n/LanguageKey.cpp \
        ../Vendors/avalon/avalon/i18n/Localization.cpp \
        ../Vendors/avalon/avalon/io/IniReader.cpp

RESOURCE_PATH = ../Resources

RESOURCES = HelveticaNeueLight.ttf \
            HelveticaNeueMedium.ttf \
            ads.ini \
            api_key.txt \
            assets.plist \
            assets.png \
            background.mp3 \
            click.mp3 \
            de.ini \
            en.ini \
            icon-down.png \
            icon-up.png \
            lost.mp3 \
            payment.ini \
            solved.mp3 \
            wrong.mp3

ALL_ASSETS := $(RESOURCES)

COCOS_ROOT = ../../..
include $(COCOS_ROOT)/cocos2dx/proj.emscripten/cocos2dx.mk

SHAREDLIBS += -lcocos2d -lcocosdenshion -lextension
COCOS_LIBS = $(LIB_DIR)/libcocos2d.so $(LIB_DIR)/libcocosdenshion.so
INCLUDES += -I$(COCOS_ROOT)/CocosDenshion/include -I$(COCOS_ROOT)/extensions

$(TARGET).js: $(OBJECTS) $(STATICLIBS) $(COCOS_LIBS) $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(SHAREDLIBS) $(STATICLIBS)

ifeq ($(shell uname -s),Darwin)
ARIEL_TTF := /Library/Fonts/Arial.ttf
else
ARIEL_TTF := $(COCOS_ROOT)/samples/Cpp/TestCpp/Resources/fonts/arial.ttf
endif

$(TARGET).data:
	@mkdir -p $(@D)
	$(eval RESTMP := $(shell mktemp -d /tmp/cocos-emscripten.XXXXXX))
	# Always need a font path, so ensure that it is created first.
	mkdir -p $(RESTMP)/fonts
	(cd $(RESOURCE_PATH) && tar cf - $(RESOURCES)) | (cd $(RESTMP) && tar xvf -)
	# NOTE: we copy the system arial.ttf so that there is always a fallback.
	cp $(ARIEL_TTF) $(RESTMP)/fonts/arial.ttf
	(cd $(RESTMP); python $(PACKAGER) $(EXECUTABLE).data $(patsubst %,--preload %,$(ALL_ASSETS)) --preload fonts --pre-run > $(EXECUTABLE).data.js)
	mv $(RESTMP)/$(EXECUTABLE).data $@
	mv $(RESTMP)/$(EXECUTABLE).data.js $@.js
	rm -rf $(RESTMP)

$(BIN_DIR)/$(HTMLTPL_FILE): $(HTMLTPL_DIR)/$(HTMLTPL_FILE)
	@mkdir -p $(@D)
	@cp -Rf $(HTMLTPL_DIR)/* $(BIN_DIR)
	@sed -i -e "s/JS_APPLICATION/$(EXECUTABLE)/g" $(BIN_DIR)/$(HTMLTPL_FILE)

$(OBJ_DIR)/%.o: %.cpp $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@

$(OBJ_DIR)/%.o: ../%.cpp $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@
