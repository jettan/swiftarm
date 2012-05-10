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
			widgetAPI.blockNavigation();
			alert("RETURN pressed");
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

