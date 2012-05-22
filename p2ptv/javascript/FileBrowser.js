var widgetAPI = new Common.API.Widget();
var tvKey     = new Common.API.TVKeyValue();

var elementsPerPage = 9;
var fileSystem;
var page;
var selected;
var listLength;

var data = ['Item1', 'Item2', 'Item3', 'Item4', 'Item5', 'Item6', 'Item7', 'Item8', 'Item9', 'Item10', 'Item11', 'Item12'];
var data2 = ['Item1', 'Item2', 'Item3']; 


function init() {
	
	page = 0;
	listLength = 0;
	
	// Create File System object
	fileSystem = new FileSystem();
	// '$USB_DIR'
	var files = fileSystem.readDir('$USB_DIR');
	
	// Load first page of files
	loadPage(page);

	$('#prevPage').sfLabel({text:"Previous Page"});
	$('#nextPage').sfLabel({text:"Next Page"});
	
	if(files){
		for(var i = 0; i < fileObj.length; i++){
			alert("Filename " + (i + 1) + ": " + files[i].name);
			alert("Is a directory? " + files[i].isDir);
		}
	}
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
		case tvKey.KEY_1:
			listSelection(0);
			break;
		case tvKey.KEY_2:
			listSelection(1);
			break;
		case tvKey.KEY_3:
			listSelection(2);
			break;
		case tvKey.KEY_4:
			listSelection(3);
			break;
		case tvKey.KEY_5:
			listSelection(4);
			break;
		case tvKey.KEY_6:
			listSelection(5);
			break;
		case tvKey.KEY_7:
			listSelection(6);
			break;
		case tvKey.KEY_8:
			listSelection(7);
			break;
		case tvKey.KEY_9:
			listSelection(8);
			break;
		case tvKey.KEY_UP:
			listScroll('prev');
			break;
		case tvKey.KEY_DOWN:
			listScroll('next');
			break;
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

// Select the element in the list of files that has been selected
function listSelection(key){

	if(key >= 0 && key < listLength){
		$('#fileList').sfList('move', key);
		loadInfo($('#fileList').sfList('getSelectedItem'));
	}
}

// Scroll up or down the list of files
function listScroll(direction){

	$('#list').sfList(direction);
}

// Load a new page of 9 files
function loadPage(number){
	alert("loadPage called");
	if(number >= 0 && number * elementsPerPage < data.length){
	
		alert("loading page...");

		var elements = new Array(0);
		var i =  number * elementsPerPage;
		
		while(i < (number * elementsPerPage) + elementsPerPage && i < data.length){
			elements.push(data[i]);
			alert("Pushed: " + data[i]);
			i++;
		}
		
		alert("Array Length: " + elements.length);
		listLength = elements.length;
		
		$('#fileList').sfList({data:elements, index:'0', itemsPerPage:elements.length});
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