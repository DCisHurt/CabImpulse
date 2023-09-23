#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CabImpulseAudioProcessor::CabImpulseAudioProcessor()
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      )
{
}

CabImpulseAudioProcessor::~CabImpulseAudioProcessor()
{
}

//==============================================================================
const juce::String CabImpulseAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CabImpulseAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool CabImpulseAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool CabImpulseAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double CabImpulseAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CabImpulseAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

int CabImpulseAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CabImpulseAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String CabImpulseAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void CabImpulseAudioProcessor::changeProgramName(int index, const juce::String &newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void CabImpulseAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 1;

    leftChain.prepare(spec);
    rightChain.prepare(spec);

    auto chainSettings = getChainSettings(apvts);

    updateCutFilter(chainSettings);
    updateImpulseResponse(chainSettings);
    updateGain(chainSettings);
}

void CabImpulseAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool CabImpulseAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

        // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}

void CabImpulseAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                            juce::MidiBuffer &midiMessages)
{
    juce::ignoreUnused(midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    auto chainSettings = getChainSettings(apvts);

    updateCutFilter(chainSettings);
    updateImpulseResponse(chainSettings);
    updateGain(chainSettings);

    juce::dsp::AudioBlock<float> block(buffer);

    auto leftBlock = block.getSingleChannelBlock(0);
    auto rightBlock = block.getSingleChannelBlock(1);

    juce::dsp::ProcessContextReplacing<float> leftContext(leftBlock);
    juce::dsp::ProcessContextReplacing<float> rightContext(rightBlock);

    leftChain.process(leftContext);
    rightChain.process(rightContext);
}

//==============================================================================
bool CabImpulseAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *CabImpulseAudioProcessor::createEditor()
{
    // return new CabImpulseAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void CabImpulseAudioProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused(destData);
}

void CabImpulseAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused(data, sizeInBytes);
}

void CabImpulseAudioProcessor::updateCutFilter(const ChainSettings &chainSettings)
{
    // int filterOrder = 2;

    auto lowCutCoefficients = juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(chainSettings.lowCutFreq,
                                                                                                          getSampleRate(),
                                                                                                          4);

    auto highCutCoefficients = juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(chainSettings.highCutFreq,
                                                                                                          getSampleRate(),
                                                                                                          4);

    auto &leftLowCut = leftChain.get<ChainPositions::LowCut>();
    auto &rightLowCut = rightChain.get<ChainPositions::LowCut>();
    auto &leftHighCut = leftChain.get<ChainPositions::HighCut>();
    auto &rightHighCut = rightChain.get<ChainPositions::HighCut>();

    *leftLowCut.get<0>().coefficients = *lowCutCoefficients[0];
    *leftLowCut.get<1>().coefficients = *lowCutCoefficients[1];
    *rightLowCut.get<0>().coefficients = *lowCutCoefficients[0];
    *rightLowCut.get<1>().coefficients = *lowCutCoefficients[1];

    *leftHighCut.get<0>().coefficients = *highCutCoefficients[0];
    *leftHighCut.get<1>().coefficients = *highCutCoefficients[1];
    *rightHighCut.get<0>().coefficients = *highCutCoefficients[0];
    *rightHighCut.get<1>().coefficients = *highCutCoefficients[1];
}

void CabImpulseAudioProcessor::updateImpulseResponse(const ChainSettings &chainSettings)
{
    String mic("IR_");
    mic += (String)(int)getSampleRate();
    mic += "_";
    mic += CabType[chainSettings.cabType];
    mic += "_";
    mic += MicType[chainSettings.micType];
    mic += "_";

    if (chainSettings.micOffAxis)
        mic += "OFF_";

    switch (chainSettings.micDistance)
    {
    case 0:
        mic += "Close";
        break;
    case 1:
        mic += "Mid";
        break;
    case 2:
        mic += "Far";
        break;

    default:
        break;
    }

    mic += "_wav";
    const char *irName = mic.toRawUTF8();
    const char *binaryData = 0;
    int binaryDataSize = 0;

    binaryData = BinaryData::getNamedResource(irName, binaryDataSize);

    auto *inputStream = new MemoryInputStream(binaryData, binaryDataSize, false);
    WavAudioFormat format;
    auto reader = format.createReaderFor(inputStream, true);

    // If reader was successfully created, load into AudioSampleBuffer array
    if (reader)
    {
        AudioBuffer<float> buffer(static_cast<int>(reader->numChannels),
                                  static_cast<int>(reader->lengthInSamples));
        reader->read(buffer.getArrayOfWritePointers(), buffer.getNumChannels(), 0, buffer.getNumSamples());

        leftChain.get<ChainPositions::IRloader>().loadImpulseResponse(std::move(buffer),
                                                                      getSampleRate(),
                                                                      juce::dsp::Convolution::Stereo::no,
                                                                      juce::dsp::Convolution::Trim::yes,
                                                                      juce::dsp::Convolution::Normalise::yes);

        rightChain.get<ChainPositions::IRloader>().loadImpulseResponse(std::move(buffer),
                                                                       getSampleRate(),
                                                                       juce::dsp::Convolution::Stereo::no,
                                                                       juce::dsp::Convolution::Trim::yes,
                                                                       juce::dsp::Convolution::Normalise::yes);
    }
    else
    {
        jassertfalse;
        return;
    }
}

