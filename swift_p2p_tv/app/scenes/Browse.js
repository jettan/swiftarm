function SceneBrowse() {
    this.row = 0;
	this.column = 0;
	var buttons;
	var but = 3;
}

var downloadURL = "http://130.161.159.107:1337/add:012b5549e2622ea8bf3d694b4f55c959539ac848";
var progressURL = "http://130.161.159.107:1337/progress";
var searchURL = "http://130.161.159.107:1337/search:bla";
var streamURL = "http://130.161.159.107:1337/stream";
var stopStreamURL = "http://130.161.159.107:1337/stopStream";

SceneBrowse.prototype.initialize = function () {
	alert('SceneBrowse.initialize()');
	
	// List with available files for download/stream
	// In future release a search function will be included
	this.data = [
		'stream.mp4',
		'bla.mp4',
		'3',
		'4',
		'5',
		'6',
		'7'
	];
    
	$('#list0').sfList({
		data: this.data,
		index: 0,
		itemsPerPage: 7
	});
	
	$('#labelList').sfLabel({
		text: 'Available files for stream/download'
	});
	$('#labelList').sfLabel('show');
	
	$('#usbButton').sfButton({
        text: 'Browse USB'
    }).sfButton('addCallback', 'clicked', function (event, index) {
		alert("USB button clicked: " + index);
	});
	
	$('#playerButton').sfButton({
        text: 'Go to player'
    }).sfButton('addCallback', 'clicked', function (event, index) {
		alert("Button3 clicked: " + index);
	});
	$('#playerButton').sfButton('hide');	
	
	buttons = new Array('#usbButton','#playerButton');
	
    $('#labelUSB').sfLabel({text: "Click button to browse usb device"});
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
	this.column = 0;
	but = 0;
	
	$('#labelList').sfLabel('show');
	$('#list0').sfList('show');
	$('#list0').sfList('focus');
	
	$("#Main_keyhelp").sfKeyHelp({
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
		case sf.key.RIGHT:
			if (this.column == 0) {
				$('#list0').sfList('blur');
				$('#usbButton').sfButton('focus');
			} else {
				$(buttons[this.row]).sfButton('blur');
				$('#list0').sfList('focus');
			}
			this.column = (this.column == 0 ? 1 : 0);
			break;
		case sf.key.UP:
			if (this.column == 0)
				$('#list0').sfList('prev');
			else {
				$(buttons[this.row]).sfButton('blur');  
				if (but == 1)
					this.row = (this.row ==0 ? 1 : 0);
				$(buttons[this.row]).sfButton('focus');  
			}
			break;
		case sf.key.DOWN:
			if (this.column == 0)
				$('#list0').sfList('next');
			else {
				$(buttons[this.row]).sfButton('blur'); 
				if (but == 1)
					this.row = (this.row ==0 ? 1 : 0);
				$(buttons[this.row]).sfButton('focus'); 
			}
			break;
		case sf.key.ENTER:
		if (this.column == 0) {
			var index = $('#list0').sfList('getIndex');
			$('#labelUSB').sfLabel("option", "text", 'Selected file: ' + this.data[index]);
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
							httpGet(downloadURL);
							downloads.push(this.data[index]);
							$('#labelDownloading').sfLabel('option','text','Downloading');
						} else {
							//Start streaming and redirect to player
							httpGet(streamURL + ":" + _THIS_.data[index]);
							$('#labelRedirect').sfLabel('option','text','PlayerStream');
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
					 $('#labelVideo').sfLabel("option", "text", result[0]); 
					 $('#labelUSB').sfLabel("option", "text", 'Selected file: ' + result[0]); 
					//_THIS_.setResult(result?getObjString(result, 'result'):'null');
					but = 1;
					$('#playerButton').sfButton('show');
					$("#playerButton").sfButton('focus');
				},
				fileType: 'all'
			});
			this.row = 3;
		} else if (this.row == 1 && this.column == 1) {
			$('#labelRedirect').sfLabel('option','text','Player');
			sf.scene.focus('Main');
		}
			break;
		case sf.key.RETURN:
			$('#category').sfList('show');
			$('#image').sfImage('show');
			$('#label').sfLabel('show');
			sf.scene.focus('Main');
            sf.key.preventDefault();
			break;
		case sf.key.RED:
			httpGet(searchURL);
			break;
		case sf.key.GREEN:			
			break;
		case sf.key.YELLOW:            
			break;
		case sf.key.BLUE:
			break;
	}
}

function httpGet(url) {
	request = new XMLHttpRequest();
	request.open("GET", url, true);
	request.onreadystatechange = processRequest;
	request.send(null);
}

function processRequest() {
	if (request.readyState == 4) {
		var result = request.responseText;
		$('#labelUSB').sfLabel("option", "text", result);
		$('#labelVideo').sfLabel("option","text",result);
		stream = result;
		// Url is returned in response when streaming, otherwise a number indicating fail or success
		if (result == -1)
			$('#labelUSB').sfLabel("option", "text", 'Request failed');
		else {
			if (result == 1)
				$('#labelUSB').sfLabel("option", "text", 'Download started');
			else
				$('#labelVideo').sfLabel('option','text',result);
		}
	}	
}
