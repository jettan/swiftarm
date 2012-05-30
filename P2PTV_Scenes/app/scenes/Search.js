function SceneSearch(options) {
	this.options = options;

	//var widgetAPI = new Common.API.Widget();
	//var tvKey     = new Common.API.TVKeyValue();

	this.letters = [[",", ".", "/", "-", "_", ":", ";", "(", ")"], ["a","b","c"], ["d","e","f"], ["g","h","i"], ["j","k","l"], ["m","n","o"], ["p","q","r"], ["s","t","u"], ["v","w","x"], ["y","z"]];
	this.overviewLetters = [", . / - _ : ; ( )", "a, b, c", "d, e, f", "g, h, i", "j, k, l", "m, n, o", "p, q, r", "s, t, u", "v, w, x", "y, z"];
	this.buttonValues = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "PRE-CH:\nshift", "type"];
	this.buttons = ['#zero', '#one', '#two', '#three', '#four', '#five', '#six', '#seven', '#eight', '#nine', '#shift', '#type'];
	this.labels = ['#zeroletters', '#oneletters', '#twoletters', '#threeletters', '#fourletters', '#fiveletters', '#sixletters', '#sevenletters', '#eightletters', '#nineletters'];

	this.keyStates = {
		OVERVIEW:0,
		SELECTION:1
	};

	this.letterType = {
		LETTERS:0,
		NUMBERS:1
	};

	this.shift;
	this.selection;
	this.type;
	this.keyState;
}

SceneSearch.prototype.initialize = function() {
	
	alert("Search init() called");
	
	this.shift = false;
	this.selection = -1;
	this.type = this.letterType.LETTERS;
	this.keyState = this.keyStates.OVERVIEW;
		
	for(var i = 0; i < this.buttons.length; i++){
		$(this.buttons[i]).sfButton({text:this.buttonValues[i]});
	}
	
	resetLetters();
	/*
	$('#one').sfButton({text:'1'});
	$('#two').sfButton({text:'2'});
	$('#three').sfButton({text:'3'});
	$('#four').sfButton({text:'4'});
	$('#five').sfButton({text:'5'});
	$('#six').sfButton({text:'6'});
	$('#seven').sfButton({text:'7'});
	$('#eight').sfButton({text:'8'});
	$('#nine').sfButton({text:'9'});
	$('#zero').sfButton({text:'0'});
	$('#shift').sfButton({text:'Shift', width:'100px'});
	$('#type').sfButton({text:'Type', width:'100px'});
	
	$('#oneletters').sfLabel({text:'a, b, c'});
	$('#twoletters').sfLabel({text:'d, e, f'});
	$('#threeletters').sfLabel({text:'g, h, i'});
	$('#fourletters').sfLabel({text:'j, k, l'});
	$('#fiveletters').sfLabel({text:'m, n, o'});
	$('#sixletters').sfLabel({text:'p, q, r'});
	$('#sevenletters').sfLabel({text:'s, t, u'});
	$('#eightletters').sfLabel({text:'v, w, x'});
	$('#nineletters').sfLabel({text:'y, z'});
	$('#zeroletters').sfLabel({text:', . / - _ : ; ( )'});
	*/
	
	//this.enableKeys();
	//widgetAPI.sendReadyEvent();
	/*
	var fileSystem = new FileSystem();
	if(!fileSystem.isValidCommonPath("$USB_DIR/sdb1/TVDownloads")){
		fileSystem.createCommonDir("USB_DIR/sdb1/TVDownloads");
	}
	*/
	alert("Search init() completed");
}

SceneSearch.prototype.handleShow = function() {
	alert("SceneSearch.handleShow()");
	// this function will be called when the scene manager show this scene 
}

SceneSearch.prototype.handleHide = function() {
	alert("SceneSearch.handleHide()");
	// this function will be called when the scene manager hide this scene  
}

