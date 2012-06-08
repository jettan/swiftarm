function SceneDownloads() {
  this.result = 0;
  this.index  = 0;
}

var c = 0;
var t;
var timer_is_on=0;

var XMLurl = "XML/data.xml";
var k = 0;
var progress = new Array();

/**
 * Function called at scene init
 */
SceneDownloads.prototype.initialize = function () {
	
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
	
	// Button that redirects to Player when pressed. Button appears after download complete.
	$('#normalButton02').sfButton({
        text: 'Go to player'
    }).sfButton('addCallback', 'clicked', function (event, index) {
		alert("Button0 clicked: " + index);
	});
	$('#normalButton02').sfButton('hide');
}

/**
 * Function called at scene show
 */
SceneDownloads.prototype.handleShow = function () {
	this.result = 0;
}

/**
 * Function called at scene hide
 */
SceneDownloads.prototype.handleHide = function () {
	
}

/**
 * Function called at scene focus
 */
SceneDownloads.prototype.handleFocus = function () {
    
    // Show left menu column
	$('#MainBG').sfBackground('option', 'column', 'left');
	$('#MainBG').sfBackground(this.defaultOpts);
	$('#MainBG').sfBackground('option', 'column', 'left');
	
	$('#image').sfImage('show');
	$('#label').sfLabel('show');
	$('#category').sfList('show');
	
	// When downloading start a timer to update the progressbar every 2 sec
	if ($('#labelDownloading').sfLabel("get").text())
		startProgress();
		
	this.index = 0;
	
	$('#progress12').sfProgressBar('setValue', 0);
	
	$("#Main_keyhelp").sfKeyHelp({
		'user': 'Help',		
		'move':'Move',
        'return': 'Return'
	});
}

/**
 * Function called at scene blur
 */
SceneDownloads.prototype.handleBlur = function () {

}

/**
 * Function called at scene key down
 */
SceneDownloads.prototype.handleKeyDown = function (keyCode) {
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
					$('#labelVideo').sfLabel('option','text',downloads[0]);
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

/**
 * Function that requests the progress of the current download from the server, and displays it in the progressbar
 * The timer is reset again so this function is called again after 2 seconds until download complete
 */
function getProgress()
{
	if (this.result < 100) {
		httpGetXML(progressURL);
		t = setTimeout("getProgress()",2000);
	} else
		stopCount();
}

/**
 * Function that calls the getProgress function and thereby starts a timer
 */
function startProgress()
{
	$('#labelFilename').sfLabel('option','text',downloads[0]);
	if (!timer_is_on) {
		timer_is_on = 1;
		getProgress();
	}
}

/**
 * Function called when scene is blurred, or when download is complete. The timeout is cleared.
 * Also when download is complete progressbar is set to 100, and a button that redirects to the Player is shown.
 */
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

/**
 * Function that requests download stats from the server
 */
function httpGetXML(url) {
	request = new XMLHttpRequest();
	request.open("GET", url, true);
	request.onreadystatechange = processXMLResponse;
	request.send(null);
}

/**
 * Function that process the response received from the server after sending a request
 */
function processXMLResponse() {
	if (request.readyState == 4) {
		// TODO: parse the XML response.
		/*var result = request.responseXML;
		$('DOWNLOAD',result).each(function(i) {
			progress[k] = $(this).find("PROGRESS").text();
			k++;
		});*/
		this.result = request.responseText;
		$('#labelProgress0').sfLabel({text:this.result});
		$('#progress12').sfProgressBar('setValue', this.result);
		
	}
}