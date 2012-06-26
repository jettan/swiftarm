function SceneBrowse() {
    this.row = 0;
	this.column = 0;
	
	// List with available files for download/stream
	// In future release a search function will be included
	var stream = 'Initiele waarde motherf***er';
	var buttons;
	var but = 3;
	var k = 0;
}

var downloadURL = "http://130.161.159.107:1337/add:";
var uploadURL = "http://130.161.159.107:1337/upload:";
var searchURL = "http://130.161.159.107:1337/search:";
var resultURL = "http://130.161.159.107:1337/results";
var streamURL = "http://130.161.159.107:1337/stream";
var stopStreamURL = "http://130.161.159.107:1337/stopStream";

var listShown = false;
var searchResults = ["Search results"];

SceneBrowse.prototype.initialize = function () {
	alert('SceneBrowse.initialize()');
	 
	$('#list0').sfList({
		data: searchResults, 
		index:'0', 
		itemsPerPage:'7'
	});
	$('#list0').sfList('show');
	
	$('#labelList').sfLabel({text:'Filebrowser'});
	$('#labelList').sfLabel('show');
	
	$('#usbButton').sfButton({text:'Browse USB'});
	
	$('#svecInput_Z6V6').sfTextInput({
		text:'sintel', 
		maxlength:'10',
		oncomplete: function (text) {
			/*if (text)
				search(searchURL + $("#svecInput_Z6V6").sfTextInput('getText'));*/
		}
	}).sfTextInput('blur');
	
	$('#svecLabel_EXXY').sfLabel({text:'Search'});
	$('#playerButton').sfButton({text:'Go to player'});
	$('#svecLabel_XKN9').sfLabel({text:'Selected file:'});
	$('#svecLabel_YDEP').sfLabel({text:'...'});
	$('#svecButton_4PT8').sfButton({text:'Add to playlist'});
	
	$('#playerButton').sfButton({text:'Go to player'});
	$('#svecLabel_EXXY').sfLabel({text:'Search'});	
	
	$('#uploadButton').sfButton({text:'Upload file'});
	
	buttons = new Array('#usbButton','#svecButton_4PT8','#uploadButton');
	
}
  

SceneBrowse.prototype.handleShow = function () {
	alert('SceneBrowse.handleShow()');
}

SceneBrowse.prototype.handleHide = function () {
	alert('SceneBrowse.handleHide()');
}

SceneBrowse.prototype.handleFocus = function () {
	alert('SceneBrowse.handleFocus()');
	
	this.row = 0;
	this.column = 1;
	but = 0;
	
	$('#labelList').sfLabel('show');
	$('#list0').sfList('show');
	//$('#list0').sfList('focus');
	$('#usbButton').sfButton('focus');
	alert('-----------===================-------------------');
	alert('They printing functions over here');
	$('#svecInput_Z6V6').sfTextInput('setKeypadPos',0, 200)
	//alert($('#svecInput_Z6V6').sfTextInput('setKeypadPos',250, 200));
	$("#keyhelp_bar").sfKeyHelp({
		'user': 'Help',		
		'move':'Move',
        'return': 'Return'
	});
}

SceneBrowse.prototype.handleBlur = function () {
	alert('SceneBrowse.handleBlur()');
	
	$('#button0').sfButton('blur');
	$('#labelList').sfLabel('hide');
	$('#list0').sfList('blur');
	$('#list0').sfList('hide');
}

