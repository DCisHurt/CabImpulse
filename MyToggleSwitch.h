#pragma once

#include <JuceHeader.h>

class MyToggleSwitch : public juce::Button
{
public:
    enum ColourIds
    {
        switchColour = 0x101000,
        switchOnBackgroundColour,
        switchOffBackgroundColour
    };

    MyToggleSwitch(juce::String name, bool isInverted, bool isVertical = false)
        : Button("ToggleSwitch"), isInverted(isInverted), isVertical(isVertical)
    {
        title = name;
        onText = "On";
        offText = "Off";

        auto &def = juce::LookAndFeel::getDefaultLookAndFeel();

        def.setColour(switchColour, juce::Colours::white);
        def.setColour(switchOffBackgroundColour, juce::Colours::darkgrey);
        def.setColour(switchOnBackgroundColour, juce::Colours::limegreen);

        setClickingTogglesState(true);
        addAndMakeVisible(switchCircle);
        switchCircle.setWantsKeyboardFocus(false);
        switchCircle.setInterceptsMouseClicks(false, false);
    }

    void setMillisecondsToSpendMoving(int newValue)
    {
        millisecondsToSpendMoving = newValue;
    }

    void paintButton(juce::Graphics &g,
                     bool shouldDrawButtonAsHighlighted,
                     bool shouldDrawButtonAsDown) override
    {

        // auto bounds = getLocalBounds().toFloat();
        // auto textBound = bounds.reduced(0, bounds.getHeight() * 0.25);
        // g.drawText(title, textBound.removeFromTop(textBound.getHeight() * 0.33), juce::Justification::centredTop, 1);

        // textBound.removeFromLeft(textBound.getWidth() * 0.3);
        // textBound.removeFromRight(textBound.getWidth() * 3 / 7.0);
        // auto buttomRow = textBound.removeFromBottom(textBound.getHeight() / 2);

        // g.drawText(onText, buttomRow, juce::Justification::bottomRight, 1);
        // g.drawText(offText, buttomRow, juce::Justification::bottomLeft, 1);
        auto b = getSwitchBounds();
        auto textBound = b.expanded(b.getWidth(), b.getHeight()*1.5);
        g.drawText(title, textBound, juce::Justification::centredTop, 1);

        auto buttomRow = textBound.reduced(textBound.getWidth() * 0.3, 0);

        g.drawText(onText, buttomRow, juce::Justification::bottomRight, 1);
        g.drawText(offText, buttomRow, juce::Justification::bottomLeft, 1);

        auto cornerSize = (isVertical ? b.getWidth() : b.getHeight()) * 0.5f;
        g.setColour(juce::Colours::yellow.withAlpha(0.1f));
        g.drawRoundedRectangle(b, cornerSize, 2.0f);
        g.setColour(findColour(getSwitchState() ? switchOnBackgroundColour : switchOffBackgroundColour));
        g.fillRoundedRectangle(b, cornerSize);

        juce::Path switchPath;
        switchPath.addRoundedRectangle(b, cornerSize, cornerSize);
        g.fillPath(switchPath);

        juce::Rectangle<float> switchCircleBounds;
        if (!isVertical)
            switchCircleBounds = {getSwitchState() ? 1.5f + b.getRight() - b.getHeight() : b.getX() - 1.5f, b.getY(), b.getHeight(), b.getHeight()};
        else
            switchCircleBounds = {
                b.getX(),
                getSwitchState() ? b.getBottom() - b.getWidth() - 1.5f : b.getY() + 1.5f,
                b.getWidth(),
                b.getWidth()};
        animator.animateComponent(&switchCircle, switchCircleBounds.reduced(1).toNearestInt(), 1.0, millisecondsToSpendMoving, false, 0.5, 0.5);
    }

    void resized() override
    {
        Button::resized();
        auto b = getSwitchBounds();
        juce::Rectangle<float> switchCircleBounds;
        if (!isVertical)
            switchCircleBounds = {getSwitchState() ? b.getRight() - b.getHeight() : b.getX(), b.getY(), b.getHeight(), b.getHeight()};
        else
            switchCircleBounds = {
                b.getX(),
                getSwitchState() ? b.getBottom() - b.getWidth() : b.getY(),
                b.getHeight(),
                b.getHeight()};
        switchCircle.setBounds(switchCircleBounds.reduced(1).toNearestInt());
    }

private:
    int millisecondsToSpendMoving{50};

    bool getSwitchState() const
    {
        return isInverted ? !getToggleState() : getToggleState();
    }
    bool isInverted = false;
    bool isVertical = false;

    juce::Rectangle<float> getSwitchBounds()
    {
        auto bounds = getLocalBounds().toFloat();

        auto b = bounds.reduced(bounds.getWidth() * 0.4, bounds.getHeight() * 0.45);

        return b;
    }

    juce::String title, onText, offText;
    class SwitchCircle : public Component
    {
        void paint(juce::Graphics &g) override
        {
            g.setColour(findColour(switchColour));
            g.fillEllipse(getLocalBounds().toFloat());
        }
    } switchCircle;
    juce::ComponentAnimator animator;
};