#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CabImpulseAudioProcessorEditor::CabImpulseAudioProcessorEditor (CabImpulseAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p),
    lowCutKnobAttachment {p.apvts, "Low Cut", lowCutKnob},
    highCutKnobAttachment {p.apvts, "High Cut", highCutKnob},
    gainKnobAttachment {p.apvts, "Output Level", gainKnob},
    cabTypeComboBoxAttachment {p.apvts, "Cab Type", cabTypeComboBox},
    micTypeComboBoxAttachment {p.apvts, "Mic Type", micTypeComboBox},
    micDistanceSliderAttachment {p.apvts, "Mic Distance", micDistanceSlider},
    micAxisSwitchAttachment {p.apvts, "Mic Axis", micAxisSwitch}
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    for( auto* comp : getComps() )
    {
        addAndMakeVisible(comp);
    }

    setSize (800, 600);
}

CabImpulseAudioProcessorEditor::~CabImpulseAudioProcessorEditor()
{
}

//==============================================================================
void CabImpulseAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
}

void CabImpulseAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto bounds = getLocalBounds();
    bounds.removeFromTop(bounds.getHeight() / 4);

    auto sliderBounds = bounds.removeFromTop(bounds.getHeight() / 3);

    lowCutKnob.setBounds(sliderBounds.removeFromLeft(sliderBounds.getWidth() / 3));
    highCutKnob.setBounds(sliderBounds.removeFromLeft(sliderBounds.getWidth() / 2));
    gainKnob.setBounds(sliderBounds);

    auto comboBoxBounds = bounds.removeFromTop(bounds.getHeight() / 2);

    cabTypeComboBox.setBounds(comboBoxBounds.removeFromLeft(comboBoxBounds.getWidth() / 2));
    micTypeComboBox.setBounds(comboBoxBounds);

    auto slider2Bounds = bounds.removeFromTop(bounds.getHeight() / 2);

    micDistanceSlider.setBounds(slider2Bounds.removeFromLeft(slider2Bounds.getWidth() / 2));

    micAxisSwitch.setBounds(slider2Bounds);
    

}

std::vector<juce::Component*> CabImpulseAudioProcessorEditor::getComps()
{
    return { &lowCutKnob,
     &highCutKnob,
     &gainKnob ,
     &cabTypeComboBox,
     &micTypeComboBox,
     &micDistanceSlider,
     &micAxisSwitch};
}