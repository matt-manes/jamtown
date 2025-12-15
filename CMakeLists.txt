cmake_minimum_required(VERSION 3.20)
project(jamtown)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)
find_package(JUCE CONFIG REQUIRED)
juce_add_gui_app(jamtown
    VERSION 0.0.0)

target_sources(jamtown
    PRIVATE
    src/app.cpp
    src/transport.cpp
    src/transportComponent.cpp
    src/trackInfo.cpp)

# add_subdirectory(images)
target_link_libraries(jamtown
    PRIVATE

    # BinaryData
    juce::juce_gui_basics
    juce::juce_audio_utils
    juce::juce_events
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)

target_compile_definitions(jamtown PRIVATE -D_SILENCE_CXX23_ALIGNED_STORAGE_DEPRECATION_WARNING=1)