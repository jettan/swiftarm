/**
 * Constructor of Downloads scene.
 */
function SceneDownloads(options) {
	this.options = options;
	
	this.progress_bar_type = [{
		type: 'status',
		max: 100
	}];
	
	this.size = 5;
}


var special_request;
var stats_request;

var timer;
var timer_on=0;

var download_list;
var stats_results;
var element_list      = [];
var focus             = 0;
var count             = 0;
var page_number       = 0;

// Custom made download list
var download_zero = ["#download_00_size","#download_00_completed", "#download_00_status", "#download_00_name",
                     "#download_00_dspeed", "#download_00_uspeed", "#download_00_num_progress",
                     "#download_00_num_seeders", "#download_00_num_peers", "#download_00_time_remaining",
                     "#download_00_hash"];

var element_zero = download_zero.slice(0);
element_zero.splice(download_zero.length - 1, 0, "#download_00_progress", "#download_00_dspeed_label",
                            "#download_00_uspeed_label", "#download_00_seeders", "#download_00_peers",
                            "#download_00_separator", "#download_00_eta", "#download_00_percentage");

var download_one = ["#download_01_size","#download_01_completed", "#download_01_status", "#download_01_name",
                    "#download_01_dspeed", "#download_01_uspeed", "#download_01_num_progress",
                    "#download_01_num_seeders", "#download_01_num_peers", "#download_01_time_remaining",
                    "#download_01_hash"];

var element_one = download_one.slice(0);
element_one.splice(download_one.length - 1, 0, "#download_01_progress", "#download_01_dspeed_label",
                          "#download_01_uspeed_label", "#download_01_seeders", "#download_01_peers",
                          "#download_01_separator", "#download_01_eta", "#download_01_percentage");

var download_two = ["#download_02_size","#download_02_completed", "#download_02_status", "#download_02_name",
                    "#download_02_dspeed", "#download_02_uspeed", "#download_02_num_progress",
                    "#download_02_num_seeders", "#download_02_num_peers", "#download_02_time_remaining",
                    "#download_02_hash"];

var element_two  = download_two.slice(0);
element_two.splice(download_two.length - 1, 0, "#download_02_progress", "#download_02_dspeed_label",
                          "#download_02_uspeed_label", "#download_02_seeders", "#download_02_peers",
                          "#download_02_separator", "#download_02_eta", "#download_02_percentage");

var download_three = ["#download_03_size","#download_03_completed", "#download_03_status", "#download_03_name",
                      "#download_03_dspeed", "#download_03_uspeed", "#download_03_num_progress",
                      "#download_03_num_seeders", "#download_03_num_peers", "#download_03_time_remaining",
                      "#download_03_hash"];

var element_three  = download_three.slice(0);
element_three.splice(download_three.length - 1, 0, "#download_03_progress", "#download_03_dspeed_label",
                              "#download_03_uspeed_label", "#download_03_seeders", "#download_03_peers",
                              "#download_03_separator", "#download_03_eta", "#download_03_percentage");

var download_four = ["#download_04_size","#download_04_completed", "#download_04_status", "#download_04_name",
                     "#download_04_dspeed", "#download_04_uspeed", "#download_04_num_progress",
                     "#download_04_num_seeders", "#download_04_num_peers", "#download_04_time_remaining",
                     "#download_04_hash"];

var element_four  = download_four.slice(0);
element_four.splice(download_four.length - 1, 0, "#download_04_progress", "#download_04_dspeed_label",
                            "#download_04_uspeed_label", "#download_04_seeders", "#download_04_peers",
                             "#download_04_separator", "#download_04_eta", "#download_04_percentage");

var progress_bar_list = ["#progress_download0", "#progress_download1", "#progress_download2",
                         "#progress_download3", "#progress_download4"];

var lines_list        = ["download_holder0", "download_holder1", "download_holder2",
                         "download_holder3", "download_holder4"];

/**
 * Function called at scene init.
 */
