#pragma once

#include "PluginProcessor.h"

class MyKnob : public juce::Slider
{
public:
    MyKnob() : juce::Slider(
                   juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                   juce::Slider::TextEntryBoxPosition::NoTextBox)
    {
        this->setPopupDisplayEnabled(true, false, nullptr);
    }
};

class MyComboBox : public juce::ComboBox
{
public:
    MyComboBox(const juce::StringArray &list) : juce::ComboBox()
    {
        this->addItemList(list, 1);
    }
};

class MyToggleButton : public juce::ToggleButton
{
public:
    MyToggleButton() : juce::ToggleButton()
    {
        // this->setClickingTogglesState(true);
    }
};

class MySlider : public juce::Slider
{
public:
    MySlider() : juce::Slider(
                   juce::Slider::SliderStyle::LinearHorizontal,
                   juce::Slider::TextEntryBoxPosition::NoTextBox)
    {

    }
};

//==============================================================================
class CabImpulseAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit CabImpulseAudioProcessorEditor(CabImpulseAudioProcessor &);
    ~CabImpulseAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    CabImpulseAudioProcessor &processorRef;

    MyKnob lowCutKnob, highCutKnob, gainKnob;

    MySlider micDistanceSlider;

    MyComboBox cabTypeComboBox = MyComboBox(CabList);
    MyComboBox micTypeComboBox = MyComboBox(MicList);

    MyToggleButton micAxisSwitch;

    using KnobAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    KnobAttachment lowCutKnobAttachment,
        highCutKnobAttachment,
        gainKnobAttachment, micDistanceSliderAttachment;

    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

    ComboBoxAttachment cabTypeComboBoxAttachment, micTypeComboBoxAttachment;

    juce::AudioProcessorValueTreeState::ButtonAttachment micAxisSwitchAttachment;

    std::vector<juce::Component *> getComps();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CabImpulseAudioProcessorEditor)
};
