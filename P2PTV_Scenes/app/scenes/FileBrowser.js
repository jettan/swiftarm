function SceneFileBrowser(options){
    this.options = options;
    this.videoPos = {
        left: convCoord(700, 720),
        top: convCoord(100, 720),
        width: convCoord(500, 720),
        height: convCoord(400, 720)
    };
	
	this.playList = [{
		url: 'http://130.161.158.45/Argentina.mp4',
		title: 'argentina',
	},{
		url: 'file:///dtv/usb/sdb1/stream.mp4',
		title: 'streammp4 usb',
	},{
		url: 'http://130.161.158.45/stream.mp4',
		title: 'streammp4',
	},{
		url: 'http://10.88.76.195/heonie/testcontents/IU_YOU&I_1080p.flv',
		title: 'IU - You & I (1080p)'
	},{
		url: 'http://10.88.76.195/heonie/testcontents/Benz.TSSplit.1-26.m2ts_3000_720.mp4',
		title: '3D Test',
		is3D: true
	}];
	
	this.nState = sf.service.VideoPlayer.STATE_STOPPED;
	
	//sf.service.VideoPlayer.ERR_NOERROR   = 0;
    //sf.service.VideoPlayer.ERR_NETWORK    = 1;
    //sf.service.VideoPlayer.ERR_NOT_SUPPORTED  = 2;
	this.error2String = {};
	this.error2String[sf.service.VideoPlayer.ERR_NOERROR] = 'NoError';
    this.error2String[sf.service.VideoPlayer.ERR_NETWORK] = 'Network';
    this.error2String[sf.service.VideoPlayer.ERR_NOT_SUPPORTED] = 'Not Supported';
	
	//sf.service.VideoPlayer.STATE_PLAYING = 1;
    //sf.service.VideoPlayer.STATE_STOPPED  = 2;
    //sf.service.VideoPlayer.STATE_PAUSED   = 3;
    //sf.service.VideoPlayer.STATE_BUFFERING    = 4;
    //sf.service.VideoPlayer.STATE_SCANNING = 5;
	this.state2String = {};
	this.state2String[sf.service.VideoPlayer.STATE_PLAYING] = 'Playing';
    this.state2String[sf.service.VideoPlayer.STATE_STOPPED] = 'Stoped';
    this.state2String[sf.service.VideoPlayer.STATE_PAUSED] = 'Paused';
    this.state2String[sf.service.VideoPlayer.STATE_BUFFERING] = 'Buffering';
    this.state2String[sf.service.VideoPlayer.STATE_SCANNING] = 'Scanning';
	
	function convCoord(val, baseResol) {
		var rate = curWidget.height / baseResol;
		return parseInt(val*rate, 10);
	}
}

SceneFileBrowser.prototype.initialize = function(){
    alert("SceneVideoPlayerPartial.initialize()");
    // this function will be called only once when the scene manager show this scene first time
    // initialize the scene controls and styles, and initialize your variables here
    // scene HTML and CSS will be loaded before this function is called

	var items = [];
	for(var i=0; i<this.playList.length; i++) {
		items.push(this.playList[i].title);
	}
	$("#lstVideoPlayerPartial").sfList({
		data: items,
		index: 0,
		itemsPerPage: 8
	}).sfList('blur');
}

SceneFileBrowser.prototype.handleShow = function(){
    alert("SceneVideoPlayerPartial.handleShow()");
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
	sf.service.VideoPlayer.init(opt);
	
	sf.service.VideoPlayer.setKeyHandler(sf.key.RETURN, function () {
		sf.service.VideoPlayer.setFullScreen(false);
	});
	sf.service.VideoPlayer.setPosition(this.videoPos);
	sf.service.VideoPlayer.show();
}

SceneFileBrowser.prototype.handleHide = function(){
    alert("SceneVideoPlayerPartial.handleHide()");
    // this function will be called when the scene manager hide this scene
	sf.service.VideoPlayer.stop();
	sf.service.VideoPlayer.hide();
}

