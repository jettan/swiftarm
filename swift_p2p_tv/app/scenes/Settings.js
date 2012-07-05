/**
 * Constructor of the Settings scene.
 */
function SceneSettings() {
	var index;
	var row;
	var label;
	var textinit;
	var settingsResult;
}

/**
 * Function called at scene init.
 */
SceneSettings.prototype.initialize = function () {
	$('#downspeed_limit_label').sfLabel({text: "Limit down speed"});
	$('#upspeed_limit_label').sfLabel({text: "Limit up speed"});
	$('#download_path_label').sfLabel({text: "Download path: "});
	
	$('#upspeed_limit_value').sfLabel({text: "..."});
	$('#downspeed_limit_value').sfLabel({text: "..."});
	$('#download_path_value').sfLabel({text: "/dtv/usb/sda1/..."});
	
	$('#browseButton').sfButton({
			text: 'Change path..'
	});
	
	// Toggle button limits the upspeed and sends the new settings to
	// the c++ backend via a http request
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
			} else {
					$('#upspeed_limit_value').sfLabel("option","text","0");
					var settings_url = tv_url + "/settings:" + $('#upspeed_limit_value').sfLabel("get").text() + ":" +
											  + $('#downspeed_limit_value').sfLabel("get").text() + ":" +
											  + $('#download_path_value').sfLabel("get").text();
					sendHttp(settings_url);
			}
	});
	
	// Toggle button limits the downspeed and sends the new settings to
	// the c++ backend via a http request
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
			} else {
					$('#downspeed_limit_value').sfLabel("option","text","0");
					var settings_url = tv_url + "/settings:" + $('#upspeed_limit_value').sfLabel("get").text() + ":" +
											  + $('#downspeed_limit_value').sfLabel("get").text() + ":" +
											  + $('#download_path_value').sfLabel("get").text();
					sendHttp(settings_url);
			}
	});
	
	index = new Array('#upspeed_limit_toggle','#downspeed_limit_toggle','#browseButton');
	
	var _THIS_ = this;
	$('#popup').sfPopup({
			text: "Please enter a valid number",
			buttons: "OK",
			callback: function(){
			$(index[_THIS_.row]).sfToggleButton('toggle');
		}
	});
	
	textinit = false;
	settingsResult = "";
	
	this.row = 0;
}

/**
 * Function to initialize the textfield.
 */
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
					var settings_url = tv_url + "/settings:" + $('#upspeed_limit_value').sfLabel("get").text() + ":" +
											  + $('#downspeed_limit_value').sfLabel("get").text() + ":" +
											  + $('#download_path_value').sfLabel("get").text();
					sendHttp(settings_url);
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

/**
 * Function called at scene show.
 */
SceneSettings.prototype.handleShow = function () {}

/**
 * Function called at scene hide.
 */
SceneSettings.prototype.handleHide = function () {}

/**
 * Function called at scene focus.
 */
SceneSettings.prototype.handleFocus = function () {
	
	$('#app_layout').sfBackground('option', 'column', 'left');
	
	$('#image').sfImage('show');
	$('#label').sfLabel('show');
	$('#scene_list').sfList('show');
	
	$('#upspeed_limit_toggle').sfToggleButton('focus');
	this.row = 0
	
	// Get current settings from C++ backend
	var url = tv_url + "/settings";
	sendHttp(url);
	setTimeout(function() {
		var res = settingsResult.split(":");
	
		$('#upspeed_limit_value').sfLabel("option","text",res[0]);
		$('#downspeed_limit_value').sfLabel("option","text",res[1]);
		$('#download_path_value').sfLabel("option","text",res[2]);
	}, 1500);
	
	$("#keyhelp_bar").sfKeyHelp({
		'move':'Move',
		'return': 'Return'
	});
}

/**
 * Function called at scene blur.
 */
SceneSettings.prototype.handleBlur = function () {
	$('#upspeed_limit_toggle').sfToggleButton('blur');
	$('#downspeed_limit_toggle').sfToggleButton('blur');
}

/**
 * Function called when remote key is pressed.
 */
SceneSettings.prototype.handleKeyDown = function (keyCode) {

	switch (keyCode) {
		case sf.key.LEFT:
			sf.scene.focus('Main');
			break;
			
		case sf.key.DOWN:
			if (this.row == 2) {
				$(index[this.row]).sfButton('blur');
			} else {
				$(index[this.row]).sfToggleButton('blur');
			}
			
			this.row = (this.row + 1) % 3;
			
			if (this.row == 2) {
				$(index[this.row]).sfButton('focus');
			} else {
				$(index[this.row]).sfToggleButton('focus');
			}
			break;
			
		case sf.key.UP:
			if (this.row == 2) {
				$(index[this.row]).sfButton('blur');
			} else {
				$(index[this.row]).sfToggleButton('blur');
			}
			if (this.row == 0) {
				this.row = 2;
				$(index[this.row]).sfButton('focus');
			} else {
				this.row = this.row - 1;
				$(index[this.row]).sfToggleButton('focus');
			}
			break;
			
		case sf.key.ENTER:
			if (this.row == 2) {
				$("#browseButton").sfButton('blur');
				var _THIS_ = this;
				// Start samsung usb module to browse to a new download path
				sf.service.USB.show({
					callback: function(result) {
						
						var chunks = result[0].split('/');
						var i;
						var path = '/dtv/usb';
						
						for (i = 1; i < chunks.length - 1; i++){
							path = path + '/' + chunks[i];
						}
						
						$('#download_path_value').sfLabel("option", "text", path);
						download_path = path;
						var settings =  "/settings:" + $('#downspeed_limit_value').sfLabel("get").text() +
							":" + $('#upspeed_limit_value').sfLabel("get").text() +
							":" + $('#download_path_value').sfLabel("get").text();
	
						var url = tv_url + settings;
						sendHttp(url);
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
/**
 * Function used to send http requests to server to get and set the settings.
 */
function sendHttp(url) {
	xmlHttp = new XMLHttpRequest();
	xmlHttp.open("GET", url, true);
	xmlHttp.onreadystatechange = processSettingsResponse;
	xmlHttp.send(null);
}

/**
 * Function called when the http server sends a response to the requests that have been sent.
 */
function processSettingsResponse() {
	if (xmlHttp.readyState == 4) {
		settingsResult = xmlHttp.responseText;
	}
}
