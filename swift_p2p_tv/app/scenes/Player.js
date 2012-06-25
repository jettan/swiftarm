function ScenePlayer(options){
    this.options = options;
    this.videoPos = {
        left: convCoord(760, 720),
        top: convCoord(100, 720),
        width: convCoord(500, 720),
        height: convCoord(400, 720)
    };
	// These item URL should be dynamically requested from the server
	this.playList = [{
		url: 'http://127.0.0.1/Argentina.mp4',
		title: 'Argentina',
	},{
		url: 'http://127.0.0.1/stream.mp4',
		title: 'streammp4'
	},{
		url: 'http://127.0.0.1/stream2.mp4',
		title: 'stream2mp4'
	},{
		url: 'http://127.0.0.1/narutoget.mp4',
		title: 'narutogetversion'
	}];
	
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
	
	this.curOpts = {}; for(var prop in this.defaultOpts) {this.curOpts[prop] = this.defaultOpts[prop];};
	this.aColumnOpts = [null, 'left', 'right', 'bottom'];
	this.iColumn = 0;
	
	this.btnData = [
		{text: 'Toggle Light', method: function () {
			this.curOpts.light = !this.curOpts.light;
			$('#MainBG').sfBackground('option', 'light', this.curOpts.light);
			this.setDescription('Toggle Light: ' + this.curOpts.light);
		}},
		{text: 'Toggle Column Mode', method: function () {
			this.iColumn = (this.iColumn+1)%this.aColumnOpts.length;
			this.curOpts.column = this.aColumnOpts[this.iColumn];
			$('#MainBG').sfBackground('option', 'column', this.curOpts.column);
		}},
		{text: 'Toggle Column Shadow', method: function () {
			this.curOpts.columnShadow = !this.curOpts.columnShadow;
			$('#MainBG').sfBackground('option', 'columnShadow', this.curOpts.columnShadow);
			this.setDescription('Toggle Column Shadow: ' + this.curOpts.columnShadow);
		}},
		{text: 'Change Column Size', method: function () {
			this.curOpts.columnSize = (this.curOpts.columnSize==350/720*curWidget.height)?200/720*curWidget.height:350/720*curWidget.height;
			$('#MainBG').sfBackground('option', 'columnSize', this.curOpts.columnSize);
			this.setDescription('Change Column Size: ' + this.curOpts.columnSize);
		}}
	];
	
	function convCoord(val, baseResol) {
		var rate = curWidget.height / baseResol;
		return parseInt(val*rate, 10);
	}
}

var item;
var fullScreen = false;
var stopStreamURL = "http://130.161.159.107:1337/stopStream";

