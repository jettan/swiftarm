var Player =
{
    plugin : null,
    state : -1,
    skipState : -1,
    stopCallback : null,    /* Callback function to be set by client */
    originalSource : null,
    
    STOPPED : 0,
    PLAYING : 1,
    PAUSED : 2,  
    FORWARD : 3,
    REWIND : 4
}

Player.init = function() {
    var success = true;
          alert("success vale :  " + success);    
    this.state = this.STOPPED;
    
    this.plugin = document.getElementById("pluginPlayer");
    
    if (!this.plugin)
    {
          alert("success vale this.plugin :  " + success);    
         success = false;
    }
    
    alert("success vale :  " + success);    
    
    alert("success vale :  " + success);    
    
    this.plugin.OnCurrentPlayTime = 'Player.setCurTime';
    this.plugin.OnStreamInfoReady = 'Player.setTotalTime';
    this.plugin.OnBufferingStart = 'Player.onBufferingStart';
    this.plugin.OnBufferingProgress = 'Player.onBufferingProgress';
    this.plugin.OnBufferingComplete = 'Player.onBufferingComplete';           
            
    alert("success vale :  " + success);       
    return success;
}

Player.deinit = function()
{
      alert("Player deinit !!! " );       
      
      if (this.plugin)
      {
            this.plugin.Stop();
      }
}

Player.setWindow = function() {
	var width      = this.plugin.GetVideoWidth();
	var height     = this.plugin.GetVideoHeight();
	
	if (width && height) {
		// Get aspect ratio from video.
		var aspect_ratio = width/height;
		var x = Math.round(480 / aspect_ratio);
		
		if (x > 270) {
			x     = 270 * aspect_ratio;
			var y = Math.round((480 - x) / 2);
			this.plugin.SetDisplayArea(458 + y, 58, x, 270);
		} else {
			var y  = Math.round((270 - x) / 2);
			this.plugin.SetDisplayArea(458, 58 + y, 480, x);
		}
	} else {
		this.plugin.SetDisplayArea(458, 58, 480, 270);
	}
}

Player.setFullscreen = function() {
	var width      = this.plugin.GetVideoWidth();
	var height     = this.plugin.GetVideoHeight();
	
	if (width && height) {
		// Get aspect ratio from video.
		var aspect_ratio = width/height;
		var x = Math.round(960 / aspect_ratio);
		
		if (x > 540) {
			x     = 540 * aspect_ratio;
			var y = Math.round((960 - x) / 2);
			this.plugin.SetDisplayArea(y, 0, x, 540);
		} else {
			var y  = Math.round((540 - x) / 2);
			this.plugin.SetDisplayArea(0, y, 960, x);
		}
	} else {
		this.plugin.SetDisplayArea(0, 0, 960, 540);
	}
}

Player.setVideoURL = function(url) {
    this.url = url;
    alert("URL = " + this.url);
}

Player.playVideo = function() {
	if (this.url == null) {
		alert("No videos to play");
	} else {
        this.state = this.PLAYING;
        document.getElementById("play").style.opacity = '0.2';
        document.getElementById("stop").style.opacity = '1.0';
        document.getElementById("pause").style.opacity = '1.0';
        document.getElementById("forward").style.opacity = '1.0';
        document.getElementById("rewind").style.opacity = '1.0';
        Display.status("Play");
        this.plugin.Play( this.url );
        Audio.plugin.SetSystemMute(false);
    }
}

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
    }
    else
    {
        alert("Ignoring stop request, not in correct state");
    }
}

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

Player.skipForwardVideo = function() {
    this.skipState = this.FORWARD;
    this.plugin.JumpForward(5);
}

Player.skipBackwardVideo = function() {
    this.skipState = this.REWIND;
    this.plugin.JumpBackward(5);
}

Player.getState = function() {
    return this.state;
}

// Global functions called directly by the player 

Player.onBufferingStart = function() {
    Display.status("Buffering...");
    switch(this.skipState)
    {
        case this.FORWARD:
            document.getElementById("forward").style.opacity = '0.2';
            break;
        
        case this.REWIND:
            document.getElementById("rewind").style.opacity = '0.2';
            break;
    }
}

Player.onBufferingProgress = function(percent) {
    Display.status("Buffering:" + percent + "%");
}

Player.onBufferingComplete = function()
{
    Display.status("Play");
    switch(this.skipState)
    {
        case this.FORWARD:
            document.getElementById("forward").style.opacity = '1.0';
            break;
        
        case this.REWIND:
            document.getElementById("rewind").style.opacity = '1.0';
            break;
    }
    this.setFullscreen();
}

Player.setCurTime = function(time)
{
    Display.setTime(time);
}

Player.setTotalTime = function()
{
    Display.setTotalTime(Player.plugin.GetDuration());
}

onServerError = function()
{
    Display.status("Server Error!");
}

OnNetworkDisconnected = function()
{
    Display.status("Network Error!");
}

getBandwidth = function(bandwidth) { alert("getBandwidth " + bandwidth); }

onDecoderReady = function() { alert("onDecoderReady"); }

onRenderError = function() { alert("onRenderError"); }

stopPlayer = function()
{
    Player.stopVideo();
}

setTottalBuffer = function(buffer) { alert("setTottalBuffer " + buffer); }

setCurBuffer = function(buffer) { alert("setCurBuffer " + buffer); }
