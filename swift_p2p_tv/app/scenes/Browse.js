function SceneBrowse() {
	// List with available files for download/stream
	// In future release a search function will be included
	var stream = '';
	var buttons;
	var k = 0;
	this.browse_focus = 1;
}

var browse_enum = {
	SEARCH_BAR: 0,
	USB_BROWSER: 1,
	ADD_TO_PLAYLIST: 2,
	UPLOAD_FILE: 3,
	GO_TO_PLAYER: 4,
	RESULT_LIST: 5
}

var tv_url          = "";
var download_url    = tv_url + "/add:";
var upload_url      = tv_url + "/upload:";
var search_url      = tv_url + "/search:";
var result_url      = tv_url + "/results";
var stream_url      = tv_url + "/stream";
var stop_stream_url = tv_url + "/stopStream";

var is_list_shown   = false;
var search_results  = ["Search results"];



SceneBrowse.prototype.initialize = function () {
	$('#results_list').sfList({
		data: search_results,
		index:'0',
		itemsPerPage:'7'
	});
	
	$('#filebrowser_label').sfLabel({text:'Filebrowser'});
	$('#usb_button').sfButton({text:'Browse USB'});
	
	var _THIS_ = this;
	$('#search_bar').sfTextInput({
		text:'sintel',
		maxlength:'10',
		oncomplete: function (text) {
			if (text) {
				var search_terms = _THIS_.removeSpaces(text);
				httpGet(search_url + search_terms);
				startResultPolling();
			}
		}
	}).sfTextInput('blur');
	
	$('#search_label').sfLabel({text:'Search'});
	$('#go_to_player_button').sfButton({text:'Go to player'});
	$('#selected_file_label').sfLabel({text:'Selected file:'});
	$('#selection_label').sfLabel({text:'n/a'});
	$('#add_to_playlist_button').sfButton({text:'Add to playlist'});
	
	$('#go_to_player_button').sfButton({text:'Go to player'});
	$('#search_label').sfLabel({text:'Search'});
	$('#upload_button').sfButton({text:'Upload file'});
	
	buttons = new Array('#usb_button','#add_to_playlist_button','#upload_button');
	
}

SceneBrowse.prototype.handleShow = function () {}

SceneBrowse.prototype.handleHide = function () {}

SceneBrowse.prototype.handleFocus = function () {
	this.browse_focus = 1;
	
	$('#filebrowser_label').sfLabel('show');
	$('#results_list').sfList('show');
	$('#usb_button').sfButton('focus');
	$("#keyhelp_bar").sfKeyHelp({
		'move':'Move',
		'return': 'Return'
	});
}

SceneBrowse.prototype.handleBlur = function () {
	$('#filebrowser_label').sfLabel('hide');
	$('#results_list').sfList('blur');
	$('#results_list').sfList('hide');
}

SceneBrowse.prototype.focusSearchLabel = function () {
	var s_bar = document.getElementById('search_bar');
	s_bar.style.borderWidth="2px";
	s_bar.style.borderBottomStyle="groove";
	s_bar.style.borderTopStyle="groove";
	s_bar.style.borderLeftStyle="groove";
	s_bar.style.borderRightStyle="groove";
	s_bar.style.borderBottomColor="#e6e6fa";
	s_bar.style.borderTopColor="#e6e6fa";
	s_bar.style.borderRightColor="#e6e6fa";
	s_bar.style.borderLeftColor="#e6e6fa";
}

SceneBrowse.prototype.blurSearchLabel = function () {
	var s_bar = document.getElementById('search_bar');
	s_bar.style.borderWidth="0px";
}

SceneBrowse.prototype.removeSpaces = function (terms) {
	var res; 
	var temp = terms.split(" ");
	res += temp[0];
	for(var i = 1; i < temp.length; i++) {
		res += "_" + temp[i];
	}
	return res;
}


