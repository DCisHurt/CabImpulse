#pragma once

#include "PluginProcessor.h"
#include "MyToggleSwitch.h"

class MyKnob : public juce::Slider
{
public:
    MyKnob(juce::String suffix) : juce::Slider(
                                       juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                                       juce::Slider::TextEntryBoxPosition::NoTextBox)
    {
        this->setPopupDisplayEnabled(true, false, nullptr);
        this->setTextValueSuffix(suffix);
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

    MyKnob lowCutKnob = MyKnob(" Hz");
    MyKnob highCutKnob = MyKnob(" Hz");
    MyKnob gainKnob = MyKnob(" dB");
    MyKnob micDistanceKnob = MyKnob(" cm");

    MyComboBox cabTypeComboBox = MyComboBox(CabList);
    MyComboBox micTypeComboBox = MyComboBox(MicList);

    MyToggleSwitch micAxisSwitch = MyToggleSwitch("Microphone Axis", false, false);

    const juce::String title = "Cab Impulse";

    juce::Label MyLabel = juce::Label(title, title);

    using KnobAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    KnobAttachment lowCutKnobAttachment,
        highCutKnobAttachment,
        gainKnobAttachment,
        micDistanceKnobAttachment;

    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

    ComboBoxAttachment cabTypeComboBoxAttachment, micTypeComboBoxAttachment;

    juce::AudioProcessorValueTreeState::ButtonAttachment micAxisSwitchAttachment;

    std::vector<juce::Component *> getComps();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CabImpulseAudioProcessorEditor)
};
