//var widgetAPI = new Common.API.Widget();
//var tvKey     = new Common.API.TVKeyValue();

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
function init() {
	
	alert("init() called");
	
	//document.bgColor = "#000099";
	/*
	$('#search').sfButton({text:'Search'});
	$('#browse').sfButton({text:'Browse'});
	$('#settings').sfButton({text:'Settings'});
	$('#exit').sfButton({text:'Exit'});
	
	$('#search').sfButton('focus');
	$('#browse').sfButton('focus');
	$('#settings').sfButton('focus');
	$('#exit').sfButton('focus');
	
	contentFrame = document.getElementById("contentFrame");
	search = document.images["search"];
	browse = document.images["browse"];
	settings = document.images["setting"];
	*/
	//gotoSearch();
	
	//this.enableKeys();
	//widgetAPI.sendReadyEvent();
}

Main.onUnload = function() {
	
}

/* initialises the key handler */
Main.enableKeys = function() {
	
	document.getElementById("anchor").focus();
}

/* Function called on key down event */
function keyDown() {
	
	var keyCode = event.keyCode;
	alert("Key Pressed: " + keyCode);
	switch(keyCode){
		case tvKey.KEY_RETURN:
			alert("RETURN pressed");
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
	//var div = document.getElementById("content");
	//div.innerHTML="<iframe src=\"search.html\" frameborder =\"0\"></iframe>";
	//contentFrame.src = "search.html";
	//document.getElementById("contentFrame").src = "../search.html";
	//window.frames["contentFrame"].src = "browser.html";
	//document.frames['contentFrame'].location.href = "browser.html";
	
	/*
	var req = new XmlHttpRequest();
	req.open("GET", "browser.html", flase);
	req.send(null);
	
	var page = req.responseText;
	
	document.getElementById("content").innerHTML = page;
	*/
	alert("gotoSearch called");
	window.location = "search.html";
	/*
	var objTag = document.getElementById("contentFrame");
	if (objTag != null) {
		 objTag.setAttribute('data', 'search.html');
		alert('Page should have been changed');
	}
	*/
}

/* Go to the settings page */
function gotoSettings(){
	//contentFrame.src = "settings.html";
	//var div document.getElementById("content");
	//div.innerHTML='<object id=\"contentFrame\" name=\"contentFrame\" data=\"browser.html\"></object>';
	window.location = "settings.html";
}

/* Go to the browse page */
function gotoBrowser() {
	
	var div document.getElementById("content");
	div.innerHTML="<iframe src=\"browser.html\" frameborder ='0'></iframe>";
	var objTag = document.getElementById("contentFrame");
	if (objTag != null) {
		 objTag.setAttribute('data', 'browser.html');
		alert('Page should have been changed');
	}
	
	var req = new XmlHttpRequest();
	req.open("GET", "browser.html", flase);
	req.send(null);
	
	var page = req.responseText;
	
	document.getElementById("content").innerHTML = page;
	
	//document.getElementById("contentFrame").src = "browser.html";
	//window.frames["contentFrame"].src = "browser.html";
	//document.frames['contentFrame'].location.href = "browser.html";
}

/* Go to the settings page */
function gotoSettings(){
	contentFrame.src = "settings.html";
}

/* Exit the app */
function exit(){
	widgetAPI.sendReturnEvent();
}

