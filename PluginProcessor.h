#pragma once

// #include <juce_audio_processors/juce_audio_processors.h>
// #include <juce_dsp/juce_dsp.h>
#include <JuceHeader.h>
#include "impulse_binarydata/BinaryData.h"

struct ChainSettings
{
    float lowCutFreq{0}, highCutFreq{0};
    int cabType{0}, micType{0}, micDistance{0};
    bool micOffAxis{0};
};

ChainSettings getChainSettings(juce::AudioProcessorValueTreeState &apvts);

//==============================================================================
class CabImpulseAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    CabImpulseAudioProcessor();
    ~CabImpulseAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

    void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor *createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String &newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState apvts{*this, nullptr, "Parameters", createParameterLayout()};

private:
    using Filter = juce::dsp::IIR::Filter<float>;
    using CutFilter = juce::dsp::ProcessorChain<Filter, Filter>;
    using ImpulseLoader = juce::dsp::Convolution;
    using MonoChain = juce::dsp::ProcessorChain<CutFilter, ImpulseLoader, CutFilter>;

    MonoChain leftChain, rightChain;

    enum ChainPositions
    {
        LowCut,
        IRloader,
        HighCut
    };

    juce::StringArray MicType = {"57", "121", "409", "421", "545", "i5", "Ref", "U87"};

    juce::StringArray CabType =
        {
            "Damien",
            "1960",
            "Sterling",
            "Green",
            "Angora",
            "Eagle",
            "Excel",
            "Igor",
            "Ogre",
            "Oversize",
            "Tangerine",
            "Genzie",
            "Blackface",
            "Chordal",
            "Dazed",
            "Top_Boost",
            "Tweed",
            "Manatee",
            "Motown",
            "Ag",
            "Neo"};

    void updateCutFilter(const ChainSettings &chainSettings);
    void updateImpulseResponse(const ChainSettings &chainSettings);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CabImpulseAudioProcessor)
};
