#pragma once

#include "juce_events/juce_events.h"
#include "juce_core/juce_core.h"
#include "actionMessages.h"
#include "browserComponent.h"

class ChangeBrowserViewListener : public juce::ActionListener {
public:
    ChangeBrowserViewListener(BrowserComponent& browserComponent)
        : browserComponent(browserComponent) {}

    void actionListenerCallback(const juce::String& message) override {
        if (message == ActionMessages::viewLibrary)
            browserComponent.setView(View::LIBRARY);
        else if (message == ActionMessages::viewPlayQueue)
            browserComponent.setView(View::PLAYQUEUE);
    }

private:
    BrowserComponent& browserComponent;
};
