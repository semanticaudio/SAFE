# Automatically generated makefile, created by the Introjucer
# Don't edit this file! Your changes will be overwritten when you re-save the Introjucer project!

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef CONFIG
  CONFIG=Debug
endif

ifeq ($(CONFIG),Debug)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Debug
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D IMAGE_DIRECTORY=\"/home/sean/Documents/smmp/Juce_Module_Stuff/SAFE_juce_module/LookAndFeel\" -D "JucePlugin_Build_LV2" -D JucePlugin_LV2URI=\"https://github.com/semanticaudio/SAFE/tree/master/SAFECompressor\" -D JucePlugin_LV2Category=\"Compressor\" -D "JucePlugin_WantsLV2TimePos=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -D "JUCE_APP_VERSION=1.3" -D "JUCE_APP_VERSION_HEX=0x10300" -I /usr/include -I /usr/include/freetype2 -I ~/SDKs/vstsdk2.4 -I ../../JuceLibraryCode -I ../../../JuceModules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -fPIC -O0
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -shared -L/usr/X11R6/lib/ -lGL -lX11 -lXext -lXinerama -ldl -lfreetype -lpthread -lrt -lcurl /usr/local/lib/libxtract.a
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D IMAGE_DIRECTORY=\"/home/sean/Documents/smmp/Juce_Module_Stuff/SAFE_juce_module/LookAndFeel\" -D "JucePlugin_Build_LV2" -D JucePlugin_LV2URI=\"https://github.com/semanticaudio/SAFE/tree/master/SAFECompressor\" -D JucePlugin_LV2Category=\"Compressor\" -D "JucePlugin_WantsLV2TimePos=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -D "JUCE_APP_VERSION=1.3" -D "JUCE_APP_VERSION_HEX=0x10300" -I /usr/include -I /usr/include/freetype2 -I ~/SDKs/vstsdk2.4 -I ../../JuceLibraryCode -I ../../../JuceModules
  TARGET := SAFECompressor.so
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

ifeq ($(CONFIG),Release)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Release
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -D IMAGE_DIRECTORY=\"/home/sean/Documents/smmp/Juce_Module_Stuff/SAFE_juce_module/LookAndFeel\" -D "JucePlugin_Build_LV2" -D JucePlugin_LV2URI=\"https://github.com/semanticaudio/SAFE/tree/master/SAFECompressor\" -D JucePlugin_LV2Category=\"Compressor\" -D "JucePlugin_WantsLV2TimePos=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -D "JUCE_APP_VERSION=1.3" -D "JUCE_APP_VERSION_HEX=0x10300" -I /usr/include -I /usr/include/freetype2 -I ~/SDKs/vstsdk2.4 -I ../../JuceLibraryCode -I ../../../JuceModules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -fPIC -O3
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -shared -fvisibility=hidden -L/usr/X11R6/lib/ -lGL -lX11 -lXext -lXinerama -ldl -lfreetype -lpthread -lrt -lcurl /usr/local/lib/libxtract.a
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "NDEBUG=1" -D IMAGE_DIRECTORY=\"/home/sean/Documents/smmp/Juce_Module_Stuff/SAFE_juce_module/LookAndFeel\" -D "JucePlugin_Build_LV2" -D JucePlugin_LV2URI=\"https://github.com/semanticaudio/SAFE/tree/master/SAFECompressor\" -D JucePlugin_LV2Category=\"Compressor\" -D "JucePlugin_WantsLV2TimePos=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -D "JUCE_APP_VERSION=1.3" -D "JUCE_APP_VERSION_HEX=0x10300" -I /usr/include -I /usr/include/freetype2 -I ~/SDKs/vstsdk2.4 -I ../../JuceLibraryCode -I ../../../JuceModules
  TARGET := SAFECompressor.so
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

