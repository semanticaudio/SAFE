#ifndef SAFE_JUCE_MODULE
#define SAFE_JUCE_MODULE

#include "../juce_core/juce_core.h"
#include "../juce_audio_processors/juce_audio_processors.h"
#include "../juce_cryptography/juce_cryptography.h"

#include "xtract/libxtract.h"

#include <complex>

#if JUCE_LINUX
    #include <curl/curl.h>
#endif

//=============================================================================
namespace juce
{
#include "LookAndFeel/SAFEImages.h"
#include "LookAndFeel/SAFEColours.h"
#include "LookAndFeel/SAFELookAndFeel.h"

#include "UIComponents/SAFEButton.h"
#include "UIComponents/SAFESlider.h"
#include "UIComponents/SAFEMetaDataScreen.h"
#include "UIComponents/SAFEDescriptorLoadScreen.h"
#include "UIComponents/LevelMeterTimer.h"
#include "UIComponents/LevelMeter.h"
#include "UIComponents/FunctionGraph.h"
#include "UIComponents/DynamicProcessorGraph.h"
#include "UIComponents/FilterInfo.h"
#include "UIComponents/FilterGraph.h"

#include "PluginUtils/SAFEWarnings.h"
#include "PluginUtils/LibXtractHolder.h"

#if JUCE_LINUX
    #include "PluginUtils/CurlHolder.h"
#endif

#include "PluginUtils/SAFEFeatureExtractor.h"
#include "PluginUtils/SAFEParameter.h"
#include "PluginUtils/SAFEAudioProcessor.h"
#include "PluginUtils/SAFEAudioProcessorEditor.h"

#include "Filters/BrechtsIIRFilter.h"
#include "Filters/Resampler.h"
#include "Filters/AllPassFilter.h"
#include "Filters/QuadratureFilter.h"

#include "Effects/MVerb.h"
}

#endif   // SAFE_JUCE_MODULE
