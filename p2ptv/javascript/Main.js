// Variables that keep track of the focus
var xFocus = 0;
var yFocus = 0;

/* Function called when the app is initialised
 * Creates the buttons and starts key handler */
Main.onLoad = function() {
	
	$('#search').sfButton({text:'Search'});
	$('#browse').sfButton({text:'Browse'});
	$('#settings').sfButton({text:'Settings'});
	$('#exit').sfButton({text:'Exit'});
	
	focusLocation = 0;
	
	this.enableKeys();
	widgetAPI.sendReadyEvent();
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
		
		case tvKey.KEY_ENTER:
			alert("ENTER pressed");
			handleEnter();
			break;
		
		case tvKey.KEY_RIGHT:
			alert("RIGHT pressed");
			xFocus = (xFocus + 1) % 1
			break;
		
		case tvKey.KEY_LEFT:
			alert("RIGHT pressed");
			xFocus = (xFocus - 1) % 1
			break;
		
		case tvKey.KEY_DOWN:
			alert("RIGHT pressed");
			yFocus = (yFocus + 1) % 1
			break;
			
		case tvKey.KEY_UP:
			alert("RIGHT pressed");
			yFocus = (yFocus - 1) % 1
			break;
		
		default:
			alert("Ignore Unhandled Key");
			break;
	}
}

/* Determines what to do when ENTER key is pressed */
function handleEnter() {
	
	if(xFocus == 0) {
		if(yFocus == 0) {
			gotoSearch();
		} else {
			gotoSettings();
		}
	} else {
		if(yFocus == 0) {
			gotoBrowse();
		} else {
			exit();
		}
	}
}

/* Go to the search page */
function gotoSearch(){
	
}

/* Go to the settings page */
function gotoSettings(){
	
}

/* Go to the browse page */
function gotoBrowse(){
	
}

/* Exit the app */
function exit(){
	
}

