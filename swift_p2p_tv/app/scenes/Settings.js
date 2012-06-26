function SceneSettings() {
	var index;
	var row;
	var label;
	var textinit;
}

SceneSettings.prototype.initialize = function () {
	alert('SceneSettings.initialize()');
	
	$('#labelSetting0').sfLabel({text: "Enable debug logging"});
	$('#labelSetting1').sfLabel({text: "Limit down speed"});
	$('#labelSetting2').sfLabel({text: "Limit up speed"});
	$('#labelSetting3').sfLabel({text: "Stop seeding after ratio"});
	$('#labelSetting4').sfLabel({text: "Download path: "});
	
	$('#labelUp').sfLabel({text: "..."});
	$('#labelDown').sfLabel({text: "..."});
	$('#labelRatio').sfLabel({text: "..."});
	$('#labelPath').sfLabel({text: "/dtv/usb/sda1/..."});
	
	$('#browseButton').sfButton({
        text: 'Change path..'
    }).sfButton('addCallback', 'clicked', function (event, index) {
		alert("Button0 clicked: " + index);
	});
	
    $('#toggleButton0').sfToggleButton({
    }).sfToggleButton('addCallback', 'changed', function (event, checked) {
		sendSettings();
	});
	
	 $('#toggleButton1').sfToggleButton({
    }).sfToggleButton('addCallback', 'changed', function (event, checked) {
		if (checked) {
			if (!textinit)
				initText('#labelUp');
				
			$("#input0").sfTextInput('option','text',$('#labelUp').sfLabel("get").text());
			label = '#labelUp';
			$('#input0').sfTextInput('show');
			$('#input0').sfTextInput('focus');
		}
	});
	
	 $('#toggleButton2').sfToggleButton({
    }).sfToggleButton('addCallback', 'changed', function (event, checked) {
		if (checked) {
			if (!textinit)
				initText('#labelDown');
				
			$("#input0").sfTextInput('option','text',$('#labelDown').sfLabel("get").text());
			label = '#labelDown';
			$('#input0').sfTextInput('show');
			$('#input0').sfTextInput('focus');
		}
	});
	
	 $('#toggleButton3').sfToggleButton({
    }).sfToggleButton('addCallback', 'changed', function (event, checked) {
		if (checked) {
			if (!textinit)
				initText('#labelRatio');
				
			$("#input0").sfTextInput('option','text',$('#labelRatio').sfLabel("get").text());
			label = '#labelRatio';
			$('#input0').sfTextInput('show');
			$('#input0').sfTextInput('focus');
		}
	});
	
	index = new Array('#toggleButton0','#toggleButton1','#toggleButton2','#toggleButton3','#browseButton');
	
	var _THIS_ = this;
	$('#popup1').sfPopup({
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
$('#input0').sfTextInput({
		text:'...', 
		maxlength:'10',
		oncomplete: function (text) {
			if (text) {
				if(isNaN($("#input0").sfTextInput('getText'))) {
					$('#popup1').sfPopup('show');
				}
				else
					$(label).sfLabel("option", "text" , $("#input0").sfTextInput('getText'));
			}
			else {
				$(label).sfLabel("option", "text" , "0");
			}
			$('#input0').sfTextInput('hide');
		}
	});
	textinit = true;
	$('#input0').sfTextInput('hide');
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
	
	$('#toggleButton0').sfToggleButton('focus');
	this.row = 0
	
	$("#keyhelp_bar").sfKeyHelp({
		'user': 'Help',		
		'move':'Move',
        'return': 'Return'
	});
}

SceneSettings.prototype.handleBlur = function () {
	alert('SceneSettings.handleBlur()');
	$('#toggleButton0').sfToggleButton('blur');
	$('#toggleButton1').sfToggleButton('blur');
	$('#toggleButton2').sfToggleButton('blur');
	$('#toggleButton3').sfToggleButton('blur');
 
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
		if (this.row == 4)
			$(index[this.row]).sfButton('blur');
		else
			$(index[this.row]).sfToggleButton('blur');
		
		this.row = (this.row + 1) % 5;
		
		if (this.row == 4)
			$(index[this.row]).sfButton('focus');
		else
			$(index[this.row]).sfToggleButton('focus');
			break;
		case sf.key.UP:
		alert("INDEX!!! : " + this.row);
		if (this.row == 4)
			$(index[this.row]).sfButton('blur');
		else
			$(index[this.row]).sfToggleButton('blur');
			
		if (this.row == 0) {
			this.row = 4;
			$(index[this.row]).sfButton('focus');
		}
		else {
			this.row = this.row - 1;
			$(index[this.row]).sfToggleButton('focus');
		}
			break;
		case sf.key.ENTER:
		if (this.row == 4) {
			$("#browseButton").sfButton('blur');
			var _THIS_ = this;
			sf.service.USB.show({
				callback: function(result){
					alert("Callback of USB module: " + result);
					var chunks = result[0].split('/');
					var i;
					var path = '/dtv/usb';
					for (i=1; i< chunks.length-1;i++)
						path = path + '/' + chunks[i];
					 $('#labelPath').sfLabel("option", "text", path); 
					 downloadPath = path;
					//_THIS_.setResult(result?getObjString(result, 'result'):'null');
					$("#browseButton").sfButton('focus');
				},
				fileType: 'all'
			});
		}
		else 
			$(index[this.row]).sfToggleButton('toggle');
			break;
		case sf.key.RETURN:
			sf.scene.focus('Main');
            sf.key.preventDefault();
			break;
		case sf.key.RED:			
			break;
		case sf.key.GREEN:			
			break;
		case sf.key.YELLOW:            
			break;
		case sf.key.BLUE:
			break;
	}
}

var xmlHttp;
function sendSettings() {
  xmlHttp = new XMLHttpRequest();
  var xmlString = "<SETTINGS>" +
    "  <DEBUG>" + 0 + "</DEBUG>" +
    "  <DOWNLIMIT>" + 1 + "</DOWNLIMIT>" +
    "  <UPLIMIT>" + 2 + "</UPLIMIT>" +
    "  <STOPRATIO>" + 3 + "</STOPRATIO>" +
    "  <DOWNPATH>" + 4 + "</DOWNPATH>" +
    "</SETTINGS>";

  // Build the URL to connect to
  var url = "http://145.94.176.101:1337/settings";

  // Open a connection to the server
  xmlHttp.open("POST", url, true);

  // Tell the server you're sending it XML
  xmlHttp.setRequestHeader("Content-Type", "text/xml");

  // Set up a function for the server to run when it's done
  xmlHttp.onreadystatechange = processSettingsResponse;

  // Send the request
  xmlHttp.send(xmlString);
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
