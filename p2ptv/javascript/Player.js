/**
 * The player.
 */
var Player  {
	plugin : null,
	state : -1,
	skipState : -1,
	stopCallback : null,
	originalSource : null,
	
	STOPPED : 0,
	PLAYING : 1,
	PAUSED : 2,
	FORWARD : 3,
	REWIND : 4
}

/**
 * Initialise the player.
 */
Player.init = function() {
	var success = true;
		alert("success vale :  " + success);    
	
	// Set the initial state to STOPPED.
	this.state = this.STOPPED;
	
	this.plugin = document.getElementById("pluginPlayer");
	
	if (!this.plugin) {
		alert("success vale this.plugin :  " + success);    
		success = false;
	}
	
	this.setWindow();
	
	this.plugin.OnCurrentPlayTime   = 'Player.setCurTime';
	this.plugin.OnStreamInfoReady   = 'Player.setTotalTime';
	this.plugin.OnBufferingStart    = 'Player.onBufferingStart';
	this.plugin.OnBufferingProgress = 'Player.onBufferingProgress';
	this.plugin.OnBufferingComplete = 'Player.onBufferingComplete';
	
	return success;
}

/**
 * Destroy the player.
 */
Player.deinit = function() {
	if (this.plugin) {
		this.plugin.Stop();
	}
}

/**
 * Set the player to windowed mode.
 */
Player.setWindow = function() {
	this.plugin.SetDisplayArea(458, 58, 472, 270);
}

/**
 * Set the player to fullscreen mode.
 */
Player.setFullscreen = function() {
	this.plugin.SetDisplayArea(0, 0, 960, 540);
}

/**
 * Set the url from which the player will playback.
 */
Player.setVideoURL = function(url) {
	this.url = url;
}

/**
 * Play the video, only if the url is not empty.
 */
Player.playVideo = function() {
	if (this.url != null) {
		this.state = this.PLAYING;
		document.getElementById("play").style.opacity = '0.2';
		document.getElementById("stop").style.opacity = '1.0';
		document.getElementById("pause").style.opacity = '1.0';
		document.getElementById("forward").style.opacity = '1.0';
		document.getElementById("rewind").style.opacity = '1.0';
		Display.status("Play");
		this.setWindow();
		this.plugin.Play( this.url );
		Audio.plugin.SetSystemMute(false);
    }
}

/**
 * Pause the video.
 */
Player.pauseVideo = function() {
	this.state = this.PAUSED;
	document.getElementById("play").style.opacity = '1.0';
	document.getElementById("stop").style.opacity = '1.0';
	document.getElementById("pause").style.opacity = '0.2';
	document.getElementById("forward").style.opacity = '0.2';
	document.getElementById("rewind").style.opacity = '0.2';
	Display.status("Pause");
	this.plugin.Pause();
}

/**
 * Stop the video playback.
 */
Player.stopVideo = function() {
	if (this.state != this.STOPPED) {
		this.state = this.STOPPED;
		document.getElementById("play").style.opacity = '1.0';
		document.getElementById("stop").style.opacity = '0.2';
		document.getElementById("pause").style.opacity = '0.2';
		document.getElementById("forward").style.opacity = '0.2';
		document.getElementById("rewind").style.opacity = '0.2';
		Display.status("Stop");
		this.plugin.Stop();
		Display.setTime(0);
		
		if (this.stopCallback) {
			this.stopCallback();
		}
	}
}

/**
 * Resume the video.
 */
Player.resumeVideo = function() {
	this.state = this.PLAYING;
	document.getElementById("play").style.opacity = '0.2';
	document.getElementById("stop").style.opacity = '1.0';
	document.getElementById("pause").style.opacity = '1.0';
	document.getElementById("forward").style.opacity = '1.0';
	document.getElementById("rewind").style.opacity = '1.0';
	Display.status("Play");
	this.plugin.Resume();
}

/**
 * Forward the video with 10 seconds.
 */
Player.skipForwardVideo = function() {
	this.skipState = this.FORWARD;
	this.plugin.JumpForward(10);
}

/**
 * Rewind the video with 10 seconds.
 */
Player.skipBackwardVideo = function() {
	this.skipState = this.REWIND;
	this.plugin.JumpBackward(10);
}

/**
 * Get the state of the player.
 * States can be PLAYING, STOPPED and PAUSED.
 */
Player.getState = function() {
    return this.state;
}

// Global functions called directly by the player.

/**
 * Disables the forward and rewind function when still buffering.
 */
Player.onBufferingStart = function() {
	Display.status("Buffering...");
	switch(this.skipState) {
		case this.FORWARD:
			document.getElementById("forward").style.opacity = '0.2';
			break;
		case this.REWIND:
			document.getElementById("rewind").style.opacity = '0.2';
			break;
	}
}

/**
 * Shows the buffering progress.
 * @param percent: The percentage completed.
 */
Player.onBufferingProgress = function(percent) {
	Display.status("Buffering:" + percent + "%");
}

/**
 * Make sure the forward and rewind functions can be used again when buffering is complete.
 */
Player.onBufferingComplete = function() {
	Display.status("Play");
	switch(this.skipState) {
		case this.FORWARD:
			document.getElementById("forward").style.opacity = '1.0';
			break;
		case this.REWIND:
			document.getElementById("rewind").style.opacity = '1.0';
			break;
	}
}

/**
 * Set the the current time position of the video for display.
 * @param time: The time to be set.
 */
Player.setCurTime = function(time) {
	Display.setTime(time);
}

/**
 * Set the video duration in the display.
 */
Player.setTotalTime = function() {
	Display.setTotalTime(Player.plugin.GetDuration());
}

/**
 * Called when server error has occurred.
 */
onServerError = function() {
	Display.status("Server Error!");
}

/**
 * Called when the TV is disconnected from the network.
 */
OnNetworkDisconnected = function() {
    Display.status("Network Error!");
}

/**
 * Callback for getBandwidth.
 */
stopPlayer = function() {
    Player.stopVideo();
}