SceneDownloads.prototype.initialize = function () {
	$('#ratio_label').sfLabel({text: 'Ratio:'});
	$('#total_up_label').sfLabel({text:'up:'});
	$('#init_total_up').sfLabel({text:'0'});
	$('#total_down_label').sfLabel({text:'down:'});
	$('#init_total_down').sfLabel({text:'0'});
	
	// Images to be shown when a download is paused
	$('#pause_image0').sfImage({src:'images/navi/pause.png'});
	$('#pause_image1').sfImage({src:'images/navi/pause.png'});
	$('#pause_image2').sfImage({src:'images/navi/pause.png'});
	$('#pause_image3').sfImage({src:'images/navi/pause.png'});
	$('#pause_image4').sfImage({src:'images/navi/pause.png'});
	
	// Labels used to visualize the downloads
	for (var i = 0; i < 5; i++) {
		$('#download_0' + i +'_name').sfLabel({text:'Name'});
		$('#download_0' + i +'_hash').sfLabel({text:'roothash'});
		$('#download_0' + i +'_completed').sfLabel({text:'0'});
		$('#download_0' + i +'_size').sfLabel({text:'0'});
		$('#download_0' + i +'_progress').sfLabel({text:'('});
		$('#download_0' + i +'_status').sfLabel({text:'n/a'});
		$('#download_0' + i +'_dspeed_label').sfLabel({text:'DL: '});
		$('#download_0' + i +'_uspeed_label').sfLabel({text:'UL: '});
		$('#download_0' + i +'_seeders').sfLabel({text:'seeders: '});
		$('#download_0' + i +'_peers').sfLabel({text:'peers: '});
		$('#download_0' + i +'_separator').sfLabel({text:'/'});
		$('#download_0' + i +'_num_seeders').sfLabel({text:'0'});
		$('#download_0' + i +'_num_peers').sfLabel({text:'0'});
		$('#download_0' + i +'_eta').sfLabel({text:' remaining'});
		$('#download_0' + i +'_time_remaining').sfLabel({text:'0'});
		$('#download_0' + i +'_uspeed').sfLabel({text:'0'});
		$('#download_0' + i +'_dspeed').sfLabel({text:'0'});
		$('#download_0' + i +'_num_progress').sfLabel({text:'0'});
		$('#download_0' + i +'_percentage').sfLabel({text:'%)'});
	}
	
	// Progress bars
	for (var j = 0; j < 5; j++) {
		$("#progress_download" + j).sfProgressBar(this.progress_bar_type[0]);
	}
	
	// List for all dynamic elements
	download_list     = [download_zero, download_one, download_two, download_three, download_four];
	// List for all elements dynamic + static
	element_list      = [element_zero, element_one, element_two, element_three, element_four];
}

/**
 * Function called at scene show.
 */
SceneDownloads.prototype.handleShow = function () {}

/**
 * Function called at scene hide.
 */
SceneDownloads.prototype.handleHide = function () {}

/**
 * Function called at scene focus.
 */
SceneDownloads.prototype.handleFocus = function () {
	$('#app_layout').sfBackground('option', 'column', 'left');
	$('#label').sfLabel('show');
	$('#scene_list').sfList('show');
	
	// Hide all download containers
	for (var d = 0; d < 5; d++) {
		blurElement(d);
		hideElement(d);
	}
	focus = 0;
	focusElement(focus);
	
	// Start polling for download stats
	startProgress();
	
	$("#keyhelp_bar").sfKeyHelp({
		'move':'Move',
		'return': 'Return',
		'pause': 'Pause download/upload',
		'play': 'Start/Resume download',
		'stop': 'Stop download',
		'rew' : 'Remove download'
		
	});
}

/**
 * Function called at scene blur.
 */
SceneDownloads.prototype.handleBlur = function () {}

/**
 * Function called when remote key is pressed.
 */
