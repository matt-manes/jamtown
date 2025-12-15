#pragma once

#include "transportComponent.h"
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_events/juce_events.h>
#include <memory>

class MainContentComponent : public juce::Component {
 public:
    MainContentComponent() {
        addAndMakeVisible(transport);
        transport.setGain(0.25f);
        setSize(666, 666);
    }

    void paint(juce::Graphics&) override {}

    void resized() override { transport.setBounds(0, 0, getWidth(), getHeight()); }

 private:
    TransportComponent transport;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};
