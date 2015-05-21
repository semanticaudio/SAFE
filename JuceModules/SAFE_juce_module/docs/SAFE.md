SAFE
====

![What a handsome Moose!](moose.png)

## Creating a SAFE Plug-In
To create a new SAFE plug-in one starts by inheriting from SAFEAudioProcessor. This class
deals with analysing audio and communicating with the SAFE server and a variety of
other bits and bobs. For very simple plug-ins all you need to do is add some parameters using
SAFEAudioProcessor::addParameter() and provide some audio processing in 
SAFEAudioProcessor::pluginProcessing() and you're away.

A quick editor can also be created by inheriting the SAFEAudioProcessorEditor class.
Using this class sliders are automatically generated for each of the plug-ins parameters
(SAFEAudioProcessorEditor::sliders). These can be placed wherever you deem appropriate on you
editor. Some buttons and a text editor are also generated to be placed on the editor. See the 
protected members on the SAFEAudioProcessorEditor page for more information.

## The Team
### Sean (Baked Goods Supervisor)

@htmlonly
<div style="text-align: left">
	<img style="width: 400px; height: auto" src="sean.jpg"/>
</div>
@endhtmlonly

### Ryan (Chief Moose Herder)

@htmlonly
<div style="text-align: left">
	<img style="width: 400px; height: auto" src="ryan.jpg"/>
</div>
@endhtmlonly

### Brecht (Keeper of the Phat Beats)

@htmlonly
<div style="text-align: left">
	<img style="width: 400px; height: auto" src="brecht.jpg"/>
</div>
@endhtmlonly

## Acknowledgments 

This project would be nothing if it weren't for JUCE.
https://github.com/julianstorer/JUCE

All the audio feature extraction is done using LibXtract.
https://github.com/jamiebullock/LibXtract