SceneDownloads.prototype.handleKeyDown = function (key_code) {
	switch (key_code) {
		// Scroll through downloads/uploads
		case sf.key.UP:
			if (focus == 0 && page_number == 0)
				break;
				
			blurElement(focus);
			if (focus == 0 && page_number > 0) {
				focus = 4;
				prevPage();
			} else {
				focus = focus - 1;
				focusElement(focus);
			}
			break;
			
		// Scroll through downloads/uploads
		case sf.key.DOWN:
			if (focus == (stats_results.length -1) % 5 && page_number == Math.floor((stats_results.length - 1) / 5))
				break;
				
			blurElement(focus);
			if (focus == 4) {
				nextPage();
			} else {
				focus += 1;
				focusElement(focus);
			}
			break;
			
		// Pause a download/upload
		case sf.key.PAUSE:
			$('#download_0'+ focus + '_status').sfLabel('hide');
			httpGetSpecial(pause_url + stats_results[focus + page_number * 5][10]);
			var myElement = document.getElementById("pause_image" + focus);
			$('#pause_image' + focus).sfImage('show');
			break;
			
		// Resume/Start a download/upload
		case sf.key.PLAY:
			$('#pause_image' + focus).sfImage('hide');
			httpGetSpecial(resume_url + stats_results[focus + page_number * 5][10]);
			$('#download_0'+ focus +'_status').sfLabel('show');
			break;
			
		// Stop a download, and remove it from the list
		case sf.key.STOP:
			$('#popup_confirmation').sfPopup({
				text:"Are you sure you want to stop the selection?",
				buttons: ["Yes", "No"],
				defaultFocus: 1,
				keyhelp: {'return' : 'Return'},
				callback : function(selectedIndex) {
					 if (selectedIndex == 0) {
						httpGetSpecial(stop_url + stats_results[focus + page_number * 5][10]);
						$('#pause_image'+ focus).sfImage('hide');
						if (focus == 0 && page_number > 0){
							prevPage();
						} else if (focus > 0) {
							focus -= 1;
							focusElement(focus);
						}
					}
				}
			}).sfPopup('show');
			break;
			
		// Remove a download from the list, and the filesystem
		case sf.key.REW:
			$('#popup_confirmation').sfPopup({
				text:"Are you sure you want to remove the selection?",
				buttons: ["Yes", "No"],
				defaultFocus: 1,
				keyhelp: {'return' : 'Return'},
				callback : function(selectedIndex) {
					 if (selectedIndex == 0) {
						httpGetSpecial(remove_url + stats_results[focus + page_number * 5][10]);
						$('#pause_image'+ focus).sfImage('hide');
						if (focus == 0 && page_number > 0){
							prevPage();
						} else if (focus > 0) {
							focus -= 1;
							focusElement(focus);
						}
					}
				}
			}).sfPopup('show');
			break;
			
		// Add a download to playlist, and redirect to player if download complete
		case sf.key.ENTER:
		if (stats_results.length > 0 ) {
				var _THIS_ = this;
				$('#player_popup').sfPopup({
					text:"Do you want to go to player to play this video?",
					buttons: ["Yes", "No"],
					defaultFocus: 1,
					keyhelp: {'return' : 'Return'},
					callback : function(selectedIndex) {
						if (selectedIndex == 0 && stats_results[focus + page_number*5][6]==100) {
							// Redirect to player
							playlist.push({
								url: "file://" + download_path + '/' + stats_results[focus + page_number*5][3],
								title: stats_results[focus + page_number*5][3]
							});
							redirect = true;
							stopProgress();
							sf.scene.focus('Main');
						}
					}
				}).sfPopup('show');
			}
			break;
		case sf.key.RETURN:
			stopProgress();
			sf.scene.focus('Main');
			sf.key.preventDefault();
			break;
		default:
			break;
	}
}

/**
 * Function to send the special http request to server like
 * pause, remove , resume.
 */
function httpGetSpecial(url) {
	special_request = new XMLHttpRequest();
	special_request.open("GET", url, true);
	special_request.onreadystatechange = processSpecialResponse;
	special_request.send(null);
}

/**
 * Function is called when http server sends a response to the special request above
 */
function processSpecialResponse() {
	if(special_request.readyState == 4) {
		var special_result = stats_request.responseText;
		$('#ratio_label').sfLabel("option", "text", "Ratio: " + special_result);
	}
}

/**
 * Function is called send http request to server for download stats
 */
function httpGetXML(url) {
	stats_request = new XMLHttpRequest();
	stats_request.open("GET", url, true);
	stats_request.onreadystatechange = processStatsResponse;
	stats_request.send(null);
}

/**
 * Function is called when http server sends a response with the download stats.
 * The download stats are loaded into the custom made containers.
 */
function processStatsResponse() {
	if (stats_request.readyState == 4) {
		var result = stats_request.responseXML;
		stats_results = [];
		count = 0;
		var _THIS_ = this;
		$('DOWNLOAD',result).each(function(i) {
			stats_results[count] = [];
			stats_results[count].push($(this).find("SIZE").text());
			stats_results[count].push($(this).find("COMPLETED").text());
			stats_results[count].push($(this).find("STATUS").text());
			stats_results[count].push($(this).find("NAME").text());
			stats_results[count].push($(this).find("DSPEED").text());
			stats_results[count].push($(this).find("USPEED").text());
			stats_results[count].push($(this).find("PROGRESS").text());
			stats_results[count].push($(this).find("SEEDERS").text());
			stats_results[count].push($(this).find("PEERS").text());
			stats_results[count].push($(this).find("TIMEMINUTES").text());
			stats_results[count].push($(this).find("HASH").text());
			if (count == 0) {
				$('#init_total_up').sfLabel("option","text",$(this).find("UPLOADAMOUNT").text());
				$('#init_total_down').sfLabel("option","text",$(this).find("DOWNLOADAMOUNT").text());
				$('#ratio_label').sfLabel("option","text","Ratio: " + $(this).find("RATIO").text());
			}
			count++;
		});
		var limit = stats_results.length - page_number * 5;
		var d;
		for(d = 0; d < limit; d++) {
			var counter;
			for(counter = 0; counter < 11; counter++)
				$(download_list[d][counter]).sfLabel("option","text",stats_results[d + page_number * 5][counter]);
			
			$(progress_bar_list[d]).sfProgressBar('setValue', stats_results[d + page_number * 5][6]);
			
			showElement(d);
		}
		var h;
		for (h = d; h < 5; h++)
			hideElement(h);
		
	}
}