ifeq ($(CONFIG),Release64)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Release64
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -m64
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -D IMAGE_DIRECTORY=\"/home/sean/Documents/smmp/Juce_Module_Stuff/SAFE_juce_module/LookAndFeel\" -D "JucePlugin_Build_LV2" -D JucePlugin_LV2URI=\"https://github.com/semanticaudio/SAFE/tree/master/SAFECompressor\" -D JucePlugin_LV2Category=\"Compressor\" -D "JucePlugin_WantsLV2TimePos=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -D "JUCE_APP_VERSION=1.3" -D "JUCE_APP_VERSION_HEX=0x10300" -I /usr/include -I /usr/include/freetype2 -I ~/SDKs/vstsdk2.4 -I ../../JuceLibraryCode -I ../../../JuceModules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -fPIC -O3
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -shared -fvisibility=hidden -L/usr/X11R6/lib/ -lGL -lX11 -lXext -lXinerama -ldl -lfreetype -lpthread -lrt -lcurl /usr/local/lib/libxtract.a
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "NDEBUG=1" -D IMAGE_DIRECTORY=\"/home/sean/Documents/smmp/Juce_Module_Stuff/SAFE_juce_module/LookAndFeel\" -D "JucePlugin_Build_LV2" -D JucePlugin_LV2URI=\"https://github.com/semanticaudio/SAFE/tree/master/SAFECompressor\" -D JucePlugin_LV2Category=\"Compressor\" -D "JucePlugin_WantsLV2TimePos=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -D "JUCE_APP_VERSION=1.3" -D "JUCE_APP_VERSION_HEX=0x10300" -I /usr/include -I /usr/include/freetype2 -I ~/SDKs/vstsdk2.4 -I ../../JuceLibraryCode -I ../../../JuceModules
  TARGET := SAFECompressor.so
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

OBJECTS := \
  $(OBJDIR)/PluginProcessor_a059e380.o \
  $(OBJDIR)/PluginEditor_94d4fb09.o \
  $(OBJDIR)/juce_audio_basics_8162c4a2.o \
  $(OBJDIR)/juce_audio_processors_40a1dc5a.o \
  $(OBJDIR)/juce_core_737cd184.o \
  $(OBJDIR)/juce_cryptography_82e7c7de.o \
  $(OBJDIR)/juce_data_structures_1ace0a40.o \
  $(OBJDIR)/juce_events_fb82dbf8.o \
  $(OBJDIR)/juce_graphics_55e8ff5c.o \
  $(OBJDIR)/juce_gui_basics_2de558d8.o \
  $(OBJDIR)/juce_gui_extra_a2f7e060.o \
  $(OBJDIR)/juce_opengl_bbcb0424.o \
  $(OBJDIR)/juce_PluginUtilities_13ddfbec.o \
  $(OBJDIR)/juce_VST3_Wrapper_af5dd2f3.o \
  $(OBJDIR)/juce_VST_Wrapper_b9d882f5.o \
  $(OBJDIR)/SAFE_juce_module_d2adbf60.o \

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(LDDEPS) $(RESOURCES)
	@echo Linking SAFECompressor
	-@mkdir -p $(BINDIR)
	-@mkdir -p $(LIBDIR)
	-@mkdir -p $(OUTDIR)
	@$(BLDCMD)

clean:
	@echo Cleaning SAFECompressor
	@$(CLEANCMD)

strip:
	@echo Stripping SAFECompressor
	-@strip --strip-unneeded $(OUTDIR)/$(TARGET)

$(OBJDIR)/PluginProcessor_a059e380.o: ../../Source/PluginProcessor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling PluginProcessor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/PluginEditor_94d4fb09.o: ../../Source/PluginEditor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling PluginEditor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_basics_8162c4a2.o: ../../../JuceModules/juce_audio_basics/juce_audio_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_processors_40a1dc5a.o: ../../../JuceModules/juce_audio_processors/juce_audio_processors.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_processors.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_core_737cd184.o: ../../../JuceModules/juce_core/juce_core.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_cryptography_82e7c7de.o: ../../../JuceModules/juce_cryptography/juce_cryptography.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_cryptography.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_data_structures_1ace0a40.o: ../../../JuceModules/juce_data_structures/juce_data_structures.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_events_fb82dbf8.o: ../../../JuceModules/juce_events/juce_events.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_graphics_55e8ff5c.o: ../../../JuceModules/juce_graphics/juce_graphics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_basics_2de558d8.o: ../../../JuceModules/juce_gui_basics/juce_gui_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_extra_a2f7e060.o: ../../../JuceModules/juce_gui_extra/juce_gui_extra.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_opengl_bbcb0424.o: ../../../JuceModules/juce_opengl/juce_opengl.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_opengl.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_PluginUtilities_13ddfbec.o: ../../../JuceModules/juce_audio_plugin_client/utility/juce_PluginUtilities.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_PluginUtilities.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_VST3_Wrapper_af5dd2f3.o: ../../../JuceModules/juce_audio_plugin_client/VST3/juce_VST3_Wrapper.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_VST3_Wrapper.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_VST_Wrapper_b9d882f5.o: ../../../JuceModules/juce_audio_plugin_client/VST/juce_VST_Wrapper.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_VST_Wrapper.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SAFE_juce_module_d2adbf60.o: ../../../JuceModules/SAFE_juce_module/SAFE_juce_module.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SAFE_juce_module.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
