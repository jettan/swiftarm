var widgetAPI = new Common.API.Widget();
var tvKey     = new Common.API.TVKeyValue();
var focuslocation;
var streaming = new Boolean();
var settable = new Boolean();

/**
 * The filesystem.
 */
var file_system = new FileSystem();

/**
 * The USB mount path.
 */
var usb_path = "$USB_DIR" + "/sda1/";

var streamUrl = "http://localhost:1337/stream";
var downloadUrl = "http://127.0.0.1:1337/download";
var closeUrl = "http://127.0.0.1:1337/close";

var Main = {
	selectedVideo : 0,
	mode : 0,
	mute : 0,
	
	UP : 0,
	DOWN : 1,
	
	WINDOW : 0,
	FULLSCREEN : 1,
	
	NMUTE : 0,
	YMUTE : 1
}

Main.onLoad = function() {
	if (Player.init() && Audio.init() && Display.init()) {
		Display.setVolume( Audio.getVolume() );
		Display.setTime(0);
		
		Player.stopCallback = function() {
			Main.setWindowMode();
		}
		
		// Enable key event processing
		this.enableKeys();
		
		widgetAPI.sendReadyEvent();
	} else {
		alert("Failed to initialise");
	}
	
	$('#Button').sfButton({text:'Button1'});
	$('#Button').sfButton('blur');
	$('#Button2').sfButton({text:'Button2'});
	$('#Button2').sfButton('blur');
	focuslocation = 0;
	streaming = false;
	settable = false;
}

Main.onUnload = function() {
    Player.deinit();
}

Main.updateCurrentVideo = function(videoURL) {
    Player.setVideoURL(videoURL);
}

Main.enableKeys = function() {
    document.getElementById("anchor").focus();
}

Main.keyDown = function() {
	var keyCode = event.keyCode;
	alert("Key pressed: " + keyCode);
	
	switch(keyCode) {
		case tvKey.KEY_RETURN:
		case tvKey.KEY_PANEL_RETURN:
			alert ("RETURN");
			Player.stopVideo();
			widgetAPI.sendReturnEvent(); 
			break;    
		case tvKey.KEY_PLAY:
			alert ("PLAY");
			this.handlePlayKey();
			break;
		case tvKey.KEY_STOP:
			alert ("STOP");
			Player.stopVideo();
			if (streaming) {
				httpGet(closeUrl);
				streaming = false;
			}
			break;
		case tvKey.KEY_PAUSE:
			alert ("PAUSE");
			this.handlePauseKey();
			break;
		case tvKey.KEY_FF:
			alert ("FF");
			if(Player.getState() != Player.PAUSED) {
				Player.skipForwardVideo();
			}
			break;
		case tvKey.KEY_RW:
			alert ("RW");
			if (Player.getState() != Player.PAUSED) {
				Player.skipBackwardVideo();
			}
			break;
		case tvKey.KEY_VOL_UP:
		case tvKey.KEY_PANEL_VOL_UP:
			alert ("VOL_UP");
			if (this.mute == 0) {
				Audio.setRelativeVolume(0);
			}
			break;
		case tvKey.KEY_VOL_DOWN:
		case tvKey.KEY_PANEL_VOL_DOWN:
			alert ("VOL_DOWN");
			if (this.mute == 0)
				Audio.setRelativeVolume(1);
			break;
		case tvKey.KEY_DOWN:
			alert("DOWN");
			break;
		case tvKey.KEY_UP:
			alert("UP");
			break;
		case tvKey.KEY_LEFT:
			alert("LEFT");
			if (focuslocation == 0) {
				$('#Button2').sfButton('focus');
				focuslocation = 2;
			} else if (focuslocation == 1) {
				$('#Button').sfButton('blur');
				focuslocation = 0;
			} else {
				$('#Button2').sfButton('blur');
				$('#Button').sfButton('focus');
				focuslocation = 1;
			}
			alert (focuslocation);
			break;
			case tvKey.KEY_RIGHT:
				alert ("RIGHT");
				if (focuslocation == 0) {
					$('#Button').sfButton('focus');
					focuslocation = 1;
				} else if (focuslocation == 1) {
					$('#Button').sfButton('blur');
					$('#Button2').sfButton('focus');
					focuslocation = 2;
				} else {
					$('#Button2').sfButton('blur');
					focuslocation = 0;
				}
				alert(focuslocation);
				break;
			case tvKey.KEY_ENTER:
				alert ("ENTER");
				alert (focuslocation);
				if (focuslocation == 1) {
					buttonHandler();
				} else if (focuslocation == 2) {
					button2Handler();
				} else {
					this.toggleMode();
				}
			case tvKey.KEY_PANEL_ENTER:
				alert ("ENTER");
				break;
			case tvKey.KEY_MUTE:
				alert ("MUTE");
				this.muteMode();
				break;
			default:
				alert ("Unhandled key");
				break;
	}
}

