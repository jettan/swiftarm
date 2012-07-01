function SceneSettings() {
	var index;
	var row;
	var label;
	var textinit;
}

SceneSettings.prototype.initialize = function () {
	alert('SceneSettings.initialize()');
	
	$('#debug_label').sfLabel({text: "Enable debug logging"});
	$('#downspeed_limit_label').sfLabel({text: "Limit down speed"});
	$('#upspeed_limit_label').sfLabel({text: "Limit up speed"});
	$('#ratio_limit_label').sfLabel({text: "Stop seeding after ratio"});
	$('#download_path_label').sfLabel({text: "Download path: "});
	
	$('#upspeed_limit_value').sfLabel({text: "..."});
	$('#downspeed_limit_value').sfLabel({text: "..."});
	$('#ratio_limit_value').sfLabel({text: "..."});
	$('#download_path_value').sfLabel({text: "/dtv/usb/sda1/..."});
	
	$('#browseButton').sfButton({
		text: 'Change path..'
	}).sfButton('addCallback', 'clicked', function (event, index) {
		alert("Button0 clicked: " + index);
	});
	
	$('#debug_toggle').sfToggleButton({
		
	}).sfToggleButton('addCallback', 'changed', function (event, checked) {
		sendSettings();
	});
	
	 $('#upspeed_limit_toggle').sfToggleButton({
		
	}).sfToggleButton('addCallback', 'changed', function (event, checked) {
		if (checked) {
			if (!textinit){
				initText('#upspeed_limit_value');
			}
			$("#input").sfTextInput('option','text',$('#upspeed_limit_value').sfLabel("get").text());
			label = '#upspeed_limit_value';
			$('#input').sfTextInput('show');
			$('#input').sfTextInput('focus');
		}
	});
	
	$('#downspeed_limit_toggle').sfToggleButton({
		
	}).sfToggleButton('addCallback', 'changed', function (event, checked) {
		if (checked) {
			if (!textinit){
				initText('#downspeed_limit_value');
			}
			
			$("#input").sfTextInput('option','text',$('#downspeed_limit_value').sfLabel("get").text());
			label = '#downspeed_limit_value';
			$('#input').sfTextInput('show');
			$('#input').sfTextInput('focus');
		}
	});
	
	$('#ratio_limit_toggle').sfToggleButton({
		
	}).sfToggleButton('addCallback', 'changed', function (event, checked) {
		if (checked) {
			if (!textinit){
				initText('#ratio_limit_value');
			}
			$("#input").sfTextInput('option','text',$('#ratio_limit_value').sfLabel("get").text());
			label = '#ratio_limit_value';
			$('#input').sfTextInput('show');
			$('#input').sfTextInput('focus');
		}
	});
	
	index = new Array('#debug_toggle','#upspeed_limit_toggle','#downspeed_limit_toggle','#ratio_limit_toggle','#browseButton');
	
	var _THIS_ = this;
	$('#popup').sfPopup({
		text: "Please enter a valid number",
		buttons: "OK",
		callback: function(){
			$(index[_THIS_.row]).sfToggleButton('toggle');
		}
	});
	
	textinit = false;
	
	this.row = 0;
}

function initText(text) {
	label = text;
	$('#input').sfTextInput({
		text:'...',
		maxlength:'10',
		oncomplete: function (text) {
			if (text) {
				if(isNaN($("#input").sfTextInput('getText'))) {
					$('#popup').sfPopup('show');
				} else {
					$(label).sfLabel("option", "text" , $("#input").sfTextInput('getText'));
				}
			}
			else {
				$(label).sfLabel("option", "text" , "0");
			}
			$('#input').sfTextInput('hide');
		}
	});
	textinit = true;
	$('#input').sfTextInput('hide');
}

SceneSettings.prototype.handleShow = function () {
	alert('SceneSettings.handleShow()');
}

SceneSettings.prototype.handleHide = function () {
	alert('SceneSettings.handleHide()');
}