ScenePlayer.prototype.initialize = function(){
    alert("ScenePlayer.initialize()");
	
	//playlist = [];
	
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

ScenePlayer.prototype.handleShow = function(){
    alert("ScenePlayer.handleShow()");
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

ScenePlayer.prototype.handleHide = function(){
    alert("ScenePlayer.handleHide()");
    // this function will be called when the scene manager hide this scene
	sf.service.VideoPlayer.stop();
	sf.service.VideoPlayer.hide();
}

ScenePlayer.prototype.handleFocus = function() {
    alert("ScenePlayer.handleFocus()");
    // this function will be called when the scene manager focus this scene
	this.setKeyHelp();
	this.refreshList();

	var _THIS_ = this;
	var mode = $('#labelRedirect').sfLabel("get").text();
	if ($('#labelRedirect').sfLabel("get").text())
			$('#labelRedirect').sfLabel('option','text','');
	
	if (mode == 'PlayerDownload') {
			_THIS_.printEvent('Selected Video : ' + $('#labelVideo').sfLabel("get").text());
			
			var link = 'file://' + downloadPath + '/' + $('#labelVideo').sfLabel("get").text();
			document.getElementById("MainListPage").innerHTML = $('#labelVideo').sfLabel("get").text();
			stream = link;
			
			playlist[0].url = link;
			playlist[0].title = $('#labelVideo').sfLabel("get").text();
			
			$("#lstPlayer").sfList('destroy');
			
			playlist = [{
				url: link,
				title: $('#labelVideo').sfLabel("get").text()
			}];
			
			var items = [];
			for(var i=0; i<playlist.length; i++) {
				items.push(playlist[i].title);
			}
			$("#lstPlayer").sfList({
				data: items,
				index: 0,
				itemsPerPage: 8
			}).sfList('blur');
			$("#lstPlayer").sfList('show');
			$("#lstPlayer").sfList('focus');
			
			_THIS_.printEvent('url: ' + playlist[0].url);
			_THIS_.printEvent('title: ' + playlist[0].title);
			
	}
		
	this.curOpts = {}; for(var prop in this.defaultOpts) {this.curOpts[prop] = this.defaultOpts[prop];};
	$('#MainBG').sfBackground(this.curOpts);
}

ScenePlayer.prototype.handleBlur = function(){
    alert("ScenePlayer.handleBlur()");
    // this function will be called when the scene manager move focus to another scene from this scene
	$("#lstPlayer").sfList('blur');
	$('#image').sfImage('hide');
	$('#label').sfLabel('hide');
	if (sf.service.VideoPlayer.Skip.isInProgress()) {
		sf.service.VideoPlayer.Skip.cancel();
	}
	
	$('#MainBG').sfBackground(this.defaultOpts);
}

ScenePlayer.prototype.handleKeyDown = function(keyCode){
    alert("ScenePlayer.handleKeyDown(" + keyCode + ")");
    // TODO : write an key event handler when this scene get focued
    switch (keyCode) {
        case sf.key.LEFT:
			//sf.scene.show('Main');
			$('#category').sfList('show');
			$('#image').sfImage('show');
			$('#label').sfLabel('show');
			sf.scene.focus('Main');
            break;
        case sf.key.UP:
		    if (!sf.service.VideoPlayer.Skip.isInProgress()) {
				$("#lstPlayer").sfList('prev');
			}
			document.getElementById("MainListPage").innerHTML = playlist[$("#lstPlayer").sfList('getIndex')].url;
            break;
        case sf.key.DOWN:
		    if (!sf.service.VideoPlayer.Skip.isInProgress()) {
                $("#lstPlayer").sfList('next');
            }
			document.getElementById("MainListPage").innerHTML = playlist[$("#lstPlayer").sfList('getIndex')].url;
            break;
        case sf.key.ENTER:
			if (sf.service.VideoPlayer.Skip.isInProgress()) {
				sf.service.VideoPlayer.Skip.stop();
			}
			else {
				sf.service.VideoPlayer.stop();
			
				item = playlist[$("#lstPlayer").sfList('getIndex')];
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
			//sf.scene.show('Main');
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
			break;
		case sf.key.N9:
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

ScenePlayer.prototype.printEvent = function(msg){
	alert("ScenePlayer.prototype.printEvent("+msg+")");
	var date = new Date();
	var timestr = date.getHours() + ":" + date.getMinutes() + ":" + date.getSeconds();
	document.getElementById("PlayerEvent").innerHTML = (timestr + "- " + msg + "<br>") + document.getElementById("PlayerEvent").innerHTML;
}

ScenePlayer.prototype.refreshList = function(){
			this.printEvent('REFRESHINGG!!');
			$("#lstPlayer").sfList('destroy');
			
			var items = [];
			for(var i=0; i<playlist.length; i++) {
				items.push(playlist[i].title);
			}
			
			$("#lstPlayer").sfList({
				data: items,
				index: 0,
				itemsPerPage: 8
			}).sfList('blur');
			$("#lstPlayer").sfList('show');
			$("#lstPlayer").sfList('focus');
			
			this.printEvent('url: ' + playlist[0].url);
			this.printEvent('title: ' + playlist[0].title);

}

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

function httpGetClose(url) {
	request = new XMLHttpRequest();
	request.open("GET", url, true);
	request.onreadystatechange = processCloseRequest;
	request.send(null);
}

function processCloseRequest() {
	if (request.readyState == 4) {
		var result = request.responseText;
		
	}	
}
