#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>
#include <utility>
#include "mainComponent.h"

class App : public juce::JUCEApplication {
public:
    App() = default;
    const juce::String getApplicationName() override { return "JamTown"; }
    const juce::String getApplicationVersion() override { return "0.0.0"; }

    void initialise(const juce::String&) override {
        mainWindow.reset(
            new MainWindow("JamTown", std::make_unique<MainComponent>(), *this));
        setInitialAppSizeAndPosition();
    }

    void shutdown() override { mainWindow = nullptr; }

private:
    void setInitialAppSizeAndPosition() {
        auto display = juce::Desktop::getInstance().getDisplays().getPrimaryDisplay();
        if (display == nullptr)
            return;
        auto screenWidth = display->totalArea.getWidth();
        auto screenHeight = display->totalArea.getHeight();
        mainWindow->setSize(static_cast<int>(screenWidth * 0.66),
                            static_cast<int>(screenHeight * 0.66));
        mainWindow->setCentrePosition(static_cast<int>(screenWidth / 2),
                                      static_cast<int>(screenHeight / 2));
    }
    class MainWindow : public juce::DocumentWindow {
    public:
        MainWindow(const juce::String& name,
                   std::unique_ptr<juce::Component> c,
                   JUCEApplication& a)
            : DocumentWindow(
                  name,
                  juce::Desktop::getInstance().getDefaultLookAndFeel().findColour(
                      ResizableWindow::backgroundColourId),
                  juce::DocumentWindow::allButtons),
              app(a) {
            setUsingNativeTitleBar(true);

#if JUCE_ANDROID || JUCE_IOS
            setContentOwned(new SafeAreaComponent{std::move(c)}, true);
            setFullScreen(true);
#else
            setContentOwned(c.release(), true);
            setResizable(true, false);
            setResizeLimits(300, 250, 10000, 10000);
            centreWithSize(getWidth(), getHeight());
#endif

            setVisible(true);
        }

        void closeButtonPressed() override { app.systemRequestedQuit(); }

#if JUCE_ANDROID || JUCE_IOS
        class SafeAreaComponent : public juce::Component {
        public:
            explicit SafeAreaComponent(std::unique_ptr<Component> c)
                : content(std::move(c)) {
                addAndMakeVisible(*content);
            }

            void resized() override {
                if (const auto* d =
                        Desktop::getInstance().getDisplays().getDisplayForRect(
                            getLocalBounds()))
                    content->setBounds(
                        d->safeAreaInsets.subtractedFrom(getLocalBounds()));
            }

        private:
            std::unique_ptr<Component> content;
        };

        void parentSizeChanged() override {
            if (auto* c = getContentComponent())
                c->resized();
        }
#endif

    private:
        JUCEApplication& app;

        //==============================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
    };

    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
START_JUCE_APPLICATION(App)
