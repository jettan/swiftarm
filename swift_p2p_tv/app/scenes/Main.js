function SceneMain() {
	this.items_per_page = 4;
}

// Global variables.
var tv_ip          = "";
var stats_url      = "";
var pause_url      = "";
var resume_url     = "";
var switch_url     = "";
var stop_url       = "";
var remove_url     = "";
var clear_url      = "";

var download_url    = "";
var upload_url      = "";
var search_url      = "";
var result_url      = "";
var stream_url      = "";
var stop_stream_url = "";

var names           = new Array();
var trackers        = new Array();
var hashes          = new Array();
var playlist        = [];
var downloading     = false;
var redirect        = false;
var video_selection = "";
var download_path   = '/dtv/usb/sda1';

SceneMain.prototype.initialize = function () {
	this.data = [
		'Browse',
		'Downloads',
		'Player',
		'Settings',
	];
	
	var listdata = [];
	for (var i = 0; i < this.data.length; i++) {
		listdata.push(this.data[i]);
	}
	
	$('#scene_list').sfList({
		data: listdata,
		index: 0,
		itemsPerPage: this.items_per_page
	});
	
	$('#label').sfLabel({
		text: 'Swift P2P TV'
	});
	$('#label_video').sfLabel({text: ""});
	
	$('#label').sfLabel('hide');
	$('#label_video').sfLabel('hide');
	
	$('#background_image').sfImage({src:'images/logobg22.png'});
	
	$('#app_layout').sfBackground({
		light: false,
		column: null,
		columnShadow: true,
		columnSize: 350
	});
	
	this.defaultOpts = {
		light: false,
		column: null,
		columnShadow: true,
		columnSize: 350 / 720 * curWidget.height
	}
	
	this.methods = [{
			title: 'getAvailableNetworks',
			func: function(){
				try {
					var err = deviceapis.network.getAvailableNetworks(function(pReturn){
						var retValue = pReturn;
						
						if(retValue[getActiveIndex(retValue)]) {					
							tv_ip = "http://" + retValue[getActiveIndex(retValue)].ip + ":1337";
							tv_url = tv_ip;
						}
					}, function(code){alert("getAvailableNetworks returns " + code);});
				}
				catch (err) {
					alert("getAvailableNetworks returns " + err);
				}
			}
		}
	];
	
	function getActiveIndex( pNetList ) {
		for( var i = 0; i < pNetList.length; i++ ) {
			if( pNetList[i].isActive() ) {
				return i;
			}
		}
	}
	
	var method = this.methods[0];
	if(method && method.func) {
			method.func();
	}
	
	stats_url  = tv_url + "/stats";
	pause_url  = tv_url + "/pause:";
	resume_url = tv_url + "/resume:";
	switch_url = tv_url + "/download:";
	stop_url   = tv_url + "/stop:";
	remove_url = tv_url + "/remove:";
	clear_url  = tv_url + "/clear";
	
	download_url    = tv_url + "/add:";
	upload_url      = tv_url + "/upload:";
	search_url      = tv_url + "/search:";
	result_url      = tv_url + "/results";
	stream_url      = tv_url + "/stream";
	stop_stream_url = tv_url + "/stopStream";
}

SceneMain.prototype.handleShow = function (data) {
	$('#loading').sfLoading('show');
	// TODO: Show eyecandy while waiting for load.
	sf.scene.show('Browse');
	setTimeout(function() { sf.scene.hide('Browse'); sf.scene.show('Player');}, 3000);
	setTimeout(function() { sf.scene.hide('Player'); sf.scene.show('Settings');}, 5000);
	setTimeout(function() { sf.scene.hide('Settings'); sf.scene.show('Downloads');}, 7000);
	setTimeout(function() { sf.scene.hide('Downloads'); $('#loading').sfLoading('hide');}, 9000);
}

SceneMain.prototype.handleHide = function () {}

SceneMain.prototype.handleFocus = function () {
	var index = $('#scene_list').sfList('getIndex');
	// Hide whatever scene was focused.
	sf.scene.hide(this.data[index]);
	
	if (redirect) {
		// Redirect to Player scene.
		$('#scene_list').sfList('move', 2);
		sf.scene.show('Player');
		
		// Hide components of Main scene.
		$('#scene_list').sfList('blur'); 
		$('#scene_list').sfList('hide');
		$('#label').sfLabel('hide');
		$('#app_layout').sfBackground(this.defaultOpts);
		sf.scene.focus('Player');
	
	} else {
		$('#scene_list').sfList('focus');
		$('#label').sfLabel('show');
		$('#background_image').sfImage('show');
		$('#keyhelp_bar').sfKeyHelp({
			'enter': 'Enter',
			'move':'Move',
			'return': 'Quit',
		});
		
		$('#app_layout').sfBackground('option', 'column', 'left');
		$('#app_layout').sfBackground(this.defaultOpts);
		$('#app_layout').sfBackground('option', 'column', 'left');
	}
}

SceneMain.prototype.handleBlur = function () {
	$('#label').sfLabel('hide');
	$('#app_layout').sfBackground(this.defaultOpts);
}

SceneMain.prototype.handleKeyDown = function (keyCode) {
	switch (keyCode) {
		case sf.key.RIGHT:
		case sf.key.ENTER:
			$('#background_image').sfImage('hide');
			var index = $('#scene_list').sfList('getIndex');
			sf.scene.show(this.data[index]);
			$('#scene_list').sfList('blur');
			$('#scene_list').sfList('hide');
			$('#label').sfLabel('hide');
			$('#app_layout').sfBackground(this.defaultOpts);
			sf.scene.focus(this.data[index]);
			break;
		
		case sf.key.UP:
			$('#scene_list').sfList('prev');
			break;
		
		case sf.key.DOWN:
			$('#scene_list').sfList('next');
			break;
			
		case sf.key.RETURN:
			var _THIS_ = this;
			var exit = false;
			$('#popupExit').sfPopup({
					text:"Do you really want to exit the application?",
					buttons: ["Yes", "No"],
					defaultFocus: 1,
					keyhelp: {'return' : 'Return'},
					callback : function (selectedIndex){
						if (selectedIndex == 0) {
							exit = true;
						}
					}
			}).sfPopup('show');
			
			if (!exit)
				sf.key.preventDefault();
			else
				widgetApi.sendReturnEvent();
				//sf.core.exit(false);
			break;
			
		default:
			break;
	}
}

