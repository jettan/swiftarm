/**
 * Constructor of the Player scene.
 */
function ScenePlayer(options) {
	this.options = options;
	this.videoPos = {
		left: convCoord(760, 720),
		top: convCoord(100, 720),
		width: convCoord(500, 720),
		height: convCoord(400, 720)
	};
	
	this.use_subtitles = true;
	this.nCurSyncTime = 0;
	this.video_state = sf.service.VideoPlayer.STATE_STOPPED;
	
	this.error_list = {};
	this.error_list[sf.service.VideoPlayer.ERR_NOERROR] = 'NoError';
	this.error_list[sf.service.VideoPlayer.ERR_NETWORK] = 'Network';
	this.error_list[sf.service.VideoPlayer.ERR_NOT_SUPPORTED] = 'Not Supported';
	
	this.state_list = {};
	this.state_list[sf.service.VideoPlayer.STATE_PLAYING] = 'Playing';
	this.state_list[sf.service.VideoPlayer.STATE_STOPPED] = 'Stoped';
	this.state_list[sf.service.VideoPlayer.STATE_PAUSED] = 'Paused';
	this.state_list[sf.service.VideoPlayer.STATE_BUFFERING] = 'Buffering';
	this.state_list[sf.service.VideoPlayer.STATE_SCANNING] = 'Scanning';
	
	this.defaultOpts = {
		light: true,
		column: null,
		columnShadow: true,
		columnSize: 350/720*curWidget.height
	}
	
	function convCoord(val, baseResol) {
		var rate = curWidget.height / baseResol;
		return parseInt(val*rate, 10);
	}
}

var item;
var fullScreen = false;

/**
 * Function called when initializing the scene Player.
 */
ScenePlayer.prototype.initialize = function() {
	var items = [];
	for(var i=0; i<playlist.length; i++) {
		items.push(playlist[i].title);
	}
	$("#lstPlayer").sfList({
		data: items,
		index: 0,
		itemsPerPage: 8
	}).sfList('blur');
	
}

/**
 * Function called when the scene Player is to be shown.
 */
ScenePlayer.prototype.handleShow = function() {
	var opt = {};
	var _THIS_ = this;
	opt.onstatechange = function(state, info) {
		_THIS_.video_state = state;
		_THIS_.setKeyHelp();
	};
	sf.service.VideoPlayer.init(opt);
	
	sf.service.VideoPlayer.setKeyHandler(sf.key.RETURN, function () {
		sf.service.VideoPlayer.setFullScreen(false);
	});
	sf.service.VideoPlayer.setPosition(this.videoPos);
	sf.service.VideoPlayer.show();
}


/**
 * Function called when the scene Player is to be hidden.
 */
ScenePlayer.prototype.handleHide = function() {
	sf.service.VideoPlayer.stop();
	sf.service.VideoPlayer.hide();
}

/**
 * Function called when the scene Player is given focus.
 */
ScenePlayer.prototype.handleFocus = function() {
	this.setKeyHelp();
	var _THIS_ = this;
	setTimeout(function() {
		_THIS_.refreshList();
	}, 1500);
	
	if (redirect) {
		redirect = false;
	}
	
	$('#app_layout').sfBackground('option', 'column', 'left');
}

/**
 * Function called when the scene Player is to be blurred.
 */
ScenePlayer.prototype.handleBlur = function() {
	$("#lstPlayer").sfList('blur');
	if (sf.service.VideoPlayer.Skip.isInProgress()) {
		sf.service.VideoPlayer.Skip.cancel();
	}
}

/**
 * Keyhandler for the scene Player.
 */
