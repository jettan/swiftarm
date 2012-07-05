/**
 * Constructor of the Browse scene.
 */
function SceneBrowse() {
	this.browse_focus = 1;
	var stream = '';
	var k = 0;
}

/// Enumerator to know where the focus is.
var browse_enum = {
	SEARCH_BAR: 0,
	USB_BROWSER: 1,
	ADD_TO_PLAYLIST: 2,
	UPLOAD_FILE: 3,
	GO_TO_PLAYER: 4,
	RESULT_LIST: 5
}

var is_list_shown   = false;
var search_results  = ["Search results"];
var requestXml;
var requestHttp;

// Variables to start timer to be used when polling for search results
var c = 0;
var t;
var timer_is_on = 0;

/**
 * Function called at scene init.
 */
SceneBrowse.prototype.initialize = function () {
	// List where search results are displayed.
	$('#results_list').sfList({
		data: search_results,
		index:'0',
		itemsPerPage:'7'
	});
	
	// Static labels.
	$('#filebrowser_label').sfLabel({text:'Filebrowser'});
	$('#search_label').sfLabel({text:'Search'});
	$('#selected_file_label').sfLabel({text:'Selected file:'});
	
	// Search bar for remote file search in community.
	var _THIS_ = this;
	$('#search_bar').sfTextInput({
		text:'Enter text here..',
		maxlength:'20',
		oncomplete: function (text) {
			if (text) {
				var search_terms = _THIS_.removeSpaces(text);
				httpGet(search_url + search_terms);
				$('#search_label').sfLabel("option","text", "Searching..");
				var my_element = document.getElementById("search_label");
				my_element.style.fontStyle = "italic";
				startResultPolling();
			}
		}
	}).sfTextInput('blur');
	
	// Dynamic label to show video selection, and server responses.
	$('#selection_label').sfLabel({text:'n/a'});
	
	// Buttons to: go to player, add file to playlist, upload a file and browse usb
	$('#go_to_player_button').sfButton({text:'Go to player'});
	$('#add_to_playlist_button').sfButton({text:'Add to playlist'});
	$('#upload_button').sfButton({text:'Upload file'});
	$('#usb_button').sfButton({text:'Browse USB'});
	
}

/**
 * Function called at scene show.
 */
SceneBrowse.prototype.handleShow = function () {}

/**
 * Function called at scene hide.
 */
SceneBrowse.prototype.handleHide = function () {}

/**
 * Function called at scene focus.
 */
SceneBrowse.prototype.handleFocus = function () {
	// Set focus to the usb button
	this.browse_focus = 1;
	$('#usb_button').sfButton('focus');
	$('#filebrowser_label').sfLabel('show');
	$('#results_list').sfList('show');
	$("#keyhelp_bar").sfKeyHelp({
		'move':'Move',
		'return': 'Return'
	});
}

/**
 * Function called at scene blur.
 */
SceneBrowse.prototype.handleBlur = function () {
	$('#filebrowser_label').sfLabel('hide');
	$('#results_list').sfList('blur');
	$('#results_list').sfList('hide');
}

/**
 * Function to visualise the focus for the search bar
 */
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

/**
 * Function to undo the visualisation of the focus when the search bar is blurred
 */
SceneBrowse.prototype.blurSearchLabel = function () {
	var s_bar = document.getElementById('search_bar');
	s_bar.style.borderWidth="0px";
}

/**
 * Function to replace the spaces in a search term with an underscore.
 */
SceneBrowse.prototype.removeSpaces = function (terms) {
	var res = "";
	var temp = terms.split(" ");
	res += temp[0];
	for(var i = 1; i < temp.length; i++) {
		res += "_" + temp[i];
	}
	return res;
}

/**
 * Function to handle the pressing of the enter button based on the focus.
 */