SceneSettings.prototype.handleFocus = function () {
	alert('SceneSettings.handleFocus()');
	
	$('#MainBG').sfBackground('option', 'column', 'left');
	$('#MainBG').sfBackground(this.defaultOpts);
	$('#MainBG').sfBackground('option', 'column', 'left');
	
	$('#image').sfImage('show');
	$('#label').sfLabel('show');
	$('#scene_list').sfList('show');
	
	$('#debug_toggle').sfToggleButton('focus');
	this.row = 0
	
	$("#keyhelp_bar").sfKeyHelp({
		'user': 'Help',
		'move':'Move',
		'return': 'Return'
	});
}

SceneSettings.prototype.handleBlur = function () {
	alert('SceneSettings.handleBlur()');
	$('#debug_toggle').sfToggleButton('blur');
	$('#upspeed_limit_toggle').sfToggleButton('blur');
	$('#downspeed_limit_toggle').sfToggleButton('blur');
	$('#ratio_limit_toggle').sfToggleButton('blur');
}

SceneSettings.prototype.handleKeyDown = function (keyCode) {
	alert('SceneSettings.handleKeyDown(' + keyCode + ')');
	switch (keyCode) {
		case sf.key.LEFT:
			sf.scene.focus('Main');
			break;
			
		case sf.key.RIGHT:
			break;
			
		case sf.key.DOWN:
			alert("INDEX!!! : " + this.row);
			if (this.row == 4) {
				$(index[this.row]).sfButton('blur');
			} else {
				$(index[this.row]).sfToggleButton('blur');
			}
			
			this.row = (this.row + 1) % 5;
			
			if (this.row == 4) {
				$(index[this.row]).sfButton('focus');
			} else {
				$(index[this.row]).sfToggleButton('focus');
			}
			break;
			
		case sf.key.UP:
			alert("INDEX!!! : " + this.row);
			if (this.row == 4) {
				$(index[this.row]).sfButton('blur');
			} else {
				$(index[this.row]).sfToggleButton('blur');
			}
			if (this.row == 0) {
				this.row = 4;
				$(index[this.row]).sfButton('focus');
			} else {
				this.row = this.row - 1;
				$(index[this.row]).sfToggleButton('focus');
			}
			break;
			
		case sf.key.ENTER:
			if (this.row == 4) {
				$("#browseButton").sfButton('blur');
				var _THIS_ = this;
				sf.service.USB.show({
					callback: function(result) {
						alert("Callback of USB module: " + result);
						
						var chunks = result[0].split('/');
						var i;
						var path = '/dtv/usb';
						
						for (i = 1; i < chunks.length - 1; i++){
							path = path + '/' + chunks[i];
						}
						
						$('#download_path_value').sfLabel("option", "text", path);
						downloadPath = path;
						//_THIS_.setResult(result?getObjString(result, 'result'):'null');
						$("#browseButton").sfButton('focus');
					},
					fileType: 'all'
				});
				
			} else {
				$(index[this.row]).sfToggleButton('toggle');
			}
			break;
			
		case sf.key.RETURN:
			sf.scene.focus('Main');
			sf.key.preventDefault();
			break;
			
		default:
			break;
	}
}

var xmlHttp;
function sendSettings() {
	xmlHttp = new XMLHttpRequest();
	
	var settings =  "/settings:" + $('#downspeed_limit_value').sfLabel("get").text() +
					":" + $('#upspeed_limit_value').sfLabel("get").text() +
					":" + $('#download_path_value').sfLabel("get").text();
	
	// TODO: build URL dynamically, not hard coded
	// Build the URL to connect to
	var url = "http://145.94.176.101:1337" + settings;
	
	// Open a connection to the server
	xmlHttp.open("GET", url, true);
	
	// Tell the server you're sending it XML
	xmlHttp.setRequestHeader("Content-Type", "text/xml");
	
	// Set up a function for the server to run when it's done
	xmlHttp.onreadystatechange = processSettingsResponse;
	
	// Send the request
	xmlHttp.send(null);
}

function processSettingsResponse() {
	if (xmlHttp.readyState == 4) {
		var settingsResult = xmlHttp.responseText;
		alert(settingsResult);
	}
}

SceneSettings.prototype.refreshButtons = function () {
	alert('SceneSettings.refreshButtons()');
}