SceneBrowse.prototype.handleEnter  = function () {
	switch (this.browse_focus) {
		case 0:
			$('#search_bar').sfTextInput('focus');
			break;
		case 1:
			$("#usb_button").sfButton('blur');
			sf.service.USB.show({
				callback: function(result) {
					$('#label_video').sfLabel("option", "text", result[0]);
					$('#selection_label').sfLabel("option", "text", result[0]);
					$("#add_to_playlist_button").sfButton('focus');
				},
				fileType: 'all'
			});
			this.browse_focus = 2;
			break;
		case 2:
			var vid = $('#label_video').sfLabel("get").text();
			vid = 'file:///dtv/usb' + vid.substring(8);
			var n = vid.split("/");
			var filename_location = n.length - 1;
			
			playlist.push({
				url: vid,
				title: n[filename_location]
			});
			break;
		case 3:
			var vid = $('#label_video').sfLabel("get").text()
			vid = 'file:///dtv/usb' + link.substring(8);
			httpGet(uploadUrl + vid);
			break;
		case 4:
			$('#label_redirect').sfLabel('option','text','Player');
			$("#go_to_player_button").sfButton('blur');
			sf.scene.focus('Main');
			break;
		case 5:
			var index = $('#results_list').sfList('getIndex');
			var _THIS_ = this;
			$('#on_select_popup').sfPopup({
				text:"Do you want to download or stream this file?",
				buttons: ["Download", "Stream"],
				defaultFocus: 1,
				keyhelp: {'return' : 'Return'},
				callback : function(selectedIndex) {
					if (selectedIndex == -1)
						$('#feedback_label').sfLabel("option", "text", 'Operation canceled');
					else if (selectedIndex == 0) {
						// Start download
						httpGet(download_url + hashes[index]);
						downloading = true;
					} else {
						//Start streaming and redirect to player
						httpGet(stream_url + ":" + hashes[index]);
						setTimeout(function() {
							var vid = stream;
							playlist.push({
								url: vid,
								title: 'Stream'
							});
							$('loading').sfLoading('hide');
							$('#label_redirect').sfLabel('option','text','Player');
							sf.scene.focus('Main');
							}, 1500);
						
						$('loading').sfLoading('show');
					}
				}
			}).sfPopup('show');
			break;
			
		default:
			break;
	}
}

SceneBrowse.prototype.handleUp    = function () {
	switch(this.browse_focus) {
		case 0:
			if (is_list_shown) {
				this.blurSearchLabel();
				$('#results_list').sfList('focus');
				this.browse_focus  = 5;
				$('#results_list').sfList('move', search_results.length - 1);
			}
			break;
		case 1:
			$('#usb_button').sfButton('blur');
			$('#upload_button').sfButton('focus');
			this.browse_focus  = 3;
			break;
		case 2:
			$('#add_to_playlist_button').sfButton('blur');
			$('#usb_button').sfButton('focus');
			this.browse_focus = 1;
			break
		case 3:
			$('#upload_button').sfButton('blur');
			$('#add_to_playlist_button').sfButton('focus');
			this.browse_focus = 2;
			break;
		case 5:
			if ($('#results_list').sfList('getIndex') == 0) {
				$('#results_list').sfList('blur');
				this.focusSearchLabel();
				this.browse_focus = 0;
			} else {
				$('#results_list').sfList('prev');
			}
			break;
		default:
			break;
	}
}

SceneBrowse.prototype.handleDown  = function () {
	switch (this.browse_focus) {
		case 0:
			if (is_list_shown) {
				this.blurSearchLabel();
				$('#results_list').sfList('focus');
				$('#results_list').sfList('move', 0);
				this.browse_focus  = 5;
			}
			break;
		case 1:
			$('#usb_button').sfButton('blur');
			$('#add_to_playlist_button').sfButton('focus');
			this.browse_focus  = 2;
			break;
		case 2:
			$('#add_to_playlist_button').sfButton('blur');
			$('#upload_button').sfButton('focus');
			this.browse_focus = 3;
			break;
		case 3:
			$('#upload_button').sfButton('blur');
			$('#usb_button').sfButton('focus');
			this.browse_focus = 1;
			break;
		case 5:
			if ($('#results_list').sfList('getIndex') == search_results.length - 1) {
				$('#results_list').sfList('blur');
				this.focusSearchLabel();
				this.browse_focus = 0;
			} else {
				$('#results_list').sfList('next');
			}
			break;
		default:
			break;
	}
}