ScenePlayer.prototype.handleKeyDown = function(keyCode) {
	switch (keyCode) {
		case sf.key.LEFT:
			$('#scene_list').sfList('show');
			$('#image').sfImage('show');
			$('#label').sfLabel('show');
			sf.scene.focus('Main');
			break;
			
		case sf.key.UP:
			if (!sf.service.VideoPlayer.Skip.isInProgress()) {
				$("#lstPlayer").sfList('prev');
			}
			break;
			
		case sf.key.DOWN:
			if (!sf.service.VideoPlayer.Skip.isInProgress()) {
				$("#lstPlayer").sfList('next');
			}
			break;
			
		case sf.key.ENTER:
			
			if (sf.service.VideoPlayer.Skip.isInProgress()) {
				sf.service.VideoPlayer.Skip.stop();
			} else {
				sf.service.VideoPlayer.stop();
				
				item = playlist[$("#lstPlayer").sfList('getIndex')];
				item.fullScreen = false;
				
				sf.service.VideoPlayer.play(item);
				
				this.use_subtitles = item.subtitle ? true : false;
				
				this.nCurSyncTime = 0;
				if (this.use_subtitles) {
					sf.service.VideoPlayer.Subtitle.show();
					sf.service.VideoPlayer.Subtitle.setSyncTime(this.nCurSyncTime);
					sf.service.VideoPlayer.Subtitle.setVerticalPosition(sf.service.VideoPlayer.Subtitle.VPOS_TOP, 10);
					sf.service.VideoPlayer.Subtitle.setFontSize(10);
					sf.service.VideoPlayer.Subtitle.setFontColor('#00ffff');
				}
			}
			break;
			
		case sf.key.RETURN:
			sf.key.preventDefault();
			if (sf.service.VideoPlayer.Skip.isInProgress()) {
				sf.service.VideoPlayer.Skip.cancel();
			} else if (fullScreen) {
				if (this.video_state == sf.service.VideoPlayer.STATE_PLAYING || this.video_state == sf.service.VideoPlayer.STATE_BUFFERING)
					sf.service.VideoPlayer.pause();
				item.fullScreen = false;
				fullScreen = false;
				sf.service.VideoPlayer.resume(item);
				this.setKeyHelp();
				break;
			}
			sf.service.VideoPlayer.stop();
			if (streaming) {
				httpGetClose(stop_stream_url);
				playlist.splice(playlist.length -1, 1);
				this.refreshList();
				streaming = false;
			}
			$('#scene_list').sfList('show');
			$('#label').sfLabel('show');
			sf.scene.focus('Main');
			break;
			
		case sf.key.RED:
			if (this.video_state == sf.service.VideoPlayer.STATE_PLAYING ||
				this.video_state == sf.service.VideoPlayer.STATE_BUFFERING ||
				this.video_state == sf.service.VideoPlayer.STATE_PAUSED) {
				
				sf.service.VideoPlayer.setFullScreen(true);
				fullScreen = true;
			}
			break;
			
		case sf.key.GREEN:
			playlist.splice($("#lstPlayer").sfList('getIndex'),1);
			this.refreshList();
			break;
			
		case sf.key.PAUSE:
			sf.service.VideoPlayer.pause();
			break;
			
		case sf.key.PLAY:
			if (sf.service.VideoPlayer.Skip.isInProgress()) {
				sf.service.VideoPlayer.Skip.stop();
			} else {
				sf.service.VideoPlayer.resume();
			}
			break;
			
		case sf.key.STOP:
			if (sf.service.VideoPlayer.Skip.isInProgress()) {
				sf.service.VideoPlayer.Skip.cancel();
			}
			sf.service.VideoPlayer.stop();
			this.setKeyHelp();
			break;
			
		case sf.key.FF:
			sf.service.VideoPlayer.Skip.start(10);
			this.setKeyHelp();
			break;
			
		case sf.key.REW:
			sf.service.VideoPlayer.Skip.start(-10);
			this.setKeyHelp();
			break;
			
			
		case sf.key.YELLOW:
			if (this.use_subtitles) {
				this.nCurSyncTime -= 500;
			} else {
				this.nCurSyncTime = 0;
			}
			sf.service.VideoPlayer.Subtitle.setSyncTime(this.nCurSyncTime);
			break;
			
		case sf.key.BLUE:
			if (this.use_subtitles) {
				this.nCurSyncTime += 500;
			} else {
				this.nCurSyncTime = 0;
			}
			sf.service.VideoPlayer.Subtitle.setSyncTime(this.nCurSyncTime);
			break;
			
		default:
			break;
	}
}

/**
 * Refresh the playlist.
 */
ScenePlayer.prototype.refreshList = function() {
	$("#lstPlayer").sfList('destroy');
	
	var items = [];
	for (var i = 0; i < playlist.length; i++) {
		items.push(playlist[i].title);
	}
	
	$("#lstPlayer").sfList({
		data: items,
		index: 0,
		itemsPerPage: 8
	}).sfList('blur');
	$("#lstPlayer").sfList('show');
	$("#lstPlayer").sfList('focus');
}


/**
 * Set the keyhelpbar depending on the situation.
 */
ScenePlayer.prototype.setKeyHelp = function (state) {
	var oKeyMap = {};
	
	if (this.video_state == sf.service.VideoPlayer.STATE_PLAYING ||
		this.video_state == sf.service.VideoPlayer.STATE_PAUSED  ||
		this.video_state == sf.service.VideoPlayer.STATE_BUFFERING) {
		
		oKeyMap.RED = 'Fullscreen';
	}
	
	if (this.use_subtitles) {
		oKeyMap.YELLOW = 'Sync -0.5sec',
		oKeyMap.BLUE = 'Sync +0.5sec'
	}
	
	if (sf.service.VideoPlayer.Skip.isInProgress()) {
		oKeyMap.ENTER = 'Play';
		oKeyMap.RETURN = 'Cancel';
	} else if (fullScreen) {
		oKeyMap.RETURN = 'Toggle fullscreen';
	} else {
		oKeyMap.UPDOWN = 'Move Item';
		oKeyMap.ENTER = 'Play';
		oKeyMap.RETURN = 'Return';
		oKeyMap.GREEN = 'Remove from playlist';
	}
	
	$("#keyhelp_bar").sfKeyHelp(oKeyMap);
}

/**
 * Make a GET /stopStream request to the HTTP server.
 */
function httpGetClose(url) {
	request = new XMLHttpRequest();
	request.open("GET", url, true);
	request.onreadystatechange = processCloseRequest;
	request.send(null);
}

/**
 * Process the response of the /stopStream request.
 */
function processCloseRequest() {
	if (request.readyState == 4) {
		var result = request.responseText;
	}
}

