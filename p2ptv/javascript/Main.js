var widgetAPI = new Common.API.Widget();
var tvKey     = new Common.API.TVKeyValue();


/* Function called when the app is initialised
 * Creates the buttons and starts key handler */
function init() {
	
	alert("init() called");
	
	$('#search').sfButton({text:'Search'});
	$('#browse').sfButton({text:'Browse'});
	$('#settings').sfButton({text:'Settings'});
	$('#downloadmanager').sfButton({text:'downloadmanager'});
	
	$('#search').sfButton('focus');
	$('#browse').sfButton('focus');
	$('#settings').sfButton('focus');
	$('#downloadmanager').sfButton('focus');
	/*
	if(serverIsAlive()){
		// Everything is fine
	}
	else{
		// Tell user to run SamyGo first
	}
	*/
	enableKeys();
	widgetAPI.sendReadyEvent();
	
	alert("init() completed");
}

function onUnload() {
	
}

/* initialises the key handler */
function enableKeys() {
	
	document.getElementById("anchor").focus();
}

/* Function called on key down event */
function keyDown() {
	
	var keyCode = event.keyCode;
	alert("Key Pressed: " + keyCode);
	switch(keyCode){
		case tvKey.KEY_RETURN:
			widgetAPI.blockNavigation();
			alert("RETURN pressed");
			// Stop App from exiting when pressed
			break;
			
		case tvKey.KEY_RED:
			alert("RED pressed");
			gotoSearch();
			break;
			
		case tvKey.KEY_GREEN:
			alert("GREEN pressed");
			gotoBrowser();
			break;
			
		case tvKey.KEY_YELLOW:
			alert("YELLOW pressed");
			gotoSettings();
			break;
			
		case tvKey.KEY_BLUE:
			alert("BLUE pressed");
			gotoDownloads();
			break;
		default:
			alert("Ignore Unhandled Key");
			break;
	}
}

/* Go to the search page */
function gotoSearch(){
	window.location = "search.html";
}

/* Go to the downloadmanager page */
function gotoDownloads(){
	window.location = "downloadmanager.html";
}

/* Go to the settings page */
function gotoSettings(){
	window.location = "settings.html";
}

/* Go to the browse page */
function gotoBrowser() {
	window.location = "fileBrowser.html";
}

/* Exit the app */
function exit(){
	widgetAPI.sendReturnEvent();
}