SceneSearch.prototype.handleFocus = function() {
	alert("SceneSearch.handleFocus()");
	// this function will be called when the scene manager focus this scene
}

SceneSearch.prototype.handleBlur = function() {
	alert("SceneSearch.handleBlur()");
	// this function will be called when the scene manager move focus to another scene from this scene
}

SceneSearch.prototype.handleKeyDown = function(keyCode) {
	
	//var keyCode = event.keyCode;
	switch(keyCode){
		case sf.key.RETURN:
			//widgetAPI.blockNavigation(event);
			alert("RETURN pressed");
			gotoMain();
			break;
		// Shift
		case sf.key.VOL_UP:
			this.shift = !this.shift;
			updateLetters();
			break;
		case sf.key.N1:
			letterSelection(1);
			break;
		case sf.key.N2:
			letterSelection(2);
			break;
		case sf.key.N3:
			letterSelection(3);
			break;
		case sf.key.N4:
			letterSelection(4);
			break;
		case sf.key.N5:
			letterSelection(5);
			break;
		case sf.key.N6:
			letterSelection(6);
			break;
		case sf.key.N7:
			letterSelection(7);
			break;
		case sf.key.N8:
			letterSelection(8);
			break;
		case sf.key.N9:
			letterSelection(9);
			break;
		case sf.key.N0:
			letterSelection(0);
			break;
		// Spacebar
		case sf.key.MUTE:
			var searchValue = document.getElementById("searchBar").value;
			document.getElementById("searchBar").value = searchValue + " ";
			break;
		// Backspace functionality
		case sf.key.PRECH:
			var searchVal = document.getElementById("searchBar").value;
			document.getElementById("searchBar").value = searchVal.substring(0, searchVal.length-1);
			break;
		case sf.key.ENTER:
			// SEARCH!
			//HttpClient.httpGet("/search:" + document.getElementById("searchBar").value);
			break;
		case sf.key.DOWN:
			gotoTestSwarms();
			break;
		default:
			alert("Ignore Unhandled Key");
			break;
	}
}

function letterSelection(number){
	
	if(this.selection == -1) {
		
		this.selection = number;
		updateLetters();
		this.keyState = this.keyStates.SELECTION;
	}
	else if(this.selection > -1 && this.selection  < 10) {
	
		if(number <= this.letters[this.selection].length && number > 0) {
			
			var searchBarValue = document.getElementById("searchBar").value;
			// send selected letter to searchbar here
			if(!this.shift){
				document.getElementById("searchBar").value = searchBarValue + this.letters[this.selection][number-1];
			}
			else{
				document.getElementById("searchBar").value = searchBarValue + this.letters[this.selection][number-1].toUpperCase();
			}
			this.selection = -1;
			updateLetters();
			this.keyState = this.keyStates.OVERVIEW;
		}
	}
}

function updateLetters() {
	
	if(this.selection == -1) {
		resetLetters();
	}
	else{
		for(var j = 1; j < letters[this.selection].length+1; j++) {
			if(!this.shift){
				$(labels[j]).sfLabel({text:this.letters[this.selection][j-1]});
			}
			else{
				$(this.labels[j]).sfLabel({text:this.letters[this.selection][j-1].toUpperCase()});
			}
		}
		for(var i = this.letters[this.selection].length+1; i < this.labels.length; i++) {
			$(this.labels[i]).sfLabel({text:" "});
		}
	}
}

function resetLetters(){
	for(var j = 0; j < this.buttons.length; j++){
		if(!this.shift){
			$(this.labels[j]).sfLabel({text:this.overviewLetters[j]});
		}
		else{
			$(this.labels[j]).sfLabel({text:this.overviewLetters[j].toUpperCase()});
		}
	}
}

function gotoMain(){
	sf.scene.hide('Search');
	sf.scene.show('Main');
	sf.scene.focus('Main');
	//window.location = "index.html";
}

function gotoTestSwarms(){
	//window.location = "testSwarms.html";
}
