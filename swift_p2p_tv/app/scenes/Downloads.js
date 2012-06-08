function SceneDownloads() {
  this.result = 0;
  this.index = 0;
}

var c=0;
var t;
var timer_is_on=0;

var XMLurl = "XML/data.xml";
var k = 0;
var progress = new Array();

SceneDownloads.prototype.initialize = function () {
	alert('SceneDownloads.initialize()');
	
	$('#progress12').sfProgressBar({
		type: 'loading',
		max: 100
	});
	$('#progress12').sfProgressBar('setValue', 50);
	$('#progress12').sfProgressBar('show');
	
    $('#labelFilename').sfLabel({text: "Filename"});
	$('#labelDownload').sfLabel({text: "Download nr."});
	$('#labelProgress').sfLabel({text: "Progress"});
	$('#labelProgress0').sfLabel({text: "0 %"});
	
	$('#normalButton02').sfButton({
        text: 'Go to player'
    }).sfButton('addCallback', 'clicked', function (event, index) {
		alert("Button0 clicked: " + index);
	});
	$('#normalButton02').sfButton('hide');
}

SceneDownloads.prototype.handleShow = function () {
	alert('SceneDownloads.handleShow()');
	this.result = 0;
}

SceneDownloads.prototype.handleHide = function () {
	alert('SceneDownloads.handleHide()');
}

SceneDownloads.prototype.handleFocus = function () {
	alert('SceneDownloads.handleFocus()');
    
	$('#MainBG').sfBackground('option', 'column', 'left');
	$('#MainBG').sfBackground(this.defaultOpts);
	$('#MainBG').sfBackground('option', 'column', 'left');
	
	$('#image').sfImage('show');
	$('#label').sfLabel('show');
	$('#category').sfList('show');
	
	if ($('#labelDownloading').sfLabel("get").text())
		startProgress();
		
	this.index = 0;
	
	$('#progress12').sfProgressBar('setValue', 0);
	//$('#progress12').sfProgressBar('hide');
	//$('#normalButton02').sfButton('show');
	//$('#normalButton02').sfButton('focus');
	this.index = 1;
	
	$("#Main_keyhelp").sfKeyHelp({
		'user': 'Help',		
		'move':'Move',
        'return': 'Return'
	});
}

SceneDownloads.prototype.handleBlur = function () {
	alert('SceneDownloads.handleBlur()');
}

SceneDownloads.prototype.handleKeyDown = function (keyCode) {
	alert('SceneDownloads.handleKeyDown(' + keyCode + ')');
	switch (keyCode) {
		case sf.key.LEFT:
                sf.scene.focus('Main');       
			break;
		case sf.key.RIGHT:
			break;
		case sf.key.UP:
			break;
		case sf.key.DOWN:  
			break;
		case sf.key.ENTER:
		if (this.index == 1) {
				$('#labelRedirect').sfLabel('option','text','PlayerDownload');
				$('#labelRedirect').sfLabel('option','text',downloads[0]);
				$('#progress12').sfProgressBar('hide');
				sf.scene.focus('Main');
			}
			break;
		case sf.key.RETURN:
		if (timer_is_on)
			stopCount();
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

function getProgress()
{
	if (this.result < 100) {
		httpGetXML(progressURL);
		t = setTimeout("getProgress()",2000);
	} else
		stopCount();
}

function startProgress()
{
	$('#labelFilename').sfLabel('option','text',downloads[0]);
	if (!timer_is_on) {
		timer_is_on = 1;
		getProgress();
	}
}

function stopCount()
{
	if (this.result == 100) {
		$('#progress12').sfProgressBar('setValue', 100);
		$('#labelDownloading').sfLabel('option','text', '');
		$('#progress12').sfProgressBar('hide');
		$('#normalButton02').sfButton('show');
		$('#normalButton02').sfButton('focus');
	}
	clearTimeout(t);
	timer_is_on=0;
}

function httpGetXML(url) {
	request = new XMLHttpRequest();
	request.open("GET", url, true);
	request.onreadystatechange = processXMLRequest;
	request.send(null);
}

function processXMLRequest() {
	if (request.readyState == 4) {
		//var result = request.responseXML;
		this.result = request.responseText;
		/*$('DOWNLOAD',result).each(function(i) {
			progress[k] = $(this).find("PROGRESS").text();
			k++;
		});*/
		$('#labelProgress0').sfLabel({text:this.result});
		$('#progress12').sfProgressBar('setValue', this.result);
		
	}
}

SceneDownloads.prototype.refreshButtons = function () {
	alert('SceneDownloads.refreshButtons()');
}
