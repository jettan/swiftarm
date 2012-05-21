var widgetAPI = new Common.API.Widget();
var tvKey     = new Common.API.TVKeyValue();
var downloads = new Array('download0', 'download1', 'download2', 'download3', 'download4');
var NewScript = document.createElement('script')
var selector  = 0;
var selected;

/**
 * Initialise DownloadManager GUI.
 */
function init() {
	this.enableKeys();
	widgetAPI.sendReadyEvent();
	//highlight();
	previous = selected;
	getDownloadInfo();
	
	alert("DownloadManager init() completed");
}

/**
 * Button handler for remote control.
 */
function keyDown() {
	
	var keyCode = event.keyCode;
	switch(keyCode){
		case tvKey.KEY_RETURN:
			widgetAPI.blockNavigation(event);
			alert("RETURN pressed");
			gotoMain();
		break;
		case tvKey.MENU:
			widgetAPI.blockNavigation(event);
			alert("MENU pressed");
			gotoMain();
		break;
		case tvKey.KEY_UP:
			goUp();
			alert(selector);
		break;
		case tvKey.KEY_DOWN:
			goDown();
			alert(selector);
		break;
		default:
			alert("Ignore Unhandled Key");
		break;
	}
}

/**
 * Highlight the selected item.
 */
function highlight() {
	switch(selector) {
		case 0:
			selected = document.getElementById(downloads[selector]);
			break;
		case 1:
			selected = document.getElementById(downloads[selector]);
			break;
		case 2:
			selected = document.getElementById(downloads[selector]);
			break;
		case 3:
			selected = document.getElementById(downloads[selector]);
			break;
		case 4:
			selected = document.getElementById(downloads[selector]);
			break;
		default:
			break;
	}		
	selected.style.background = "green";
}

/**
 * Deselect current selection to move on to the next.
 */
function deselect()
{
	selected.style.background = "transparent";
}

/**
 * Go one up on the list.
 */
function goUp() {
deselect();
selector = (selector == 0) ? 4 : selector-1;
highlight();
}

/**
 * Go one down on the list.
 */
function goDown() {
deselect();
selector = (selector+1) % 5;
highlight();
}

/**
 * Enable key presses.
 */
function enableKeys(){
	document.getElementById("anchor").focus();
}

/**
 * Go back to Main Menu.
 */
function gotoMain(){
	window.location = "index.html";
}