SceneFileBrowser.prototype.handleFocus = function(){
    alert("SceneVideoPlayerPartial.handleFocus()");
    // this function will be called when the scene manager focus this scene
	this.setKeyHelp();
	$("#lstVideoPlayerPartial").sfList('focus');
}

SceneFileBrowser.prototype.handleBlur = function(){
    alert("SceneVideoPlayerPartial.handleBlur()");
    // this function will be called when the scene manager move focus to another scene from this scene
	$("#lstVideoPlayerPartial").sfList('blur');
	if (sf.service.VideoPlayer.Skip.isInProgress()) {
		sf.service.VideoPlayer.Skip.cancel();
	}
}

SceneFileBrowser.prototype.handleKeyDown = function(keyCode){
    alert("SceneVideoPlayerPartial.handleKeyDown(" + keyCode + ")");
    // TODO : write an key event handler when this scene get focued
    switch (keyCode) {
        case sf.key.UP:
		    if (!sf.service.VideoPlayer.Skip.isInProgress()) {
				$("#lstVideoPlayerPartial").sfList('prev');
			}
            break;
        case sf.key.DOWN:
		    if (!sf.service.VideoPlayer.Skip.isInProgress()) {
                $("#lstVideoPlayerPartial").sfList('next');
            }
            break;
        case sf.key.ENTER:
			if (sf.service.VideoPlayer.Skip.isInProgress()) {
				sf.service.VideoPlayer.Skip.stop();
			}
			else {
				sf.service.VideoPlayer.stop();
				var item = this.playList[$("#lstVideoPlayerPartial").sfList('getIndex')];
				item.fullScreen = false;
				
				sf.service.VideoPlayer.play(item);
			}
			document.getElementById("VideoPlayerPartialEvent").innerHTML = (item.url + "<br>") + document.getElementById("VideoPlayerPartialEvent").innerHTML;
            break;
		case sf.key.RETURN:
			sf.key.preventDefault();
			if (sf.service.VideoPlayer.Skip.isInProgress()) {
				sf.service.VideoPlayer.Skip.cancel();
				sf.key.preventDefault();
			}
			sf.service.VideoPlayer.stop();
			sf.scene.hide('FileBrowser');
			sf.scene.show('Main');
			sf.scene.focus('Main');
			break;
			
		case sf.key.RED:
		    if (this.nState == sf.service.VideoPlayer.STATE_PLAYING || 
		    this.nState == sf.service.VideoPlayer.STATE_BUFFERING || 
		    this.nState == sf.service.VideoPlayer.STATE_PAUSED) {
		        sf.service.VideoPlayer.setFullScreen(true);
		    }
			break;
		
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
    }
}

SceneFileBrowser.prototype.printEvent = function(msg){
	alert("SceneVideoPlayerPartial.prototype.printEvent("+msg+")");
	document.getElementById("VideoPlayerPartialEvent").innerHTML = (msg + "<br>") + document.getElementById("VideoPlayerPartialEvent").innerHTML;
}

SceneFileBrowser.prototype.setKeyHelp = function (state) {
	//sf.service.VideoPlayer.STATE_PLAYING = 1;
    //sf.service.VideoPlayer.STATE_STOPPED  = 2;
    //sf.service.VideoPlayer.STATE_PAUSED   = 3;
    //sf.service.VideoPlayer.STATE_BUFFERING    = 4;
    //sf.service.VideoPlayer.STATE_SCANNING = 5;
	var oKeyMap = {};
	
	if (this.nState == sf.service.VideoPlayer.STATE_PLAYING || 
	this.nState == sf.service.VideoPlayer.STATE_PAUSED || 
	this.nState == sf.service.VideoPlayer.STATE_BUFFERING) {
		oKeyMap.RED = 'Fullscreen';
	}
	
	if (sf.service.VideoPlayer.Skip.isInProgress()) {
        oKeyMap.ENTER = 'Play';
		oKeyMap.RETURN = 'Cancel';
    }
	else {
		oKeyMap.UPDOWN = 'Move Item';
		oKeyMap.ENTER = 'Play';
        oKeyMap.RETURN = 'Return';
	}
	
	$("#keyhelp").sfKeyHelp(oKeyMap);
}
