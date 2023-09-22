#pragma once

// #include <juce_audio_processors/juce_audio_processors.h>
// #include <juce_dsp/juce_dsp.h>
#include <JuceHeader.h>

struct ChainSettings
{
    float lowCutFreq{0}, highCutFreq{0};
    int micType{0}, micDistance{0};
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

    // enum CabType
    // {
    //     SM57,
    //     SM58,
    //     MD421,
    //     MD441,
    //     U87,
    //     R121,
    //     R122,
    //     R160,
    //     R121Ribbon,
    //     R122Ribbon,
    //     R160Ribbon,
    //     OffAxis
    // };

// Marshall 1936 2×12
// Marshall 1960 4×12
// Marshall 1960AHW 4×12
// Marshall 1970 4×12
// Line 6 Vetta 4×12
// ENGL Pro 4×12
// Randall RS412XLT100 4×12
// Krank Krankenstein 4×12
// Bogner Uberkab 4×12
// Mesa Standard 4×12
// Orange 4×12
// Genz Benz G-Flex ported 2×12
// Fender 1965 Super Reverb 4x10
// Roland JC120 2×12
// Supro Thunderbolt 1x15
// Vox AC30 2×12
// Fender Deluxe 1x12
// Ampeg SVT 8x10
// Ampeg Portaflex 1x15
// Aguilar DB 4x12
// Gallien-Krueger Neo 4x10



    enum MicDistance
    {
        Close,
        Near,
        Far
    };

    enum MicType
    {
        SM57,
        R121,
        MD409,
        MD421,
        S545SD,
        ADi5,
        M30,
        U87
    };

    void updateCutFilter(const ChainSettings &chainSettings);
    void updateImpulseResponse(const ChainSettings &chainSettings);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CabImpulseAudioProcessor)
};
