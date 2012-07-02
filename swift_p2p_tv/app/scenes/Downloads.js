function SceneDownloads(options) {
	this.options = options;
	
	this.progress_bar_type = [{
		type: 'status',
		max: 100
	}];
	
	this.size = 5;
}

var stats_url  = tv_url + "/stats";
var pause_url  = tv_url + "/pause:";
var resume_url = tv_url + "/resume:";
var switch_url = tv_url + "/download:";
var stop_url   = tv_url + "/stop:";
var remove_url = tv_url + "/remove:";
var clear_url  = tv_url + "/clear";

var special_request;
var stats_request;

var download_list;
var stats_results;
var element_list      = [];
var focus             = 0;
var count             = 0;
var page_number       = 0;

var download_zero = ["#00_download_size","#00_download_completed", "#00_download_status", "#00_download_name",
                 "#00_download_dspeed", "#00_download_uspeed", "#00_download_num_progress",
                 "#00_download_num_seeders", "#00_download_num_peers", "#00_download_time_remaining",
                 "#00_download_hash"];

var element_zero = download_zero.slice(0);
element_zero.splice(download_zero.length - 1, 0, "#00_download_progress", "#00_download_dspeed_label",
                    "#00_download_uspeed_label", "#00_download_seeders", "#00_download_peers",
                    "#00_download_separator", "#00_download_eta", "#00_download_percentage");

var download_one = ["#01_download_size","#01_download_completed", "#01_download_status", "#01_download_name",
                "#01_download_dspeed", "#01_download_uspeed", "#01_download_num_progress",
                "#01_download_num_seeders", "#01_download_num_peers", "#01_download_time_remaining",
                "#01_download_hash"];

var element_one = download_one.slice(0);
element_one.splice(download_one.length - 1, 0, "#01_download_progress", "#01_download_dspeed_label",
                    "#01_download_uspeed_label", "#01_download_seeders", "#01_download_peers",
                    "#01_download_separator", "#01_download_eta", "#01_download_percentage");

var download_two = ["#02_download_size","#02_download_completed", "#02_download_status", "#02_download_name",
                "#02_download_dspeed", "#02_download_uspeed", "#02_download_num_progress",
                "#02_download_num_seeders", "#02_download_num_peers", "#02_download_time_remaining",
                "#02_download_hash"];

var element_two  = download_two.slice(0);
element_two.splice(download_two.length - 1, 0, "#02_download_progress", "#02_download_dspeed_label",
                    "#02_download_uspeed_label", "#02_download_seeders", "#02_download_peers",
                    "#02_download_separator", "#02_download_eta", "#02_download_percentage");

var download_three = ["#03_download_size","#03_download_completed", "#03_download_status", "#03_download_name",
                  "#03_download_dspeed", "#03_download_uspeed", "#03_download_num_progress",
                  "#03_download_num_seeders", "#03_download_num_peers", "#03_download_time_remaining",
                  "#03_download_hash"];

var element_three  = download_three.slice(0);
element_three.splice(download_three.length - 1, 0, "#03_download_progress", "#03_download_dspeed_label",
                    "#03_download_uspeed_label", "#03_download_seeders", "#03_download_peers",
                    "#03_download_separator", "#03_download_eta", "#03_download_percentage");

var download_four = ["#04_download_size","#04_download_completed", "#04_download_status", "#04_download_name",
                 "#04_download_dspeed", "#04_download_uspeed", "#04_download_num_progress",
                 "#04_download_num_seeders", "#04_download_num_peers", "#04_download_time_remaining",
                 "#04_download_hash"];

var element_four  = download_four.slice(0);
element_four.splice(download_four.length - 1, 0, "#04_download_progress", "#04_download_dspeed_label",
                    "#04_download_uspeed_label", "#04_download_seeders", "#04_download_peers",
                    "#04_download_separator", "#04_download_eta", "#04_download_percentage");

var progress_bar_list = ["#progress_download0", "#progress_download1", "#progress_download2",
                     "#progress_download3", "#progress_download4"];

var lines_list        = ["download_holder0", "download_holder1", "download_holder2",
                     "download_holder3", "download_holder4"];

SceneDownloads.prototype.initialize = function () {
	$('#ratio_label').sfLabel({text: 'Ratio:'});
	$('#total_up_label').sfLabel({text:'up:'});
	$('#init_total_up').sfLabel({text:'0'});
	$('#total_down_label').sfLabel({text:'down:'});
	$('#init_total_down').sfLabel({text:'0'});
	$('#pause_image').sfImage({src:'images/navi/pause.png'});
	
	for (var i = 0; i < 5; i++) {
		$('#0' + i +'_download_name').sfLabel({text:'Name'});
		$('#0' + i +'_download_hash').sfLabel({text:'roothash'});
		$('#0' + i +'_download_completed').sfLabel({text:'0'});
		$('#0' + i +'_download_size').sfLabel({text:'0'});
		$('#0' + i +'_download_progress').sfLabel({text:'('});
		$('#0' + i +'_download_status').sfLabel({text:'n/a'});
		$('#0' + i +'_download_dspeed_label').sfLabel({text:'DL: '});
		$('#0' + i +'_download_uspeed_label').sfLabel({text:'UL: '});
		$('#0' + i +'_download_seeders').sfLabel({text:'seeders: '});
		$('#0' + i +'_download_peers').sfLabel({text:'peers: '});
		$('#0' + i +'_download_separator').sfLabel({text:'/'});
		$('#0' + i +'_download_num_seeders').sfLabel({text:'0'});
		$('#0' + i +'_download_num_peers').sfLabel({text:'0'});
		$('#0' + i +'_download_eta').sfLabel({text:' remaining'});
		$('#0' + i +'_download_time_remaining').sfLabel({text:'0'});
		$('#0' + i +'_download_uspeed').sfLabel({text:'0'});
		$('#0' + i +'_download_dspeed').sfLabel({text:'0'});
		$('#0' + i +'_download_num_progress').sfLabel({text:'0'});
		$('#0' + i +'_download_percentage').sfLabel({text:'%)'});
	}
	
	for (var j = 0; j < 5; j++) {
		$("#progress_download" + j).sfProgressBar(this.progress_bar_type[0]);
	}
	
	download_list     = [download_zero, download_one, download_two, download_three, download_four];
	element_list      = [element_zero, element_one, element_two, element_three, element_four];
}

