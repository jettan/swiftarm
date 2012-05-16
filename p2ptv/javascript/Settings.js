var widgetAPI = new Common.API.Widget();
var tvKey     = new Common.API.TVKeyValue();

function init() {
	
	this.enableKeys();
	widgetAPI.sendReadyEvent();
}

function enableKeys(){
	
	document.getElementById("anchor").focus();
}

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
		case tvKey.KEY_PRECH:
			alert("MENU or PRECH pressed");
			gotoMain();
			break;
			
		default:
			alert("Ignore Unhandled Key");
			break;
	}
}


function gotoMain(){
	
	window.location = "index.html";
}