SceneBrowse.prototype.handleKeyDown = function (keyCode) {
	alert('SceneBrowse.handleKeyDown(' + keyCode + ')');
	switch (keyCode) {
		case sf.key.LEFT:
			if (this.column == 0) {
				if (this.row == 0)
					$('#svecInput_Z6V6').sfTextInput('blur');
				else if (listShown)
						$('#list0').sfList('blur');
					
				$('#usbButton').sfButton('focus');
				this.column = 1;
				this.row = 0;
			} else if (this.column == 1) {
				if (this.row == 0)
					$('#usbButton').sfButton('blur');
				else if (this.row == 1)
					$('#svecButton_4PT8').sfButton('blur');
				else
					$('#uploadButton').sfButton('blur');
				if (listShown) {
					$('#list0').sfList('focus');
					this.column = 0;
					this.row = 1 + $('#list0').sfList('getIndex');
				}
				else {
						$('#svecInput_Z6V6').sfTextInput('focus');
					this.column = 0;
					this.row = 0;
				}
			} else if (this.column == 2) {
				$('#playerButton').sfButton('blur');
				$('#svecButton_4PT8').sfButton('focus');
				this.column = 1;
				this.row = 1;
			}		
			break;
		case sf.key.RIGHT:
			if (this.column == 0) {
				if (this.row == 0)
					$('#svecInput_Z6V6').sfTextInput('blur');
				else if (listShown)
						$('#list0').sfList('blur');
					
				$('#usbButton').sfButton('focus');
				this.column = 1;
				this.row = 0;
			} else if (this.column == 1) {
				if (this.row == 1 || this.row == 2) {
					$(buttons[this.row]).sfButton('blur');
					$('#playerButton').sfButton('focus');
					this.column = 2;
					break;
				}
				else
					$('#usbButton').sfButton('blur');
					
				if (listShown) {
					$('#list0').sfList('focus');
					this.column = 0;
					this.row = 1 + $('#list0').sfList('getIndex');
				}
				else {
						$('#svecInput_Z6V6').sfTextInput('focus');
					this.column = 0;
					this.row = 0;
				}
			} else if (this.column == 2) {
				$('#playerButton').sfButton('blur');
				if (listShown) {
					$('#list0').sfList('focus');
					this.column = 0;
					this.row = 1 + $('#list0').sfList('getIndex');
				}
				else {
						$('#svecInput_Z6V6').sfTextInput('focus');
					this.column = 0;
					this.row = 0;
				}
			}	
			break;
		case sf.key.UP:
			alert('=======================HIEROOOOOOOOOOOOOOOOOOO=======================');
			alert(searchResults.length);
			if (this.column == 0 && listShown) {
				if (this.row == 0) {
						$('#svecInput_Z6V6').sfTextInput('blur');
						/*searchResults = [];
						for(var y=0; y<names.length; y++)
							searchResults.push(names[y]);*/
						alert('=======================HIEROOOOOOOOOOOOOOOOOOO2=======================');
						alert(searchResults.length);
						$('#list0').sfList('focus');
						this.row = 1 + searchResults.length;
						$('#list0').sfList('move',searchResults.length-1);
				} else if ($('#list0').sfList('getIndex') == 0) {
						$('#list0').sfList('blur');
						$('#svecInput_Z6V6').sfTextInput('focus');
						this.row = 0;
						break;
				} else {
					$('#list0').sfList('prev');
					this.row = this.row - 1;
				}
			} else if (this.column == 1) {
				if (this.row == 0) {
					$('#usbButton').sfButton('blur');
					$('#uploadButton').sfButton('focus');
					this.row = 2;
					break;
				} else if (this.row == 1) {
					$('#svecButton_4PT8').sfButton('blur');
					$('#usbButton').sfButton('focus');
					this.row = 0;
					break;
				} else {
					$('#uploadButton').sfButton('blur');
					$('#svecButton_4PT8').sfButton('focus');
					this.row = 1;
					break;
				}
			} 
			break;
		case sf.key.DOWN:
			alert('=======================HIEROOOOOOOOOOOOOOOOOOO=======================');
			alert(searchResults.length);
			if (this.column == 0 && listShown) {
				if (this.row == 0) {
						$('#svecInput_Z6V6').sfTextInput('blur');
						/*searchResults = [];
						for(var y=0; y<names.length; y++)
							searchResults.push(names[y]);*/
						alert('=======================HIEROOOOOOOOOOOOOOOOOOO2=======================');
						alert(searchResults.length);
						$('#list0').sfList('focus');
						this.row = 1;
						$('#list0').sfList('move',0);
				} else if ($('#list0').sfList('getIndex') == searchResults.length - 1) {
						$('#list0').sfList('blur');
						alert(searchResults.length);
						$('#svecInput_Z6V6').sfTextInput('focus');
						this.row = 0;
						break;
				} else {
					$('#list0').sfList('next');
					this.row = this.row + 1;
				}
			} else if (this.column == 1) {
				if (this.row == 0) {
					$('#usbButton').sfButton('blur');
					$('#svecButton_4PT8').sfButton('focus');
					this.row = 1;
					break;
				} else if (this.row == 1){
					$('#svecButton_4PT8').sfButton('blur');
					$('#uploadButton').sfButton('focus');
					this.row = 2;
					break;
				} else {
					$('#uploadButton').sfButton('blur');
					$('#usbButton').sfButton('focus');
					this.row = 0;
					break;
				}
			} 
			break;
		case sf.key.ENTER:
		if (this.column == 0 && this.row == 0) {
			httpGet(searchURL + $("#svecInput_Z6V6").sfTextInput('getText'));
			startResultPolling();
		} else if (this.column == 0 && listShown && this.row > 0) {
			var index = $('#list0').sfList('getIndex');
			$('#svecLabel_YDEP').sfLabel("option", "text", searchResults[index]);
			var _THIS_ = this;
			$('#popupUD').sfPopup({
					text:"Do you want to download or stream this file?",
					buttons: ["Download", "Stream"],
					defaultFocus: 1,
					keyhelp: {'return' : 'Return'},
					callback : function(selectedIndex){
						if (selectedIndex == -1)
							$('#labelUSB').sfLabel("option", "text", 'Operation canceled');
						else if (selectedIndex == 0) {
							// Start download
							httpGet(downloadURL + hashes[index]);
							//downloads.push(this.searchResults[index]);
							downloading = true;
							$('#is_downloading').sfLabel('option','text','Downloading');
						} else {
							//Start streaming and redirect to player
							httpGet(streamURL + ":" + hashes[index]);
							alert("!dadada: " + stream);
							alert("!lalala: " + $('#label_video').sfLabel("get").text());
							var vid = stream;						
							playlist.push({
								url: vid,
								title: 'Stream'
							});
							
							//$('#svecLabel_YDEP').sfLabel("option","text",$('#label_video').sfLabel("get").text());
							$('#label_redirect').sfLabel('option','text','Player');
							sf.scene.focus('Main');
						}
					}
				}).sfPopup('show');
		} else if (this.row == 0 && this.column == 1) {
			$("#usbButton").sfButton('blur');
			var _THIS_ = this;
			sf.service.USB.show({
				callback: function(result){
					alert("Callback of USB module: " + result);
					 $('#label_video').sfLabel("option", "text", result[0]); 
					 $('#svecLabel_YDEP').sfLabel("option", "text", result[0]); 
					but = 1;
					$("#playerButton").sfButton('focus');
				},
				fileType: 'all'
			});
			this.column = 2;
			this.row = 1;
		} else if (this.row == 1 && this.column == 1) {
			// Add to selection to playlist(Check for correct path) 
			//$('#svecLabel_YDEP').sfLabel("option","text","You are absolutely motherfucking right!");
			
			var vid = $('#label_video').sfLabel("get").text();
			//var vid = '$Usb/sda1/lalala.mkv';
			vid = 'file:///dtv/usb' + vid.substring(8);
			var n = vid.split("/");
			var nlength = n.length - 1;
			
			playlist.push({
				url: vid,
				title: n[nlength]
			});
			
			//playlist = playlist.splice(4,5);
			$('#svecLabel_YDEP').sfLabel("option","text",playlist.length);
		
		} else if (this.row == 2 && this.column == 1){
			// Upload selection via swift(Check for correct path)
			if($('#labelUSB').sfLabel("get").text()) {
				var vid = $('#labelUSB').sfLabel("get").text()
				vid = 'file:///dtv/usb' + link.substring(8);
				httpGet(uploadUrl + vid);
			}
			
		} else if (this.row == 1 && this.column == 2) {
			$('#label_redirect').sfLabel('option','text','Player');
			$("#playerButton").sfButton('blur');
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
		case sf.key.RED:
			searchResults = ['1','2','3','4','5'];
			break;
		case sf.key.GREEN:
			$('#svecLabel_YDEP').sfLabel("option", "text", stream);
			break;
		case sf.key.YELLOW:
			$('#svecLabel_YDEP').sfLabel("option", "text", $('#label_video').sfLabel("get").text());
			break;
		case sf.key.BLUE:
			break;
	}
}

var c=0;
var t;
var timer_is_on=0;

function timedCount()
{
	result();
	$('#svecLabel_YDEP').sfLabel("option", "text", "C =" + c);
	c+=5;
	if(c <= 15)
		t=setTimeout("timedCount()",5000);
	else
		stopCount();
}

function startResultPolling()
{
	c = 0;
	if (!timer_is_on) {
		timer_is_on=1;
		timedCount();
	}
}

function stopCount()
{
	clearTimeout(t);
	timer_is_on=0;
}

var requestXml
function result() {
	requestXml = new XMLHttpRequest();
	requestXml.open("GET", resultURL, true);
	requestXml.onreadystatechange = processSearchResponse;
	requestXml.send(null);
}

function processSearchResponse() {
	alert('=====================READYSTATE ===============' + requestXml.readyState);
	if (requestXml.readyState == 4) {
		var resultXml = requestXml.responseXML;
		
		alert('GOT XML RESPONSE!');
		alert(resultXml);
		alert('--------------------END XML RESPONSE------------------');
		k = 0;
		$('RESULT',resultXml).each(function(i) {
			names[k] = $(this).find("NAME").text();
			trackers[k] = $(this).find("TRACKER").text();
			hashes[k] = $(this).find("HASH").text();
			k++;
		});
		
		searchResults = [];
		
		for(var j=0; j<names.length; j++)
			searchResults.push(names[j]);
		
		$('#list0').sfList('clear');
		$('#list0').sfList('option', 'data', searchResults);//.sfList('focus');
		
		alert('=========LALALALALALLAL===========');
		alert(searchResults.length);
		listShown = true;
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
			$('#labelUSB').sfLabel("option", "text", 'Request failed');
		else {
			if (resultHttp == 1)
				$('#labelUSB').sfLabel("option", "text", 'Download started');
			else
				$('#label_video').sfLabel('option','text',resultHttp);
		}
	}	
}