SceneBrowse.prototype.handleLeft  = function () {
	switch(this.browse_focus) {
		case 0:
			this.blurSearchLabel();
			$('#usb_button').sfButton('focus');
			this.browse_focus = 1;
			break;
		case 1:
			// TODO: Go to label instead of search bar.
			$('#usb_button').sfButton('blur');
			this.focusSearchLabel();
			this.browse_focus = 0;
			break;
		case 2:
			$('#add_to_playlist_button').sfButton('blur');
			this.focusSearchLabel();
			this.browse_focus = 0;
			break;
		case 3:
			$('#upload_button').sfButton('blur');
			this.focusSearchLabel();
			this.browse_focus = 0;
			break;
		case 4:
			$('#go_to_player_button').sfButton('blur');
			$('#add_to_playlist_button').sfButton('focus');
			this.browse_focus = 2;
			break;
		case 5:
			$('#results_list').sfList('blur');
			$('#go_to_player_button').sfButton('focus');
			this.browse_focus = 4;
			break;
		default:
			break;
	}
}


SceneBrowse.prototype.handleRight = function () {
	switch(this.browse_focus) {
		case 0:
			this.blurSearchLabel();
			$('#usb_button').sfButton('focus');
			this.browse_focus = 1;
			break;
		case 1:
			$('#usb_button').sfButton('blur');
			this.focusSearchLabel();
			this.browse_focus = 0;
			break;
		case 2:
			$('#add_to_playlist_button').sfButton('blur');
			$('#go_to_player_button').sfButton('focus');
			this.browse_focus = 4;
			break;
		case 3:
			$('#upload_button').sfButton('blur');
			$('#go_to_player_button').sfButton('focus');
			this.browse_focus = 4;
			break;
		case 4:
			$('#go_to_player_button').sfButton('blur');
			this.focusSearchLabel();
			this.browse_focus = 0;
			break;
		case 5:
			$('#results_list').sfList('blur');
			$('#usb_button').sfButton('focus');
			this.browse_focus = 1;
			break;
		
		default:
			break;
	}
}

SceneBrowse.prototype.handleKeyDown = function (key_code) {
	switch (key_code) {
		
		case sf.key.LEFT:
			this.handleLeft();
			break;
			
		case sf.key.RIGHT:
			this.handleRight();
			break;
			
		case sf.key.UP:
			this.handleUp();
			break;
			
		case sf.key.DOWN:
			this.handleDown();
			break;
			
		case sf.key.ENTER:
			this.handleEnter();
			break;
			
		case sf.key.RED:
			$('#selection_label').sfLabel("option", "text", search_results.length);
			break;
			
		case sf.key.RETURN:
			$('#scene_list').sfList('show');
			$('#label').sfLabel('show');
			sf.scene.focus('Main');
			sf.key.preventDefault();
			break;
			
		default:
			break;
	}
}

var c = 0;
var t;
var timer_is_on = 0;

function timedCount() {
	result();
	c += 1;
	if (c <= 4 && search_results.length < 1)
		t = setTimeout("timedCount()", 4000);
	else
		stopCount();
}

function startResultPolling() {
	c              = 0;
	search_results = [];
	
	if (!timer_is_on) {
		timer_is_on = 1;
		timedCount();
	}
}

function stopCount() {
	clearTimeout(t);
	timer_is_on = 0;
}

var requestXml;
function result() {
	requestXml = new XMLHttpRequest();
	requestXml.open("GET", result_url, true);
	requestXml.onreadystatechange = processSearchResponse;
	requestXml.send(null);
}

function processSearchResponse() {
	if (requestXml.readyState == 4) {
		var resultXml = requestXml.responseXML;
		
		k = 0;
		names = [];
		trackers = [];
		hashes = [];
		$('RESULT', resultXml).each(function(i) {
			names[k] = $(this).find("NAME").text();
			trackers[k] = $(this).find("TRACKER").text();
			hashes[k] = $(this).find("HASH").text();
			k++;
		});
		
		search_results = [];
		for (var j = 0; j < names.length; j++)
			search_results.push(names[j]);
		
		$('#results_list').sfList('clear');
		$('#results_list').sfList('option', 'data', search_results);
		
		is_list_shown = true;
	}
}

var requestHttp;
function httpGet(url) {
	requestHttp = new XMLHttpRequest();
	requestHttp.open("GET", url, true);
	requestHttp.onreadystatechange = processRequest;
	requestHttp.send(null);
}

function processRequest() {
	if (requestHttp.readyState == 4) {
		var resultHttp = requestHttp.responseText;
		
		// Url is returned in response when streaming, otherwise a number indicating fail or success
		if (resultHttp == -1)
			$('#feedback_label').sfLabel("option", "text", 'Request failed');
		else {
			if (resultHttp == 1)
				$('#feedback_label').sfLabel("option", "text", 'Download started');
			else
				stream = resultHttp;
		}
	}
}