SceneDownloads.prototype.handleShow = function () {}

SceneDownloads.prototype.handleHide = function () {}

SceneDownloads.prototype.handleFocus = function () {
	$('#app_layout').sfBackground('option', 'column', 'left');
	$('#label').sfLabel('show');
	$('#scene_list').sfList('show');
	
	for (var d = 0; d < 5; d++) {
		blurElement(d);
		hideElement(d);
	}
	focus = 0;
	focusElement(focus);
	
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

SceneDownloads.prototype.handleBlur = function () {}

SceneDownloads.prototype.handleKeyDown = function (key_code) {
	switch (key_code) {
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
		case sf.key.PAUSE:
			$('#00_download_status').sfLabel('hide');
			httpGetSpecial(pause_url + stats_results[focus + page_number * 5][10]);
			$('#pause_image').sfImage('show');
			break;
		case sf.key.PLAY:
			$('#pause_image').sfImage('hide');
			httpGetSpecial(resume_url + stats_results[focus + page_number * 5][10]);
			$('#00_download_status').sfLabel('show');
			break;
		case sf.key.STOP:
			$('#popup_confirmation').sfPopup({
				text:"Are you sure you want to stop the selection?",
				buttons: ["Yes", "No"],
				defaultFocus: 1,
				keyhelp: {'return' : 'Return'},
				callback : function(selectedIndex) {
					 if (selectedIndex == 0) {
						httpGetSpecial(stop_url + stats_results[focus + page_number * 5][10]);
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
		case sf.key.REW:
			$('#popup_confirmation').sfPopup({
				text:"Are you sure you want to remove the selection?",
				buttons: ["Yes", "No"],
				defaultFocus: 1,
				keyhelp: {'return' : 'Return'},
				callback : function(selectedIndex) {
					 if (selectedIndex == 0) {
						httpGetSpecial(remove_url + stats_results[focus + page_number * 5][10]);
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
		case sf.key.RETURN:
			stopProgress();
			sf.scene.focus('Main');
			sf.key.preventDefault();
			break;
		default:
			break;
	}
}

function httpGetSpecial(url) {
	special_request = new XMLHttpRequest();
	special_request.open("GET", url, true);
	special_request.onreadystatechange = processSpecialResponse;
	special_request.send(null);
}

function processSpecialResponse() {
	if(special_request.readyState == 4) {
		var special_result = stats_request.responseText;
		$('#ratio_label').sfLabel("option", "text", "Ratio: " + special_result);
	}
}

function httpGetXML(url) {
	stats_request = new XMLHttpRequest();
	stats_request.open("GET", url, true);
	stats_request.onreadystatechange = processStatsResponse;
	stats_request.send(null);
}

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

var timer;
var timer_on=0;

function getProgress() {
	httpGetXML(stats_url);
	t = setTimeout("getProgress()", 4000);
}

function startProgress() {
	if (!timer_on) {
		timer_on=1;
		getProgress();
	}
}

function stopProgress() {
	clearTimeout(timer);
	timer_on=0;
}

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

function hideElement(index) {
	alert("SceneDownloads.hideElement()");
	var element = element_list[index];
	var i;
	for(i=0; i<element.length; i++)
		$(element[i]).sfLabel('hide');
		
	var myElement = document.getElementById(progress_bar_list[index].substring(1));
	myElement.style.visibility="hidden";
	var myElement = document.getElementById(lines_list[index]);
	myElement.style.visibility="hidden";
}

function showElement(index) {
	alert("SceneDownloads.showElement()");
	var element = element_list[index];
	var i;
	for(i = 0; i < element.length; i++)
		$(element[i]).sfLabel('show');
		
	var myElement = document.getElementById(progress_bar_list[index].substring(1));
	myElement.style.visibility="visible";
	var myElement = document.getElementById(lines_list[index]);
	myElement.style.visibility="visible";
}

function focusElement(index) {
	var myElement = document.getElementById(lines_list[index]);
	myElement.style.borderWidth="2px";
	myElement.style.borderBottomStyle="groove";
	myElement.style.borderBottomColor="#ffffff";
	myElement.style.borderTopStyle="groove";
	myElement.style.borderTopColor="#ffffff";
	myElement.style.borderLeftStyle="groove";
	myElement.style.borderLeftColor="#ffffff";
	myElement.style.borderRightStyle="groove";
	myElement.style.borderRightColor="#ffffff";
}

function blurElement(index) {
	var myElement = document.getElementById(lines_list[index]);
	myElement.style.borderWidth="2px";
	myElement.style.borderBottomStyle="solid";
	myElement.style.borderBottomColor="#000000";
	myElement.style.borderTopStyle="solid";
	myElement.style.borderTopColor="#000000";
	myElement.style.borderLeftStyle="solid";
	myElement.style.borderLeftColor="#000000";
	myElement.style.borderRightStyle="solid";
	myElement.style.borderRightColor="#000000";
}

