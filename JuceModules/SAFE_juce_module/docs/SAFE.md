SAFE
====

@htmlonly
<script type="text/javascript">

function getWidth()
{
	if (document.width !== undefined)
	{
		return document.width;
	}
	else
	{
		return document.body.offsetWidth;
	}
}

function getHeight()
{
	if (document.height !== undefined)
	{
		return document.height;
	}
	else
	{
		return document.body.offsetHeight;
	}
}

function releaseTheMoose()
{
	var title = document.getElementById("title");
	title.src = "titleAlone.png";
	title.onclick = "";

	var moose = document.createElement("img");
	moose.src = "mooseAlone.png";
	moose.style.position = "absolute";
	moose.style.left = title.offsetLeft + "px";
	moose.style.top = title.offsetTop + "px";
	moose.id = "moose";
	document.body.appendChild(moose);

	var xDirection = 2;
	var yDirection = 2;

	setInterval (function bounce()
		{
			if (moose.x < 0 || moose.x > (getWidth() - moose.width))
				xDirection *= -1;

			if (moose.y < 0 || moose.y > (getHeight() - moose.height))
				yDirection *= -1;

			moose.style.left = moose.x + xDirection + "px";
			moose.style.top = moose.y + yDirection + "px";
		}, 10);
}
</script>

<div style="text-align: center">
	<img id="title" src="moose.png" onclick="releaseTheMoose()"/>
</div>
@endhtmlonly

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
<img style="width: 400px; height: auto" src="sean.jpg"/>
@endhtmlonly

### Ryan (Chief Moose Herder)

@htmlonly
<script type="text/javascript">
function toggleRyan()
{
	if (typeof toggleRyan.current == 'undefined')
	{
		toggleRyan.current = 0;
	}

	if (toggleRyan.current)
	{
		document.getElementById("ryan").src="ryan.jpg";
		toggleRyan.current = 0;
	}
	else
	{
		document.getElementById("ryan").src="alpaca.jpg";
		toggleRyan.current = 1;
	}
}
</script>

<img id="ryan" style="width: 400px; height: auto" src="ryan.jpg" onclick="toggleRyan()"/>
@endhtmlonly

### Brecht (Keeper of the Phat Beats)

@htmlonly
<img style="width: 400px; height: auto" src="brecht.jpg"/>
@endhtmlonly

## Acknowledgments 

This project would be nothing if it weren't for JUCE.
https://github.com/julianstorer/JUCE

All the audio feature extraction is done using LibXtract.
https://github.com/jamiebullock/LibXtract
