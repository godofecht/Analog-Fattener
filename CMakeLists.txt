cmake_minimum_required(VERSION 3.15)
project(AnalogFattener VERSION 1.0.0)

# Set JUCE options before adding the subdirectory
set(JUCE_GUI_BASICS_INCLUDE_X11 1 CACHE BOOL "Force include of X11 headers")

# Add JUCE
add_subdirectory(JUCE)

# Create the plugin target
juce_add_plugin(AnalogFattener
    COMPANY_NAME "MyCompany"
    PLUGIN_NAME "AnalogFattener"
    PLUGIN_DESC "An analog fattener plugin"
    PLUGIN_MANUFACTURER_CODE "MyCo"
    PLUGIN_CODE "AbCd"
    FORMATS VST3 AU Standalone
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    INSTALL_VST3 TRUE
    INSTALL_AU TRUE
)

# The plugin sources include <JuceHeader.h>, which this generates.
juce_generate_juce_header(AnalogFattener)

target_sources(AnalogFattener PRIVATE
    Source/PluginEditor.cpp
    Source/PluginProcessor.cpp
)

target_compile_definitions(AnalogFattener PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

# Link all required libraries
target_link_libraries(AnalogFattener PRIVATE
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_plugin_client
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_cryptography
    juce::juce_data_structures
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
    juce::juce_opengl
    PUBLIC
    juce::juce_recommended_config_flags
)