Main.handlePlayKey = function() {
	switch ( Player.getState() ) {
		case Player.STOPPED:
			Player.playVideo();
			break;
		case Player.PAUSED:
			Player.resumeVideo();
			break;
		default:
			alert ("Ignoring play key, not in correct state");
			break;
	}
}

Main.handlePauseKey = function() {
	switch ( Player.getState() ) {
		case Player.PLAYING:
			Player.pauseVideo();
			break;
		default:
			alert ("Ignoring pause key, not in correct state");
			break;
	}
}

Main.setFullScreenMode = function() {
if (this.mode != this.FULLSCREEN) {
		Display.hide();
		Player.setFullscreen();
		this.mode = this.FULLSCREEN;
	}
}

Main.setWindowMode = function() {
	if (this.mode != this.WINDOW) {
		Display.show();
		Player.setWindow();
		this.mode = this.WINDOW;
	}
}

Main.toggleMode = function() {
	if (Player.getState() == Player.PAUSED) {
		Player.resumeVideo();
	}
	switch (this.mode) {
		case this.WINDOW:
			this.setFullScreenMode();
			break;
		case this.FULLSCREEN:
			this.setWindowMode();
			break;
		default:
			alert("ERROR: unexpected mode in toggleMode");
			break;
	}
}

Main.setMuteMode = function() {
	if (this.mute != this.YMUTE) {
		var volumeElement = document.getElementById("volumeInfo");
		Audio.plugin.SetUserMute(true);
		document.getElementById("volumeBar").style.backgroundImage = "url(Images/videoBox/muteBar.png)";
		document.getElementById("volumeIcon").style.backgroundImage = "url(Images/videoBox/mute.png)";
		widgetAPI.putInnerHTML(volumeElement, "MUTE");
		this.mute = this.YMUTE;
	}
}

Main.noMuteMode = function() {
	if (this.mute != this.NMUTE) {
		Audio.plugin.SetUserMute(false);
		document.getElementById("volumeBar").style.backgroundImage = "url(Images/videoBox/volumeBar.png)";
		document.getElementById("volumeIcon").style.backgroundImage = "url(Images/videoBox/volume.png)";
		Display.setVolume( Audio.getVolume() );
		this.mute = this.NMUTE;
	}
}

Main.muteMode = function() {
	switch (this.mute) {
		case this.NMUTE:
			this.setMuteMode();
			break;
		case this.YMUTE:
			this.noMuteMode();
			break;
		default:
			alert("ERROR: unexpected mode in muteMode");
			break;
	}
}

/**
 * Method to handle files on select and give the correct path to the player.
 */

function selectItem() {
	
	var file_url;
	
	
	//Player.setVideoURL(url3);
	Display.setDescription(url);
}


function buttonHandler() {
	alert("Button handler!");
	settable = true;
	httpGet(streamUrl);
	streaming = true;
	//testFileAPI();
}

function button2Handler() {
	alert("Button2 handler!");
	settable = true;
	httpGet(downloadUrl);
}

function httpGet(url) {
	request = new XMLHttpRequest();
	
	request.open("GET", url, true);
	request.onreadystatechange = processRequest;
	request.send(null);
}

function processRequest() {
	if (request.readyState == 4) {
		var result = request.responseText;
		if (settable)
			Main.updateCurrentVideo(result);
		Display.setDescription(result);
		settable = false;
		alert(result);
	}
}

