CACHE_DIR = temp_cache
CMAKE = cmake .. -DCMAKE_BUILD_TYPE=Release

MK_DIR = @cmake -E make_directory
CH_DIR = @cmake -E chdir
ECHO = @cmake -E echo

MK_CACHE = $(MK_DIR) $(CACHE_DIR)
EXEC_BUILD = $(CH_DIR) $(CACHE_DIR)

PROJECT=metagen

PROJECT_PREFIX = project

SUPPORT_MAKES = auto nmake mingw linux vc05 vc08 vc10 cb_mingw cb_nmake cb_linux xcode

none: needcmake
	$(ECHO) "Usage:"
	$(ECHO) "  make MakeType [TARGET=TargetToMake]"
	$(ECHO) "or"
	$(ECHO) "  nmake MakeType"
	$(ECHO) "if MS VC is used"
	$(ECHO) 
	$(ECHO) "Available MakeType"
	$(ECHO) "  $(SUPPORT_MAKES)"
	$(ECHO) 
	$(ECHO) "  nmake        Generate Microsoft VC makefile and then use nmake to build."
	$(ECHO) "  mingw        Generate MinGW makefile and then use mingw32-make to build."
	$(ECHO) "  linux        Generate Linux/Unix makefile and then use GCC make to build."
	$(ECHO) "  vc13         Generate project files for Microsoft VC 2013. No auto build. You need to open the project in VC IDE then build."
	$(ECHO) "  vc12         Generate project files for Microsoft VC 2012. No auto build. You need to open the project in VC IDE then build."
	$(ECHO) "  vc10         Generate project files for Microsoft VC 2010. No auto build. You need to open the project in VC IDE then build."
	$(ECHO) "  vc08         Generate project files for Microsoft VC 2008. No auto build. You need to open the project in VC IDE then build."
	$(ECHO) "  vc05         Generate project files for Microsoft VC 2005. No auto build. You need to open the project in VC IDE then build."
	$(ECHO) "  cb_mingw     Generate project files for Code::Blocks using MinGW g++ compiler. No auto build. You need to open the project in Code::Blocks IDE then build."
	$(ECHO) "  cb_nmake     Generate project files for Code::Blocks using VC compiler. No auto build. You need to open the project in Code::Blocks IDE then build."
	$(ECHO) "  cb_linux     Generate project files for Code::Blocks using Linux/Unix GCC compiler. No auto build. You need to open the project in Code::Blocks IDE then build."
	$(ECHO) "  xcode        Generate project files for Apple Xcode. No auto build. You need to open the project in Xcode IDE then build."
	$(ECHO) "  auto         Auto detect the compiler and make environment and then use make to build. NOT recommend."
	$(ECHO) 
	$(ECHO) "  clean        No build. Remove and clean all generated files, include build and projects files."
	$(ECHO) "  cleanbuild   No build. Remove and clean all build files, but leave projects files."
	$(ECHO) 
	$(ECHO) "Optional TargetToMake"
	$(ECHO) 
	$(ECHO) "  TARGET=lib                   Build the library. This is the default if TARGET is omitted."
	$(ECHO) "  TARGET=tutorials             Build the tutorials."
	$(ECHO) "  TARGET=test                  Build the unit test."
	$(ECHO) "  TARGET=samplereflection      Build the reflection sample application."
	$(ECHO) "  TARGET=sampleserialization   Build the serialization sample application."
	$(ECHO) "  TARGET=samplelua             Build the Lua binding sample application."
	$(ECHO) "  TARGET=sampleopengl          Build the Opengl binding sample application."
	$(ECHO) "  TARGET=samplebox2d           Build the Box2D binding sample application."
	$(ECHO) "  TARGET=samplesfml            Build the SFML binding sample application."
	$(ECHO) "  TARGET=sampleirrlicht        Build the Irrlicht binding sample application."

needcmake:
	$(ECHO)

auto: needcmake
	$(MK_CACHE)_auto
	$(CH_DIR) $(CACHE_DIR)_auto $(CMAKE)
	$(EXEC_BUILD)_auto make $(TARGET)

mingw: needcmake
	$(MK_CACHE)_mingw
	$(CH_DIR) $(CACHE_DIR)_mingw $(CMAKE) -G"MinGW Makefiles"
	$(EXEC_BUILD)_mingw mingw32-make $(TARGET)

