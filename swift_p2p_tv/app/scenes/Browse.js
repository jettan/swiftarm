function SceneBrowse() {
	this.row = 0;
	this.column = 0;
	
	// List with available files for download/stream
	// In future release a search function will be included
	var stream = 'Initiele waarde motherf***er';
	var buttons;
	var k = 0;
}

var tv_url          = "http://130.161.159.107:1337";
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
	
	$('#search_bar').sfTextInput({
		text:'sintel',
		maxlength:'10',
		oncomplete: function (text) {
			if (text) {
				httpGet(search_url + text);
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
	this.row = 0;
	this.column = 1;
	
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

SceneBrowser.prototype.handleEnter  = function () {}

SceneBrowser.prototype.handleUp    = function () {
	if (this.column == 0 && is_list_shown) {
		if (this.row == 0) {
			$('#search_bar').sfTextInput('blur');
			$('#results_list').sfList('focus');
			this.row = 1 + search_results.length;
			$('#results_list').sfList('move', search_results.length - 1);
			
		} else if ($('#results_list').sfList('getIndex') == 0) {
			$('#results_list').sfList('blur');
			$('#search_bar').sfTextInput('focus');
			this.row = 0;
		} else {
			$('#results_list').sfList('prev');
			this.row -= 1;
		}
		
	} else if (this.column == 1) {
		if (this.row == 0) {
			$('#usb_button').sfButton('blur');
			$('#upload_button').sfButton('focus');
			this.row = 2;
		} else if (this.row == 1) {
			$('#add_to_playlist_button').sfButton('blur');
			$('#usb_button').sfButton('focus');
			this.row = 0;
		} else {
			$('#upload_button').sfButton('blur');
			$('#add_to_playlist_button').sfButton('focus');
			this.row = 1;
		}
	}
}

SceneBrowser.prototype.handleDown  = function () {
	if (this.column == 0 && is_list_shown) {
		if (this.row == 0) {
				$('#search_bar').sfTextInput('blur');
				$('#results_list').sfList('focus');
				this.row = 1;
				$('#results_list').sfList('move',0);
		} else if ($('#results_list').sfList('getIndex') == search_results.length - 1) {
				$('#results_list').sfList('blur');
				alert(search_results.length);
				$('#search_bar').sfTextInput('focus');
				this.row = 0;
		} else {
			$('#results_list').sfList('next');
			this.row += 1;
		}
	} else if (this.column == 1) {
		if (this.row == 0) {
			$('#usb_button').sfButton('blur');
			$('#add_to_playlist_button').sfButton('focus');
			this.row = 1;
		} else if (this.row == 1){
			$('#add_to_playlist_button').sfButton('blur');
			$('#upload_button').sfButton('focus');
			this.row = 2;
		} else {
			$('#upload_button').sfButton('blur');
			$('#usb_button').sfButton('focus');
			this.row = 0;
		}
	}
}


SceneBrowser.prototype.handleLeft  = function () {
	if (this.column == 0) {
		if (this.row == 0)
			$('#search_bar').sfTextInput('blur');
		else if (is_list_shown)
			$('#results_list').sfList('blur');
			$('#usb_button').sfButton('focus');
			this.column = 1;
			this.row = 0;
	} else if (this.column == 1) {
		if (this.row == 0)
			$('#usb_button').sfButton('blur');
		else if (this.row == 1)
			$('#add_to_playlist_button').sfButton('blur');
		else
			$('#upload_button').sfButton('blur');
	
		if (is_list_shown) {
			$('#results_list').sfList('focus');
			this.column = 0;
			this.row = 1 + $('#results_list').sfList('getIndex');
		} else {
			$('#search_bar').sfTextInput('focus');
			this.column = 0;
			this.row = 0;
		}
		
	} else if (this.column == 2) {
		$('#go_to_player_button').sfButton('blur');
		$('#add_to_playlist_button').sfButton('focus');
		this.column = 1;
		this.row = 1;
	}
}


SceneBrowser.prototype.handleRight = function () {
	if (this.column == 0) {
		if (this.row == 0)
			$('#search_bar').sfTextInput('blur');
		else if (is_list_shown)
			$('#results_list').sfList('blur');
	
	$('#usb_button').sfButton('focus');
		this.column = 1;
		this.row = 0;
	} else if (this.column == 1) {
		if (this.row == 1 || this.row == 2) {
			$(buttons[this.row]).sfButton('blur');
			$('#go_to_player_button').sfButton('focus');
			this.column = 2;
		} else
			$('#usb_button').sfButton('blur');
		
		if (is_list_shown) {
			$('#results_list').sfList('focus');
			this.column = 0;
			this.row = 1 + $('#results_list').sfList('getIndex');
		} else {
			$('#search_bar').sfTextInput('focus');
			this.column = 0;
			this.row = 0;
		}
	} else if (this.column == 2) {
		$('#go_to_player_button').sfButton('blur');
		if (is_list_shown) {
			$('#results_list').sfList('focus');
			this.column = 0;
			this.row = 1 + $('#results_list').sfList('getIndex');
		} else {
			$('#search_bar').sfTextInput('focus');
			this.column = 0;
			this.row = 0;
		}
	}
}

// TODO: Use enumerator instead of rows and columns.
SceneBrowse.prototype.handleKeyDown = function (key_code) {
	switch (keyCode) {
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
			if (this.column == 0 && is_list_shown && this.row > 0) {
				var index = $('#results_list').sfList('getIndex');
				$('#selection_label').sfLabel("option", "text", search_results[index]);
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
							$('#is_downloading').sfLabel('option','text','Downloading');
						} else {
							//Start streaming and redirect to player
							httpGet(stream_url + ":" + hashes[index]);
							var vid = stream;
							playlist.push({
								url: vid,
								title: 'Stream'
							});
							
							$('#label_redirect').sfLabel('option','text','Player');
							sf.scene.focus('Main');
						}
					}
				}).sfPopup('show');
			} else if (this.row == 0 && this.column == 1) {
				$("#usb_button").sfButton('blur');
				sf.service.USB.show({
					callback: function(result){
						$('#label_video').sfLabel("option", "text", result[0]); 
						$('#selection_label').sfLabel("option", "text", result[0]); 
						$("#go_to_player_button").sfButton('focus');
					},
					fileType: 'all'
				});
				this.column = 2;
				this.row = 1;
			} else if (this.row == 1 && this.column == 1) {
				// Add to selection to playlist(Check for correct path) 
				var vid = $('#label_video').sfLabel("get").text();
				vid = 'file:///dtv/usb' + vid.substring(8);
				var n = vid.split("/");
				var nlength = n.length - 1;
				
				playlist.push({
					url: vid,
					title: n[nlength]
				});
				
				$('#selection_label').sfLabel("option","text",playlist.length);
			
			} else if (this.row == 2 && this.column == 1){
				// Upload selection via swift(Check for correct path)
				if($('#feedback_label').sfLabel("get").text()) {
					var vid = $('#feedback_label').sfLabel("get").text()
					vid = 'file:///dtv/usb' + link.substring(8);
					httpGet(uploadUrl + vid);
				}
				
			} else if (this.row == 1 && this.column == 2) {
				$('#label_redirect').sfLabel('option','text','Player');
				$("#go_to_player_button").sfButton('blur');
				sf.scene.focus('Main');
			}
			break;
			
		case sf.key.RETURN:
			$('#scene_list').sfList('show');
			$('#image').sfImage('show');
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
	$('#selection_label').sfLabel("option", "text", "C =" + c);
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
		
		stream = resultHttp;
		// Url is returned in response when streaming, otherwise a number indicating fail or success
		if (resultHttp == -1)
			$('#feedback_label').sfLabel("option", "text", 'Request failed');
		else {
			if (resultHttp == 1)
				$('#feedback_label').sfLabel("option", "text", 'Download started');
			else
				$('#label_video').sfLabel('option','text',resultHttp);
		}
	}	
}
