function ScenePlayer(options){
    this.options  = options;
    this.videoPos = {
        left: convCoord(760, 720),
        top: convCoord(100, 720),
        width: convCoord(500, 720),
        height: convCoord(400, 720)
    };
	
	this.bUseSubtitle = true;
	this.nCurSyncTime = 0;
	this.nState = sf.service.VideoPlayer.STATE_STOPPED;
	
	this.error2String = {};
	this.error2String[sf.service.VideoPlayer.ERR_NOERROR] = 'NoError';
    this.error2String[sf.service.VideoPlayer.ERR_NETWORK] = 'Network';
    this.error2String[sf.service.VideoPlayer.ERR_NOT_SUPPORTED] = 'Not Supported';
	
	this.state2String = {};
	this.state2String[sf.service.VideoPlayer.STATE_PLAYING] = 'Playing';
    this.state2String[sf.service.VideoPlayer.STATE_STOPPED] = 'Stoped';
    this.state2String[sf.service.VideoPlayer.STATE_PAUSED] = 'Paused';
    this.state2String[sf.service.VideoPlayer.STATE_BUFFERING] = 'Buffering';
    this.state2String[sf.service.VideoPlayer.STATE_SCANNING] = 'Scanning';
	
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
var fullScreen    = false;
var stopStreamURL = "http://130.161.159.107:1337/stopStream";

/**
 * Function called at scene init
 */
ScenePlayer.prototype.initialize = function(){

	var items = [];
	for(var i=0; i<this.playList.length; i++) {
		items.push(this.playList[i].title);
	}
	$("#lstPlayer").sfList({
		data: items,
		index: 0,
		itemsPerPage: 8
	}).sfList('blur');
	
}

/**
 * Function called at scene show
 */
ScenePlayer.prototype.handleShow = function(){
	var opt = {};
	var _THIS_ = this;
	opt.onerror = function(error, info){
		_THIS_.printEvent('ERROR : ' + (_THIS_.error2String[error]||error) + (info ? ' (' + info + ')' : ''));
	};
	opt.onend = function(){
		_THIS_.printEvent('END');
	};
	opt.onstatechange = function(state, info){
		_THIS_.printEvent('StateChange : ' + (_THIS_.state2String[state]||state) + (info ? ' (' + info + ')' : ''));
		_THIS_.nState = state;
		_THIS_.setKeyHelp();
	};
	// Initialize service api video player
	sf.service.VideoPlayer.init(opt);
	
	sf.service.VideoPlayer.setKeyHandler(sf.key.RETURN, function () {
		sf.service.VideoPlayer.setFullScreen(false);
	});
	sf.service.VideoPlayer.setPosition(this.videoPos);
	sf.service.VideoPlayer.show();
}

/**
 * Function called at scene hide
 */
ScenePlayer.prototype.handleHide = function(){
	sf.service.VideoPlayer.stop();
	sf.service.VideoPlayer.hide();
}

/**
 * Function called at scene focus
 */
ScenePlayer.prototype.handleFocus = function() {
	this.setKeyHelp();
	$("#lstPlayer").sfList('focus');

	var _THIS_ = this;
	var mode = $('#labelRedirect').sfLabel("get").text();
	
	if ($('#labelRedirect').sfLabel("get").text())
			$('#labelRedirect').sfLabel('option','text','');
	
	// Check were we got redirected from: usb file, downloaded file, stream
	if (mode == 'Player') {	
		if ($('#labelVideo').sfLabel("get").text()) {
			
			_THIS_.printEvent('Selected Video : ' + $('#labelVideo').sfLabel("get").text());
			
			var link = $('#labelVideo').sfLabel("get").text();
			link = 'file:///dtv/usb' + link.substring(8);
			
			document.getElementById("MainListPage").innerHTML = link;
			$('#labelVideo').sfLabel("option","text",link);
			stream = link;
			
			this.playList[0].url = link;
			this.playList[0].title = 'Play selection';
			
			$("#lstPlayer").sfList('destroy');
			
			// Show video selection in list	
			this.playList = [{
				url: link,
				title: 'Usb video'
			}];
			
			var items = [];
			for(var i=0; i<this.playList.length; i++) {
				items.push(this.playList[i].title);
			}
			$("#lstPlayer").sfList({
				data: items,
				index: 0,
				itemsPerPage: 8
			}).sfList('blur');
			$("#lstPlayer").sfList('show');
			$("#lstPlayer").sfList('focus');
		}
	} else if (mode == 'PlayerStream') {
			this.playList[0].url = stream;
			this.playList[0].title = 'Stream';
			
			$("#lstPlayer").sfList('destroy');
			
			// Show video selection in list
			this.playList = [{
				url: stream,
				title: 'Stream'
			}];
			
			var items = [];
			for(var i=0; i<this.playList.length; i++) {
				items.push(this.playList[i].title);
			}
			$("#lstPlayer").sfList({
				data: items,
				index: 0,
				itemsPerPage: 8
			}).sfList('blur');
			$("#lstPlayer").sfList('show');
			$("#lstPlayer").sfList('focus');
	} else if (mode == 'PlayerDownload') {	
			var link = 'file://' + downloadPath + '/' + $('#labelVideo').sfLabel("get").text();
			document.getElementById("MainListPage").innerHTML = $('#labelVideo').sfLabel("get").text();
			stream = link;
			
			this.playList[0].url = link;
			this.playList[0].title = $('#labelVideo').sfLabel("get").text();
			
			$("#lstPlayer").sfList('destroy');
			
			// Show video selection in list	
			this.playList = [{
				url: link,
				title: $('#labelVideo').sfLabel("get").text()
			}];
			
			var items = [];
			for(var i=0; i<this.playList.length; i++) {
				items.push(this.playList[i].title);
			}
			$("#lstPlayer").sfList({
				data: items,
				index: 0,
				itemsPerPage: 8
			}).sfList('blur');
			$("#lstPlayer").sfList('show');
			$("#lstPlayer").sfList('focus');
					
	}		
}

/**
 * Function called at scene blur
 */
ScenePlayer.prototype.handleBlur = function(){
	$("#lstPlayer").sfList('blur');
	$('#image').sfImage('hide');
	$('#label').sfLabel('hide');
	if (sf.service.VideoPlayer.Skip.isInProgress()) {
		sf.service.VideoPlayer.Skip.cancel();
	}
	
	$('#MainBG').sfBackground(this.defaultOpts);
}

/**
 * Function called at scene key down
 */
ScenePlayer.prototype.handleKeyDown = function(keyCode){
    switch (keyCode) {
        case sf.key.LEFT:
			$('#category').sfList('show');
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
			this.playList[0].url = stream;
			if (sf.service.VideoPlayer.Skip.isInProgress()) {
				sf.service.VideoPlayer.Skip.stop();
			}
			else {
				sf.service.VideoPlayer.stop();
			
				item = this.playList[$("#lstPlayer").sfList('getIndex')];
				item.fullScreen = false;
				
				var _THIS_ = this;
				_THIS_.printEvent('Playing Video : ' + item.title + ' | ' + item.url);
				
				sf.service.VideoPlayer.play(item);
				
				this.bUseSubtitle = item.subtitle ? true : false;
				
				this.nCurSyncTime = 0;
				if (this.bUseSubtitle) {
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
				if (this.nState == sf.service.VideoPlayer.STATE_PLAYING || this.nState == sf.service.VideoPlayer.STATE_BUFFERING)
					sf.service.VideoPlayer.pause();
				item.fullScreen = false;
				fullScreen = false;
				sf.service.VideoPlayer.resume(item);
				break;
			}
			sf.service.VideoPlayer.stop();
			httpGetClose(stopStreamURL);
			$('#category').sfList('show');
			$('#image').sfImage('show');
			$('#label').sfLabel('show');
			sf.scene.focus('Main');
			break;
			
		case sf.key.RED:
		    if (this.nState == sf.service.VideoPlayer.STATE_PLAYING || 
		    this.nState == sf.service.VideoPlayer.STATE_BUFFERING || 
		    this.nState == sf.service.VideoPlayer.STATE_PAUSED) {
		        sf.service.VideoPlayer.setFullScreen(true);
				fullScreen = true;
		    }
			break;
		case sf.key.GREEN:
			break
		case sf.key.PAUSE:
			sf.service.VideoPlayer.pause();
			break;
		case sf.key.PLAY:
			if (sf.service.VideoPlayer.Skip.isInProgress()) {
				sf.service.VideoPlayer.Skip.stop();
			}
			else {
				sf.service.VideoPlayer.resume();
			}
			break;
		case sf.key.STOP:
			if (sf.service.VideoPlayer.Skip.isInProgress()) {
				sf.service.VideoPlayer.Skip.cancel();
			}
			sf.service.VideoPlayer.stop();
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
			if(this.bUseSubtitle) {
				this.nCurSyncTime -= 500;
			}
			else {
				this.nCurSyncTime = 0;
			}
			sf.service.VideoPlayer.Subtitle.setSyncTime(this.nCurSyncTime);
			this.printEvent("set subtitle sync time: " + this.nCurSyncTime + "ms");
			break;
		case sf.key.BLUE:
			if(this.bUseSubtitle) {
				this.nCurSyncTime += 500;
			}
			else {
				this.nCurSyncTime = 0;
			}
			sf.service.VideoPlayer.Subtitle.setSyncTime(this.nCurSyncTime);
			this.printEvent("set subtitle sync time: " + this.nCurSyncTime + "ms");
			break;
    }
}

/**
 * Function prints messages in box under the videoplayer
 */
ScenePlayer.prototype.printEvent = function(msg){
	alert("ScenePlayer.prototype.printEvent("+msg+")");
	var date = new Date();
	var timestr = date.getHours() + ":" + date.getMinutes() + ":" + date.getSeconds();
	document.getElementById("PlayerEvent").innerHTML = (timestr + "- " + msg + "<br>") + document.getElementById("PlayerEvent").innerHTML;
}

/**
 * Function that sets the key help to user defined button tips
 */
ScenePlayer.prototype.setKeyHelp = function (state) {
	var oKeyMap = {};
	
	if (this.nState == sf.service.VideoPlayer.STATE_PLAYING || 
	this.nState == sf.service.VideoPlayer.STATE_PAUSED || 
	this.nState == sf.service.VideoPlayer.STATE_BUFFERING) {
		oKeyMap.RED = 'Fullscreen';
	}
	
	if(this.bUseSubtitle) {
        oKeyMap.YELLOW = 'Sync -0.5sec',
        oKeyMap.BLUE = 'Sync +0.5sec'
    }
	
	if (sf.service.VideoPlayer.Skip.isInProgress()) {
        oKeyMap.ENTER = 'Play';
		oKeyMap.RETURN = 'Cancel';
    }
	else if (fullScreen) {
		oKeyMap.RETURN = 'Toggle fullscreen';
	}
	else {
		oKeyMap.UPDOWN = 'Move Item';
		oKeyMap.ENTER = 'Play';
        oKeyMap.RETURN = 'Return';
	}
	
	$("#Main_keyhelp").sfKeyHelp(oKeyMap);
}

/**
 * Function that sends http request to server
 */
function httpGetClose(url) {
	request = new XMLHttpRequest();
	request.open("GET", url, true);
	request.onreadystatechange = processCloseResponse;
	request.send(null);
}

/**
 * Function that processes response from a server after a request has been sent
 */
function processCloseResponse() {
	if (request.readyState == 4) {
		var result = request.responseText;
		
	}	
}