void CabImpulseAudioProcessor::updateGain(const ChainSettings &chainSettings)
{
    auto &leftGain = leftChain.get<ChainPositions::GainControl>();
    auto &rightGain = rightChain.get<ChainPositions::GainControl>();
    leftGain.setGainDecibels(chainSettings.gain);
    rightGain.setGainDecibels(chainSettings.gain);
}

ChainSettings getChainSettings(juce::AudioProcessorValueTreeState &apvts)
{
    ChainSettings settings;

    settings.gain = apvts.getRawParameterValue("Gain")->load();
    settings.lowCutFreq = apvts.getRawParameterValue("Low Cut")->load();
    settings.highCutFreq = apvts.getRawParameterValue("High Cut")->load();
    settings.cabType = (int)apvts.getRawParameterValue("Cab Type")->load();
    settings.micType = (int)apvts.getRawParameterValue("Mic Type")->load();
    settings.micDistance = (int)apvts.getRawParameterValue("Mic Distance")->load();
    settings.micOffAxis = apvts.getParameter("Mic Off Axis")->getValue();

    return settings;
}

juce::AudioProcessorValueTreeState::ParameterLayout CabImpulseAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterFloat>("Gain",
                                                           "Gain",
                                                           juce::NormalisableRange<float>(-48.0f, 24.0f, 0.1f, 1.0f),
                                                           0.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("Low Cut",
                                                           "Low Cut",
                                                           juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 1.0f),
                                                           20.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("High Cut",
                                                           "High Cut",
                                                           juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 1.0f),
                                                           20000.0f));

    layout.add(std::make_unique<juce::AudioParameterChoice>("Cab Type",
                                                            "Cab Type",
                                                            juce::StringArray{
                                                                "Marshall 1936 2X12",
                                                                "Marshall 1960 4X12",
                                                                "Marshall 1960AHW 4X12",
                                                                "Marshall 1970 4X12",
                                                                "Line 6 Vetta 4X12",
                                                                "ENGL Pro 4X12",
                                                                "Randall RS412XLT100 4X12",
                                                                "Krank Krankenstein 4X12",
                                                                "Bogner Uberkab 4X12",
                                                                "Mesa Standard 4X12",
                                                                "Orange 4X12",
                                                                "Genz Benz G-Flex ported 2X12",
                                                                "Fender 1965 Super Reverb 4X10",
                                                                "Roland JC120 2X12",
                                                                "Supro Thunderbolt 1X15",
                                                                "Vox AC30 2X12",
                                                                "Fender Deluxe 1X12",
                                                                "Ampeg SVT 8X10",
                                                                "Ampeg Portaflex 1X15",
                                                                "Aguilar DB 4X12",
                                                                "Gallien-Krueger Neo 4X10"},
                                                            0));

    layout.add(std::make_unique<juce::AudioParameterChoice>("Mic Type",
                                                            "Mic Type",
                                                            juce::StringArray{
                                                                "SM57",
                                                                "R121",
                                                                "MD409",
                                                                "MD421",
                                                                "S545SD",
                                                                "ADi5",
                                                                "M30",
                                                                "U87"},
                                                            0));

    layout.add(std::make_unique<juce::AudioParameterChoice>("Mic Distance",
                                                            "Mic Distance",
                                                            juce::StringArray{"Close", "Mid", "Far"},
                                                            0));

    layout.add(std::make_unique<juce::AudioParameterBool>("Mic Off Axis",
                                                          "Mic Off Axis",
                                                          false));

    return layout;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new CabImpulseAudioProcessor();
}