/**
 * Function that is called by the timer each 4 seconds to get new download stats
 */
function getProgress() {
	httpGetXML(stats_url);
	timer = setTimeout("getProgress()", 4000);
}

/**
 * Function that start the timer to keep polling each 4 seconds for download stats
 */
function startProgress() {
	if (!timer_on) {
		timer_on=1;
		getProgress();
	}
}

/**
 * Function stops the timer, and therefore stops polling for download stats
 */
function stopProgress() {
	clearTimeout(timer);
	timer_on=0;
}

/**
 * Function reloads the download containers with the next x downloads.
 */
function nextPage() {
	if (page_number < Math.floor((stats_results.length - 1) / 5)) {
		page_number++;
		switchPage();
		focus = 0;
		focusElement(focus);
	} else if (page_number == Math.floor(stats_results.length - 1 / 5)) {
		var numelements = stats_results.length % 5;
		var hidecounter;
		for (hidecounter = numelements; hidecounter < 5; hidecounter++)
			hideElement(hidecounter);
		switchPage();
		focus = 0;
		focusElement(focus);
	}
}

/**
 * Function reloads the download containers with the previous x downloads.
 */
function prevPage() {
	if (page_number == Math.floor((stats_results.length - 1) / 5)) {
			var showcounter = 0;
			for (showcounter = 0; showcounter < 5; showcounter++)
				showElement(showcounter);
	}
	
	if(page_number > 0) {
		page_number--;
		switchPage();
		focus = 4;
		focusElement(focus);
	}
}

/**
 * Function is called by previous 2 functions to do the actual reloading.
 */
function switchPage() {
	var end = 5;
	if (page_number == Math.floor((stats_results.length - 1) / 5) && (stats_results.length % 5) > 0)
		end = stats_results.length % 5;
		
	var start = page_number * 5;
	for (var j = 0; j < end; j++) {
		var counter;
		for (counter = 0; counter < 11; counter++)
			$(download_list[j][counter]).sfLabel("option", "text", stats_results[j + start][counter]);
		$(progress_bar_list[j]).sfProgressBar('setValue', stats_results[j+start][6]);
	}
}

/**
 * Function hides a download container
 */
function hideElement(index) {
	var element = element_list[index];
	var i;
	for(i=0; i<element.length; i++)
		$(element[i]).sfLabel('hide');
		
	var myElement = document.getElementById(progress_bar_list[index].substring(1));
	myElement.style.visibility="hidden";
	var myElement = document.getElementById(lines_list[index]);
	myElement.style.visibility="hidden";
}

/**
 * Function shows a download container
 */
function showElement(index) {
	var element = element_list[index];
	var i;
	for(i = 0; i < element.length; i++)
		$(element[i]).sfLabel('show');
		
	var myElement = document.getElementById(progress_bar_list[index].substring(1));
	myElement.style.visibility="visible";
	var myElement = document.getElementById(lines_list[index]);
	myElement.style.visibility="visible";
}

/**
 * Function moves the focus to current download container.
 */
function focusElement(index) {
	if (index > 0) {
		var my_element = document.getElementById(lines_list[index - 1]);
		my_element.style.borderBottomWidth = "0px";
	}
	
	for (var j = 0; j < 5; j++) {
		if (j != index) {
			var my_element = document.getElementById(lines_list[j]);
			my_element.style.width = "692px";
		}
	}
		
	var my_element = document.getElementById(lines_list[index]);
	my_element.style.borderWidth="4px";
	my_element.style.borderStyle="ridge";
	my_element.style.borderColor="#03fff7";
	
	if (index < 4) {
		var my_element = document.getElementById(lines_list[index + 1]);
		my_element.style.borderTopWidth = "0px";
	}
}

/**
 * Function blurs the current download container.
 */
function blurElement(index) {
	if (index > 0) {
		var my_element = document.getElementById(lines_list[index - 1]);
		my_element.style.borderBottomWidth = "2px";
	}
	
	for (var j = 0; j < 5; j++) {
		if (j != index) {
			var my_element = document.getElementById(lines_list[j]);
			my_element.style.width = "690px";
		}
	}
		
	var my_element = document.getElementById(lines_list[index]);
	my_element.style.borderWidth="2px";
	my_element.style.borderStyle="groove";
	my_element.style.borderColor="#e6e6fa";
	
	if (index < 4) {
		var my_element = document.getElementById(lines_list[index + 1]);
		my_element.style.borderTopWidth = "2px";
	}
	var myElement = document.getElementById(lines_list[index]);
}