nmake: needcmake
	$(ECHO) NOTE: *****************************
	$(ECHO) NOTE: If cmake raises errors, try run this in Visual Studio Command Prompt from the VS package.
	$(ECHO) NOTE: *****************************
	$(MK_CACHE)_nmake
	$(CH_DIR) $(CACHE_DIR)_nmake $(CMAKE) -G"NMake Makefiles"
	$(EXEC_BUILD)_nmake nmake $(TARGET)

linux: needcmake
	$(MK_CACHE)_linux
	$(CH_DIR) $(CACHE_DIR)_linux $(CMAKE) -G"Unix Makefiles"
	$(EXEC_BUILD)_linux make $(TARGET)

vc05: needcmake
	$(MK_DIR) $(PROJECT_PREFIX)_vc05
	$(CH_DIR) $(PROJECT_PREFIX)_vc05 $(CMAKE) -G"Visual Studio 8 2005"
	$(ECHO) Please open the solution $(PROJECT).slnin $(PROJECT_PREFIX)_vc05 in VC IDE.

vc08: needcmake
	$(MK_DIR) $(PROJECT_PREFIX)_vc08
	$(CH_DIR) $(PROJECT_PREFIX)_vc08 $(CMAKE) -G"Visual Studio 9 2008"
	$(ECHO) Please open the solution $(PROJECT).sln in $(PROJECT_PREFIX)_vc08 in VC IDE.

vc10: needcmake
	$(MK_DIR) $(PROJECT_PREFIX)_vc10
	$(CH_DIR) $(PROJECT_PREFIX)_vc10 $(CMAKE) -G"Visual Studio 10"
	$(ECHO) Please open the solution $(PROJECT).sln in $(PROJECT_PREFIX)_vc10 in VC IDE.

vc12: needcmake
	$(MK_DIR) $(PROJECT_PREFIX)_vc12
	$(CH_DIR) $(PROJECT_PREFIX)_vc12 $(CMAKE) -G"Visual Studio 11"
	$(ECHO) Please open the solution $(PROJECT).sln in $(PROJECT_PREFIX)_vc12 in VC IDE.

vc13: needcmake
	$(MK_DIR) $(PROJECT_PREFIX)_vc13
	$(CH_DIR) $(PROJECT_PREFIX)_vc13 $(CMAKE) -G"Visual Studio 12"
	$(ECHO) Please open the solution $(PROJECT).sln in $(PROJECT_PREFIX)_vc13 in VC IDE.

cb_mingw: needcmake
	$(MK_DIR) $(PROJECT_PREFIX)_cb_mingw
	$(CH_DIR) $(PROJECT_PREFIX)_cb_mingw $(CMAKE) -G"CodeBlocks - MinGW Makefiles"
	$(ECHO) Please open the project file $(PROJECT).cbp in $(PROJECT_PREFIX)_cb_mingw in Code::Blocks.

cb_nmake: needcmake
	$(MK_DIR) $(PROJECT_PREFIX)_cb_nmake
	$(CH_DIR) $(PROJECT_PREFIX)_cb_nmake $(CMAKE) -G"CodeBlocks - NMake Makefiles"
	$(ECHO) Please open the project file $(PROJECT).cbp in $(PROJECT_PREFIX)_cb_nmake in Code::Blocks.

cb_linux: needcmake
	$(MK_DIR) $(PROJECT_PREFIX)_cb_linux
	$(CH_DIR) $(PROJECT_PREFIX)_cb_linux $(CMAKE) -G"CodeBlocks - Unix Makefiles"
	$(ECHO) Please open the project file $(PROJECT).cbp in $(PROJECT_PREFIX)_cb_linux in Code::Blocks.

xcode: needcmake
	cmake -DCMAKE_BUILD_TYPE=Release -G"Xcode"
	$(ECHO) Please open the solution $(PROJECT).xcodeproj in Xcode IDE.

cleanbuild: needcmake
	@cmake -E remove_directory bin
	@cmake -E remove_directory $(CACHE_DIR)

clean: needcmake cleanbuild
	@cmake -E remove_directory $(PROJECT_PREFIX)_vc05
	@cmake -E remove_directory $(PROJECT_PREFIX)_vc08
	@cmake -E remove_directory $(PROJECT_PREFIX)_vc10
	@cmake -E remove_directory $(PROJECT_PREFIX)_vc12
	@cmake -E remove_directory $(PROJECT_PREFIX)_vc13
	@cmake -E remove_directory $(PROJECT_PREFIX)_cb_mingw
	@cmake -E remove_directory $(PROJECT_PREFIX)_cb_nmake
	@cmake -E remove_directory $(PROJECT_PREFIX)_cb_linux

.PHONY: clean

