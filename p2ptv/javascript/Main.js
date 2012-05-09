var widgetAPI = new Common.API.Widget();
var tvKey     = new Common.API.TVKeyValue();

// Variables that keep track of the focus
var xFocus = 0;
var yFocus = 0;

var MaxXFocus = 4;
var MaxYFocus = 0;

var contentFrame;
var search;
var browse;
var settings;

/* Function called when the app is initialised
 * Creates the buttons and starts key handler */
Main.onLoad = function() {
	
	$('#search').sfButton({text:'Search'});
	$('#browse').sfButton({text:'Browse'});
	$('#settings').sfButton({text:'Settings'});
	$('#exit').sfButton({text:'Exit'});
	
	$('#search').sfButton('focus');
	$('#browse').sfButton('focus');
	$('#settings').sfButton('focus');
	$('#exit').sfButton('focus');
	
	contentFrame = document.getElementById('contentFrame');
	
	gotoSearch();
	
	this.enableKeys();
	widgetAPI.sendReadyEvent();
}

Main.onUnload  function() {
	
}

/* initialises the key handler */
Main.enableKeys = function() {
	
	document.getelementById("anchor").focus();
}

/* Function called on key down event */
Main.keyDown = function() {
	
	var keyCode = event.keyCode;
	aler("Key Pressed: " + keyCode);	
	switch(keyCode){
		case tvKey.KEY_RETURN:
			alert("RETURN pressed")
			// Stop App from exiting when pressed
			widgetAPI.blockNavigation();
			break;
			
		case tvkey.KEY_RED:
			alert("RED pressed");
			gotoSearch();
			break;
			
		case tvKey.KEY_GREEN:
			alert("GREEN pressed");
			gotoBrowse();
			break;
			
		case tvKey.KEY_YELLOW:
			alert("YELLOW pressed");
			gotoSettings();
			break;
			
		case tvKey.KEY_BLUE:
			alert("BLUE pressed");
			exit();
			break;
		default:
			alert("Ignore Unhandled Key");
			break;
	}
}

/* Go to the search page */
function gotoSearch(){
	contentFrame.src = "search.html";
}

/* Go to the settings page */
function gotoSettings(){
	contentFrame.src = "settings.html";
}

/* Go to the browse page */
function gotoBrowse(){
	contentFrame.src = "browser.html";
}

/* Exit the app */
function exit(){
	widgetAPI.sendReturnEvent();
}