SceneBrowse.prototype.handleEnter  = function () {
	switch (this.browse_focus) {
		// When search bar is focused open text input
		case 0:
			$('#search_bar').sfTextInput('focus');
			break;
			
		// When usb button is focused open usb browse module
		case 1:
			$("#usb_button").sfButton('blur');
			sf.service.USB.show({
				callback: function(result) {
					video_selection = result[0];
					$('#selection_label').sfLabel("option", "text", result[0]);
					$("#add_to_playlist_button").sfButton('focus');
				},
				fileType: 'all'
			});
			this.browse_focus = 2;
			break;
			
		// When add_to_playlist button is focused add file selection to playlist
		case 2:
			var vid = 'file:///dtv/usb' + video_selection.substring(8);
			var n = vid.split("/");
			var filename_location = n.length - 1;
			
			playlist.push({
				url: vid,
				title: n[filename_location]
			});
			$('#selection_label').sfLabel("option", "text", "File selection added to playlist");
			break;
			
		// When upload button is focused start uploading file selection
		case 3:
			var vid = "/dtv/usb" + video_selection.substring(8);
			httpGet(upload_url + vid);
			$('#selection_label').sfLabel("option", "text", "Seeding file..");
			break;
			
		// When go_to_player button is focused redirect to player
		case 4:
			redirect = true;
			$("#go_to_player_button").sfButton('blur');
			sf.scene.focus('Main');
			break;
			
		// When result list is focused, show popup with choice to download or stream selected file
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
						//Start streaming, add to playlist and redirect to player
						httpGet(stream_url + ":" + hashes[index]);
						setTimeout(function() {
							var vid = stream;
							playlist.push({
								url: vid,
								title: 'Stream'
							});
							streaming = true;
							$('loading').sfLoading('hide');
							redirect = true;
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

/*
 * Function to handle the pressing of the up button based on the focus.
 */
SceneBrowse.prototype.handleUp    = function () {
	switch(this.browse_focus) {
		// When at the search bar, and the result list is filled, then move focus to result list
		case 0:
			if (is_list_shown) {
				this.blurSearchLabel();
				$('#results_list').sfList('focus');
				this.browse_focus  = 5;
				$('#results_list').sfList('move', search_results.length - 1);
			}
			break;
		
		// When at the usb button, move focus to upload button
		case 1:
			$('#usb_button').sfButton('blur');
			$('#upload_button').sfButton('focus');
			this.browse_focus  = 3;
			break;
			
		// When at the add_to_playlist button, move focus to usb button
		case 2:
			$('#add_to_playlist_button').sfButton('blur');
			$('#usb_button').sfButton('focus');
			this.browse_focus = 1;
			break;
			
		// When at the upload button, move focus to add_to_playlist button
		case 3:
			$('#upload_button').sfButton('blur');
			$('#add_to_playlist_button').sfButton('focus');
			this.browse_focus = 2;
			break;
			
		// When at the result list go to the previous result if index!=0, otherwise move focus to the search bar
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

/*
 * Function to handle the pressing of the down button based on the focus.
 */
SceneBrowse.prototype.handleDown  = function () {
	switch (this.browse_focus) {
		// When at the search bar, and the result list is filled, then move focus to result list
		case 0:
			if (is_list_shown) {
				this.blurSearchLabel();
				$('#results_list').sfList('focus');
				$('#results_list').sfList('move', 0);
				this.browse_focus  = 5;
			}
			break;
			
		// When at the usb button, move focus to add_to_playlist button
		case 1:
			$('#usb_button').sfButton('blur');
			$('#add_to_playlist_button').sfButton('focus');
			this.browse_focus  = 2;
			break;
			
		// When at the add_to_playlist button, move focus to upload button
		case 2:
			$('#add_to_playlist_button').sfButton('blur');
			$('#upload_button').sfButton('focus');
			this.browse_focus = 3;
			break;
			
		// When at the upload button, move focus to usb button
		case 3:
			$('#upload_button').sfButton('blur');
			$('#usb_button').sfButton('focus');
			this.browse_focus = 1;
			break;
			
		// When at the result list go to the next result if index isn't the last element, otherwise move focus to the search bar
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

/*
 * Function to handle the pressing of the left button based on the focus.
 */
SceneBrowse.prototype.handleLeft  = function () {
	switch(this.browse_focus) {	
		// When at the search bar, move focus to usb button
		case 0:
			this.blurSearchLabel();
			$('#usb_button').sfButton('focus');
			this.browse_focus = 1;
			break;
		
		// When at the usb button, move focus to the search bar
		case 1:
			$('#usb_button').sfButton('blur');
			this.focusSearchLabel();
			this.browse_focus = 0;
			break;
			
		// When at the add_to_playlist button, move focus to the search bar
		case 2:
			$('#add_to_playlist_button').sfButton('blur');
			this.focusSearchLabel();
			this.browse_focus = 0;
			break;
			
		// When at the upload button, move focus to the search bar
		case 3:
			$('#upload_button').sfButton('blur');
			this.focusSearchLabel();
			this.browse_focus = 0;
			break;
			
		// When at the go_to_player button, move focus to the add_to_playlist button
		case 4:
			$('#go_to_player_button').sfButton('blur');
			$('#add_to_playlist_button').sfButton('focus');
			this.browse_focus = 2;
			break;
			
		// When at the results list, move focus to the go_to_player button
		case 5:
			$('#results_list').sfList('blur');
			$('#go_to_player_button').sfButton('focus');
			this.browse_focus = 4;
			break;
			
		default:
			break;
	}
}

/**
 * Function to handle the pressing of the right button based on the focus.
 */
SceneBrowse.prototype.handleRight = function () {
	switch(this.browse_focus) {	
		// When at the search bar, move focus to usb button
		case 0:
			this.blurSearchLabel();
			$('#usb_button').sfButton('focus');
			this.browse_focus = 1;
			break;
			
		// When at the usb button, move focus to the search bar
		case 1:
			$('#usb_button').sfButton('blur');
			this.focusSearchLabel();
			this.browse_focus = 0;
			break;
			
		// When at the add_to_playlist button, move focus to the go_to_player button
		case 2:
			$('#add_to_playlist_button').sfButton('blur');
			$('#go_to_player_button').sfButton('focus');
			this.browse_focus = 4;
			break;
		
		// When at the upload button, move focus to the go_to_player button
		case 3:
			$('#upload_button').sfButton('blur');
			$('#go_to_player_button').sfButton('focus');
			this.browse_focus = 4;
			break;
			
		// When at the go_to_player button, move focus to the search bar
		case 4:
			$('#go_to_player_button').sfButton('blur');
			this.focusSearchLabel();
			this.browse_focus = 0;
			break;
			
		// When at the result list, move focus to the usb button
		case 5:
			$('#results_list').sfList('blur');
			$('#usb_button').sfButton('focus');
			this.browse_focus = 1;
			break;
		
		default:
			break;
	}
}

/**
 * Function called when key is pressed on the remote.
 */
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

/**
 * Function to be called by the timer each 4 seconds to get new search results.
 */
function timedCount() {
	// Function call to get the results
	result();
	
	// Set timer to call this function again in 4 seconds, until 16 second have passed,
	// c <= 4, or when search results are obtained. Then stop the timer.
	c += 1;
	if (c <= 4 && search_results.length < 1)
		t = setTimeout("timedCount()", 4000);
	else
		stopCount();
}

/**
 * Function to start polling for results.
 *  Application is still responsive during the polling.
 */
function startResultPolling() {
	c              = 0;
	search_results = [];
	
	if (!timer_is_on) {
		timer_is_on = 1;
		timedCount();
	}
}

/**
 * Function to stop the timer, and thereby stop polling.
 */
function stopCount() {
	clearTimeout(t);
	timer_is_on = 0;
	$('#search_label').sfLabel("option","text", "Search");
	var my_element = document.getElementById("search_label");
	my_element.style.fontStyle = "normal";
}

/**
 * Function to do an HTTP request to obtain results, after having sent a search request.
 */
function result() {
	requestXml = new XMLHttpRequest();
	requestXml.open("GET", result_url, true);
	requestXml.onreadystatechange = processSearchResponse;
	requestXml.send(null);
}

/**
 * Function that is called when a reponse has been received by the server after
 * sending a result request. Names, trackers and hashes of the result files
 * are stored in arrays. And the result list is filled with the names of the files.
 */
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

/**
 * Function that is used to send general http request to the webserver.
 */
function httpGet(url) {
	requestHttp = new XMLHttpRequest();
	requestHttp.open("GET", url, true);
	requestHttp.onreadystatechange = processRequest;
	requestHttp.send(null);
}

/**
 * Function that is called when a response is received from the server
 * after sending a general http request.
 */
function processRequest() {
	if (requestHttp.readyState == 4) {
		var resultHttp = requestHttp.responseText;
		
		// When a stream request was sent, the stream url is returned
		// Otherwise feedback messages are returned.
		if (resultHttp == -1)
			$('#selection_label').sfLabel("option", "text", 'Request failed');
		else {
			if (resultHttp.substring(0,4) == "http")
				stream = resultHttp;
			else
				$('#selection_label').sfLabel("option", "text", resultHttp);
		}
	}
}

