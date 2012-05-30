var widgetAPI = new Common.API.Widget();
var tvKey     = new Common.API.TVKeyValue();

var elementsPerPage = 9;
var fileSystem;
var files;
var page;
var selected;
var listLength;
/*
var videoPos = {
	left: 700,
	top: 100,
	width: 500,
	height: 400
};
*/
//var videoState = $.sf.service.VideoPlayer.STATE_STOPPED;
var errorString = ['NoError', 'Network', 'Not Supported'];
var stateString = ['Playing', 'Stopped', 'Paused', 'Buffering', 'Scanning'];

var directory = ['$USB_DIR/sdb1'];
var data = ['Item1', 'Item2', 'Item3', 'Item4', 'Item5', 'Item6', 'Item7', 'Item8', 'Item9', 'Item10', 'Item11', 'Item12'];
var data2 = ['Item1', 'Item2', 'Item3'];
var numbers = ['1', '2', '3', '4', '5', '6', '7', '8', '9'];

function init() {
	
	page = 0;
	listLength = 0;
	
	// Create File System object
	fileSystem = new FileSystem();
	
	if(fileSystem.isValidCommonPath(directory[directory.length - 1] == 1){
		// '$USB_DIR'
		files = fileSystem.readDir(directory[directory.length-1]);
	}
	else{
		// Tell user to connect USB storage device.
	}
	
	if(files){
		// Load first page of files
		loadPage(page);
	}
	else{
		// show error message (USB storage devide not connected)
	}
	
	$('#fileList').sfList('focus');
	
	$('#prevPage').sfLabel({text:"Previous Page"});
	$('#nextPage').sfLabel({text:"Next Page"});
	
	enableKeys();
	widgetAPI.sendReadyEvent();
	/*
	var opt = {};
	opt.onerror = function(error, info){
		
		alert("Video Error: " + errorString[error] + " " + info);
	};
	
	opt.onend = function(){
		alert("Video END");
	};
	
	opt.onstatechange = function(state, info){
		videoState = state;
		setKeyHelp();
	}
	
	$.sf.service.VideoPlayer.init(opt);
	
	$.sf.service.VideoPlayer.setKeyHandler(sf.key.RETURN, function() {
		sf.service.VideoPlayer.setFullScreen(false);
	});
	
	$.sf.service.VideoPlayer.setPosition(videoPos);
	$.sf.service.VideoPlayer.show();
	*/
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
			/*
		case tvKey.KEY_PLAY:
			if($.sf.service.VideoPlayer.Skip.isInProgress()){
				$.sf.service.VideoPLayer.pause();
			}
			else{
				$.sf.service.VideoPlayer.resume();
			}
			break;
		case tvKey.KEY_STOP:
			if($.sf.service.VideoPlayer.Skip.inProgress()){
				$.sf.service.VideoPLayer.Skip.cancel();
			}
			$.sf.service.VideoPlayer.stop();
			break;
		case tvKey.KEY_PAUSE:
			$.sf.service.VideoPlayer.pause();
			break;
		case tvKey.KEY_FF:
			$.sf.service.VideoPlayer.Skip.start(10);
			break;
		case tvKey.KEY_REW:
			$.sf.service.VideoPlayer.Skip.start(-10);
			break;*/
	}
}

//What to do when enter is pressed
function enterHandler(){
	
	var i = $('#fileList').sfList('getIndex') + (page * elementsPerPage);
	
	// If the selected file is a directory, open it.
	if(files[i].isDir && files[i].name != ".." && files[i].name != ".") {
		
		directory.push(directory[directory.length-1] + "/" + files[i].name);
		files = fileSystem.readDir(directory[directory.length-1]);
		page = 0;
		loadPage(page);
	}
	else {
		/*
		// Selected file is not a directory (play video if possible)i
		if($.sf.service.VideoPlayer.Skip.isInProgress()){
			$.sf.service.VideoPlayer.Skip.stop();
		}
		else{
			if(videoSelected()){
				
				$.sf.VideoPlayer.stop();
				var url = getVideoUrl();
				$.sf.service.VideoPlayer.play(url);
			}
		}*/
	}
}

function videoSelected(){
	
	return files[i].name == "stream.mp4";
}

function getVideoUrl(){
	
}

// Go to the previous directory
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
	if(number >= 0 && number * elementsPerPage < files.length){
		
		alert("loading page...");
		
		var elements = new Array(0);
		var buttons = new Array(0);
		var i =  number * elementsPerPage;
		var n = 0;
		
		while(i < (number * elementsPerPage) + elementsPerPage && i < files.length/*data.length*/){
			elements.push(files[i].name);
			buttons.push(numbers[n]);
			alert("Pushed: " + files[i].name);
			i++;
			n++
		}
		
		alert("Array Length: " + elements.length);
		listLength = files.length;
		
		$('#fileList').sfList({data:elements, index:'0', itemsPerPage:elements.length});
		$('#numbers').sfList({data:buttons, index:'0', itemsPerPage:buttons.length});
		page = number;
	}
}

// Load the information of a certain file and show on screen
function loadInfo(fileName){
	
}

function setKeyHelp(state){
	/*
	var oKeyMap = {};
	
	if(videoState == $.sf.service.VideoPlayer.STATE_PLAYING ||
		videoState == $.sf.service.VideoPlayer.STATE_PAUSED ||
		videoState == $.sf.service.Video>player.STATE_BUFFERING){
		
		oKeyMap.RED = 'Fullscreen'
	}
	
	if($.sf.service.VideoPlayer.Skip.isInProgress()){
		
		oKeyMap.ENTER = 'Play';
		oKeyMap.RETURN = 'Cancel';
	}
	else{
		oKeyMap.UPDOWN = 'Move Item';
		oKeyMap.ENTER = 'Play';
		oKeyMap.RETURN = 'Return';
	}
	
	$("#keyhelp").sfKeyHelp(oKeyMap);
	*/
}

function gotoMain(){
	alert("GOING TO MAIN");
	window.location = "index.html";
}
