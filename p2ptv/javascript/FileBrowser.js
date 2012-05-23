var widgetAPI = new Common.API.Widget();
var tvKey     = new Common.API.TVKeyValue();

var elementsPerPage = 9;
var fileSystem;
var files;
var page;
var selected;
var listLength;

var directory = ['$USB_DIR'];
var data = ['Item1', 'Item2', 'Item3', 'Item4', 'Item5', 'Item6', 'Item7', 'Item8', 'Item9', 'Item10', 'Item11', 'Item12'];
var data2 = ['Item1', 'Item2', 'Item3'];
var numbers = ['1', '2', '3', '4', '5', '6', '7', '8', '9'];

function init() {
	
	page = 0;
	listLength = 0;
	
	// Create File System object
	fileSystem = new FileSystem();
	// '$USB_DIR'
	files = fileSystem.readDir('$USB_DIR');
	
	// Load first page of files
	loadPage(page);
	
	$('#numbers').sfList({data:numbers, index:'0', itemsPerPage:numbers.length});
	$('#fileList').sfList('focus');
	
	$('#prevPage').sfLabel({text:"Previous Page"});
	$('#nextPage').sfLabel({text:"Next Page"});
	
	$('#ouput').sfLabel({text:'-----------------', width:'300px'});
	/*
	if(files){
		for(var i = 0; i < files.length; i++){
			alert("Filename " + (i + 1) + ": " + files[i].name);
			alert("Is a directory? " + files[i].isDir);
			$('#ouput').sfLabel({text:file[i].name});
		}
	}
	*/
	
	enableKeys();
	widgetAPI.sendReadyEvent();
}

function onUnload() {
	
}

function enableKeys(){
	
	document.getElementById("anchor").focus();
}

function keyDown() {
	var keyCode = event.keyCode;
	switch(keyCode){
		// Go to main menu
		case tvKey.KEY_RETURN:
			widgetAPI.blockNavigation(event);
			alert("RETURN pressed");
			gotoMain();
			break;
		// Open directory
		case tvKey.KEY_RIGHT:
		case tvKey.KEY_ENTER:
			alert("ENTER pressed");
			enterHandler();
			break;
		// Previous directory
		case tvKey.KEY_LEFT:
		case tvKey.KEY_PRECH:
			alert("PRECH pressed");
			backHandler();
			break;
		// Select element in list
		case tvKey.KEY_1:
			listSelection(0);
			$('#ouput').sfLabel({text:"1"});
			break;
		case tvKey.KEY_2:
			listSelection(1);
			$('#ouput').sfLabel({text:"2"});
			break;
		case tvKey.KEY_3:
			listSelection(2);
			$('#ouput').sfLabel({text:"3"});
			break;
		case tvKey.KEY_4:
			listSelection(3);
			$('#ouput').sfLabel({text:"4"});
			break;
		case tvKey.KEY_5:
			listSelection(4);
			$('#ouput').sfLabel({text:"5"});
			break;
		case tvKey.KEY_6:
			listSelection(5);
			$('#ouput').sfLabel({text:"6"});
			break;
		case tvKey.KEY_7:
			listSelection(6);
			$('#ouput').sfLabel({text:"7"});
			break;
		case tvKey.KEY_8:
			listSelection(7);
			$('#ouput').sfLabel({text:"8"});
			break;
		case tvKey.KEY_9:
			listSelection(8);
			$('#ouput').sfLabel({text:"9"});
			break;
		// Scroll through list
		case tvKey.KEY_UP:
			listScroll('prev');
			break;
		case tvKey.KEY_DOWN:
			listScroll('next');
			break;
		// Change file page
		case tvKey.KEY_PANEL_CH_UP:
		case tvKey.KEY_CH_UP:
			loadPage(page-1);
			break;
		case tvKey.KEY_PANEL_CH_DOWN:
		case tvKey.KEY_CH_DOWN:
			loadPage(page+1);
			break;
	}
}

//What to do when enter is pressed
function enterHandler(){
	
	var i = loadInfo($('#fileList').sfList('getIndex'));
	i = (page * elementsPerPage) + i;
	
	// If the selected file is a directory, open it.
	if(files[i].isDir()) {
		
		directory.push(directory[directoro.length-1] + files[i].name);
		files = fileSystem.readDir(directory[directory.length-1]);
		page = 0;
		loadPage(page);
	}
	else {
		
	}
}

function backHandler(){
	
	if(directory.length > 1){
		directory.pop();
		files = fileSystem.readDir(directory[directory.length-1]);
		page = 0;
		loadPage(page);
	}
}

// Select the element in the list of files that has been selected
function listSelection(key){
	
	if(key >= 0 && key < listLength){
		$('#fileList').sfList('move', key);
		$('#numbers').sfList('move', key);
		loadInfo($('#fileList').sfList('getSelectedItem'));
	}
}

// Scroll up or down the list of files
function listScroll(direction){
	
	if($('#fileList').sfList('getIndex') == 0 && page > 0 && direction == 'prev') {
		loadPage(page-1);
		$('#fileList').sfList('move', '8');
		$('#numbers').sfList('move', '8');
		return;
	}
	if($('#fileList').sfList('getIndex') == 8 && direction == 'next') {
		loadPage(page+1);
		return;
	}
	else {
		$('#fileList').sfList(direction);
		$('#numbers').sfList(direction);
	}
}

// Load a new page of 9 files
function loadPage(number){
	alert("loadPage called");
	if(number >= 0 && number * elementsPerPage < data.length){
		
		alert("loading page...");
		
		var elements = new Array(0);
		var buttons = new Array(0);
		var i =  number * elementsPerPage;
		var n = 0;
		
		while(i < (number * elementsPerPage) + elementsPerPage && i < data.length){
			elements.push(data[i]);
			buttons.push(numbers[n]);
			alert("Pushed: " + data[i]);
			i++;
			n++
		}
		
		alert("Array Length: " + elements.length);
		listLength = elements.length;
		
		$('#fileList').sfList({data:elements, index:'0', itemsPerPage:elements.length});
		$('#numbers').sfList({data:buttons, index:'0', itemsPerPage:buttons.length});
		page = number;
	}
}

// Load the information of a certain file and show on screen
function loadInfo(fileName){
	
}

function gotoMain(){
	alert("GOING TO MAIN");
	window.location = "index.html";
}
