#if defined (SAFE_JUCE_MODULE) && ! JUCE_AMALGAMATED_INCLUDE
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
 #error "Incorrect use of JUCE cpp file"
#endif

// Your project must contain an AppConfig.h file with your project-specific settings in it,
// and your header search path must make it accessible to the module's files.
#include "AppConfig.h"
#include "SAFE_juce_module.h"

#define ANALYSIS_FRAME_LENGTH 4096

namespace juce
{
#include "LookAndFeel/SAFEImages.cpp"
#include "LookAndFeel/SAFEColours.cpp"
#include "LookAndFeel/SAFELookAndFeel.cpp"

#include "UIComponents/SAFEButton.cpp"
#include "UIComponents/SAFESlider.cpp"
#include "UIComponents/SAFEMetaDataScreen.cpp"
#include "UIComponents/SAFEDescriptorLoadScreen.cpp"
#include "UIComponents/LevelMeterTimer.cpp"
#include "UIComponents/LevelMeter.cpp"
#include "UIComponents/FunctionGraph.cpp"
#include "UIComponents/DynamicProcessorGraph.cpp"
#include "UIComponents/FilterInfo.cpp"
#include "UIComponents/FilterGraph.cpp"

#include "PluginUtils/LibXtractHolder.cpp"
#include "PluginUtils/SAFEFeatureExtractor.cpp"
#include "PluginUtils/SAFEParameter.cpp"
#include "PluginUtils/SAFEAudioProcessor.cpp"
#include "PluginUtils/SAFEAudioProcessorEditor.cpp"

#include "Filters/BrechtsIIRFilter.cpp"
#include "Filters/Resampler.cpp"
#include "Filters/AllPassFilter.cpp"
#include "Filters/QuadratureFilter.cpp"
}
