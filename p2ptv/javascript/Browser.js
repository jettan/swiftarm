var widgetAPI = new Common.API.Widget();
var tvKey     = new Common.API.TVKeyValue();
var focuslocation;
var streaming = new Boolean();
var settable = new Boolean();

/**
 * The filesystem.
 */
var fileSystem;

/**
 * The USB mount path.
 */
var usb_path = "$USB_DIR" + "/sda1/";

var streamUrl = "http://localhost:1337/stream";
var downloadUrl = "http://127.0.0.1:1337/download";
var closeUrl = "http://127.0.0.1:1337/close";

var Browser = {
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

Browser.init = function() {
	if (Player.init() && Audio.init() && Display.init()) {
		Display.setVolume( Audio.getVolume() );
		Display.setTime(0);
		
		Player.stopCallback = function() {
			Main.setWindowMode();
		}
		
		// Enable key event processing
		this.enableKeys();
		
		widgetAPI.sendReadyEvent();
		
		Display.setDiscription("Display Test");
	} else {
		alert("Failed to initialise");
	}
	
	var data = ['Item1', 'Item2', 'Item3', 'Item4', 'Item5'];
	$('#fileList').sfList({data:data, index:'0', itemsPerPage:'5'});
	//$('#scroll').sfScroll({page:5});
	
	focuslocation = 0;
	streaming = false;
	settable = false;
	
	fileSystem = new FileSystem();
	// '$USB_DIR'
	var files = fileSystem.readDir('/dtv/usb/');
	
	if(files){
		for(int i = 0; i < fileObj.length; i++){
			alert("Filename " + (i + 1) + ": " + files.[i].name);
			alert("Is a directory? " + files.[i].isDir);
			Display.setDiscription("Filename " + (i + 1) + ": " + files[i].name);
		}
	}
}

Browser.onUnload = function() {
    Player.deinit();
}

Browser.updateCurrentVideo = function(videoURL) {
    Player.setVideoURL(videoURL);
}

Browser.enableKeys = function() {
    document.getElementById("anchor").focus();
}

Browser.keyDown = function() {
	var keyCode = event.keyCode;
	alert("Key pressed: " + keyCode);
	
	switch(keyCode) {
		case tvKey.KEY_RETURN:
		case tvKey.KEY_PANEL_RETURN:
			widgetAPI.blockNavigation();
			alert ("RETURN");
			Player.stopVideo();
			gotoMain();
			break;
		case tvKey.MENU:
		case tvKey.KEY_PRECH:
			alert("PRECH pressed");
			Player.stopVideo();
			gotoMain();
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
			break;
		case tvKey.KEY_RIGHT:
			alert ("RIGHT");
			break;
		case tvKey.KEY_ENTER:
			alert ("ENTER");
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

Browser.handlePlayKey = function() {
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

Browser.handlePauseKey = function() {
	switch ( Player.getState() ) {
		case Player.PLAYING:
			Player.pauseVideo();
			break;
		default:
			alert ("Ignoring pause key, not in correct state");
			break;
	}
}

Browser.setFullScreenMode = function() {
if (this.mode != this.FULLSCREEN) {
		Display.hide();
		Player.setFullscreen();
		this.mode = this.FULLSCREEN;
	}
}

Browser.setWindowMode = function() {
	if (this.mode != this.WINDOW) {
		Display.show();
		Player.setWindow();
		this.mode = this.WINDOW;
	}
}

Browser.toggleMode = function() {
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

Browser.setMuteMode = function() {
	if (this.mute != this.YMUTE) {
		var volumeElement = document.getElementById("volumeInfo");
		Audio.plugin.SetUserMute(true);
		document.getElementById("volumeBar").style.backgroundImage = "url(Images/videoBox/muteBar.png)";
		document.getElementById("volumeIcon").style.backgroundImage = "url(Images/videoBox/mute.png)";
		widgetAPI.putInnerHTML(volumeElement, "MUTE");
		this.mute = this.YMUTE;
	}
}

Browser.noMuteMode = function() {
	if (this.mute != this.NMUTE) {
		Audio.plugin.SetUserMute(false);
		document.getElementById("volumeBar").style.backgroundImage = "url(Images/videoBox/volumeBar.png)";
		document.getElementById("volumeIcon").style.backgroundImage = "url(Images/videoBox/volume.png)";
		Display.setVolume( Audio.getVolume() );
		this.mute = this.NMUTE;
	}
}

Browser.muteMode = function() {
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

function gotoMain(){
	
	window.location = "index.html";
}

