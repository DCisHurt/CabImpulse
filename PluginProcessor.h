#pragma once

// #include <juce_audio_processors/juce_audio_processors.h>
// #include <juce_dsp/juce_dsp.h>
#include <JuceHeader.h>
#include "impulse_binarydata/BinaryData.h"

struct ChainSettings
{
    float gain{0}, lowCutFreq{0}, highCutFreq{0}, micDistance{0};
    int cabType{0}, micType{0};
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
    using Gain = juce::dsp::Gain<float>;
    using MonoChain = juce::dsp::ProcessorChain<CutFilter, ImpulseLoader, CutFilter, Gain>;

    MonoChain leftChain, rightChain;

    enum ChainPositions
    {
        LowCut,
        IRloader,
        HighCut,
        GainControl
    };

    AudioBuffer<float> irBufferClose;
    AudioBuffer<float> irbufferMid;
    AudioBuffer<float> irbufferFar;

    void updateCutFilter(const ChainSettings &chainSettings);
    void updateImpulseResponse(const ChainSettings &chainSettings);
    void updateGain(const ChainSettings &chainSettings);
    void updateParameters();
    void readIRbuffer(AudioBuffer<float> &buffer, int index);
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CabImpulseAudioProcessor)
};
