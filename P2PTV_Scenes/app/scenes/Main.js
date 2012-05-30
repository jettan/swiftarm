function SceneMain(options) {
	this.options = options;
}

/* Function called when the app is initialised
 * Creates the buttons and starts key handler */
SceneMain.prototype.initialize = function() {
	
	alert("init() called");
	
	$('#search').sfButton({text:'Search'});
	$('#browse').sfButton({text:'Browse'});
	$('#settings').sfButton({text:'Settings'});
	$('#downloadmanager').sfButton({text:'Downloads'});
	
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
	//enableKeys();
	//widgetAPI.sendReadyEvent();
	
	alert("init() completed");
}

SceneMain.prototype.handleShow = function () {
	alert("SceneScene1.handleShow()");
	// this function will be called when the scene manager show this scene 
}

SceneMain.prototype.handleHide = function () {
	alert("SceneScene1.handleHide()");
	// this function will be called when the scene manager hide this scene  
}

SceneMain.prototype.handleFocus = function () {
	alert("SceneScene1.handleFocus()");
	// this function will be called when the scene manager focus this scene
}

SceneMain.prototype.handleBlur = function () {
	alert("SceneScene1.handleBlur()");
	// this function will be called when the scene manager move focus to another scene from this scene
}


function onUnload() {
	
}

/* initialises the key handler */
function enableKeys() {
	
	document.getElementById("anchor").focus();
}

/* Function called on key down event */
SceneMain.prototype.handleKeyDown = function(keyCode) {

	alert("Key Pressed: " + keyCode);
	switch(keyCode){
		case sf.key.RETURN:
			widgetAPI.blockNavigation();
			alert("RETURN pressed");
			// Stop App from exiting when pressed
			break;
			
		case sf.key.RED:
			alert("RED pressed");
			gotoSearch();
			break;
			
		case sf.key.GREEN:
			alert("GREEN pressed");
			gotoBrowser();
			break;
			
		case sf.key.YELLOW:
			alert("YELLOW pressed");
			gotoSettings();
			break;
			
		case sf.key.BLUE:
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
	sf.scene.hide('Main');
	sf.scene.show('Search');
	sf.scene.focus('Search');
	//window.location = "search.html";
}

/* Go to the downloadmanager page */
function gotoDownloads(){
	sf.scene.hide('Main');
	sf.scene.show('DownloadManager');
	sf.scene.focus('DownloadManager');
	//window.location = "downloadmanager.html";
}

/* Go to the settings page */
function gotoSettings(){
	sf.scene.hide('Main');
	sf.scene.show('Settings');
	sf.scene.focus('Settings');
	//window.location = "settings.html";
}

/* Go to the browse page */
function gotoBrowser() {
	sf.scene.hide('Main');
	sf.scene.show('FileBrowser');
	sf.scene.focus('FileBrowser');
	//window.location = "fileBrowser.html";
}

/* Exit the app */
function exit(){
	widgetAPI.sendReturnEvent();
}
