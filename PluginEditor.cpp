#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CabImpulseAudioProcessorEditor::CabImpulseAudioProcessorEditor(CabImpulseAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p),
      lowCutKnobAttachment{p.apvts, "Low Cut", lowCutKnob},
      highCutKnobAttachment{p.apvts, "High Cut", highCutKnob},
      gainKnobAttachment{p.apvts, "Output Level", gainKnob},
      cabTypeComboBoxAttachment{p.apvts, "Cab Type", cabTypeComboBox},
      micTypeComboBoxAttachment{p.apvts, "Mic Type", micTypeComboBox},
      micDistanceKnobAttachment{p.apvts, "Mic Distance", micDistanceKnob},
      micAxisSwitchAttachment{p.apvts, "Mic Axis", micAxisSwitch}
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    MyLabel.setJustificationType(juce::Justification::centred);
    MyLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colour(0xFFC3C3C3));
    MyLabel.setFont(juce::Font(40.0f, juce::Font::bold));

    cabTypeComboBox.setJustificationType(juce::Justification::centred);
    micTypeComboBox.setJustificationType(juce::Justification::centred);

    for (auto *comp : getComps())
    {
        addAndMakeVisible(comp);
    }
    setResizable(true, true);
    setResizeLimits(800, 437, 1918, 1050);

    const float ratio = 1.83f;
    getConstrainer()->setFixedAspectRatio(ratio);

    setSize(437, 800);
}

CabImpulseAudioProcessorEditor::~CabImpulseAudioProcessorEditor()
{
}

//==============================================================================
void CabImpulseAudioProcessorEditor::paint(juce::Graphics &g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colour(0xFF2C2C2C));

    g.setColour(juce::Colour(0xFFC3C3C3));
    g.setFont(16);
    
    g.drawFittedText("Cabinet Type",
                     cabTypeComboBox.getBounds().expanded(0, cabTypeComboBox.getBounds().getHeight() * 0.8),
                     juce::Justification::centredTop, 1);
    g.drawFittedText("Microphone Type",
                     micTypeComboBox.getBounds().expanded(0, micTypeComboBox.getBounds().getHeight() * 0.8),
                     juce::Justification::centredTop, 1);
    g.drawFittedText("Lo-Cut", lowCutKnob.getBounds(), juce::Justification::centredBottom, 1);
    g.drawFittedText("Output Level", gainKnob.getBounds(), juce::Justification::centredBottom, 1);
    g.drawFittedText("Hi-Cut", highCutKnob.getBounds(), juce::Justification::centredBottom, 1);
    g.drawFittedText("Mic Distance", micDistanceKnob.getBounds(), juce::Justification::centredBottom, 1);
}

void CabImpulseAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto bounds = getLocalBounds().reduced(20, 10);

    auto firstColumn = bounds.removeFromLeft(bounds.getWidth() / 3);
    auto secondColumn = bounds.removeFromLeft(bounds.getWidth() * 5 / 8);
    auto thirdColumn = bounds;

    auto cabTypeComboBoxBounds = firstColumn.removeFromTop(firstColumn.getHeight() / 3);
    cabTypeComboBoxBounds = cabTypeComboBoxBounds.reduced(cabTypeComboBoxBounds.getWidth() * 0.1, cabTypeComboBoxBounds.getHeight() * 0.4);
    cabTypeComboBox.setBounds(cabTypeComboBoxBounds);
    highCutKnob.setBounds(firstColumn.removeFromTop(firstColumn.getHeight() / 2));
    lowCutKnob.setBounds(firstColumn);

    auto micTypeComboBoxBounds = thirdColumn.removeFromTop(thirdColumn.getHeight() / 3);
    micTypeComboBoxBounds = micTypeComboBoxBounds.reduced(micTypeComboBoxBounds.getWidth() * 0.1, micTypeComboBoxBounds.getHeight() * 0.4);
    micTypeComboBox.setBounds(micTypeComboBoxBounds);

    micDistanceKnob.setBounds(thirdColumn.removeFromTop(thirdColumn.getHeight() / 2));
    micAxisSwitch.setBounds(thirdColumn);

    MyLabel.setBounds(secondColumn.removeFromTop(secondColumn.getHeight() / 3));
    gainKnob.setBounds(secondColumn);
}

std::vector<juce::Component *> CabImpulseAudioProcessorEditor::getComps()
{
    return {
        &lowCutKnob,
        &highCutKnob,
        &gainKnob,
        &cabTypeComboBox,
        &micTypeComboBox,
        &micDistanceKnob,
        &micAxisSwitch,
        &MyLabel};
}