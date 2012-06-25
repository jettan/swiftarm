function SceneMain() {
	
}

var names = new Array();
var trackers = new Array();
var hashes = new Array();
var downloadStats = new Array();
var downloads = [];
var playlist = {};
var downloading = false;
var downloadPath = '/dtv/usb/sda1';

SceneMain.prototype.initialize = function () {
	alert('SceneMain.initialize()');
	this.itemPerPage = 8;
	this.data = [
		// UI Components
		'Browse',
		'Downloads',
		'Player',
		'Settings',
	];
	playlist = [];
	var listdata = [];
	for(var i=0; i<this.data.length; i++) {
		listdata.push(this.data[i]);
	}
	
	$('#category').sfList({
		data: listdata,
		index: 0,
		itemsPerPage: this.itemPerPage
	});
	
	$('#image').sfImage({src:'images/logo2.jpg'});
	$('#label').sfLabel({
		text: 'Swift P2P TV'
	});
	$('#label').sfLabel('hide');
	$('#labelVideo').sfLabel({text: ""});
	$('#labelVideo').sfLabel('hide');
	$('#labelDownloading').sfLabel({text: ""});
	$('#labelDownloading').sfLabel('hide');
	
	$('#MainBG').sfBackground({
        light: true,
        column: null,
        columnShadow: true,
        columnSize: 350
    });
	
	this.defaultOpts = {
        light: true,
        column: null,
        columnShadow: true,
        columnSize: 350/720*curWidget.height
    }
}

SceneMain.prototype.handleShow = function (data) {
	alert('SceneMain.handleShow()');	
	//var index = $('#category').sfList('getIndex');
	sf.scene.show('Start');
}

SceneMain.prototype.handleHide = function () {
	alert('SceneMain.handleHide()');
}

SceneMain.prototype.handleFocus = function () {
	alert('SceneMain.handleFocus()');
	var index = $('#category').sfList('getIndex');
	sf.scene.hide(this.data[index]);
	if ($('#labelRedirect').sfLabel("get").text()) {
		$('#category').sfList('move',2);
		sf.scene.show('Player');
		$('#category').sfList('blur'); 
		$('#category').sfList('hide');
		$('#image').sfImage('hide');
		$('#label').sfLabel('hide');
		$('#labelRedirect').sfLabel('hide');
		$('#MainBG').sfBackground(this.defaultOpts);
		sf.scene.focus('Player');
	} else {
		$('#category').sfList('focus');
		$('#image').sfImage('show');
		$('#label').sfLabel('show');
		$('#Main_keyhelp').sfKeyHelp({
			'user': 'user',
			'enter': 'Enter',		
			'move':'move',
			'return': 'Exit app',
		});
		$('#MainBG').sfBackground('option', 'column', 'left');
		$('#MainBG').sfBackground(this.defaultOpts);
		$('#MainBG').sfBackground('option', 'column', 'left');
	}

}

SceneMain.prototype.handleBlur = function () {
	alert('SceneMain.handleBlur()');
	$('#image').sfImage('hide');
	$('#label').sfLabel('hide');
	$('#MainBG').sfBackground(this.defaultOpts);
}

SceneMain.prototype.handleKeyDown = function (keyCode) {
	alert('SceneMain.handleKeyDown(' + keyCode + ')');
	switch (keyCode) {
		case sf.key.LEFT:
			break;
		case sf.key.RIGHT:
		case sf.key.ENTER:
			$('#startimage').sfImage('hide');
			var index = $('#category').sfList('getIndex');
			sf.scene.show(this.data[index]);
			$('#category').sfList('blur');            
			var index = $('#category').sfList('getIndex');
			$('#category').sfList('hide');
			$('#image').sfImage('hide');
			$('#label').sfLabel('hide');
			$('#MainBG').sfBackground(this.defaultOpts);
			sf.scene.focus(this.data[index]);
			break;
		case sf.key.UP:
			$('#category').sfList('prev');
			document.getElementById("MainListPage").innerHTML = (Math.floor($('#category').sfList('getIndex')/this.itemPerPage)+1) + "/" + Math.ceil(this.data.length/this.itemPerPage);
			break;
		case sf.key.DOWN:
			$('#category').sfList('next');
			document.getElementById("MainListPage").innerHTML = (Math.floor($('#category').sfList('getIndex')/this.itemPerPage)+1) + "/" + Math.ceil(this.data.length/this.itemPerPage);
			break;
		case sf.key.RED:
			$('#MainBG').sfBackground('option', 'column', 'left');
			break;
		case sf.key.GREEN:
		    $('#MainBG').sfBackground(this.defaultOpts);
			break;
		case sf.key.YELLOW:
			break;
		case sf.key.BLUE:
			break;
		case sf.key.RETURN:
			var _THIS_ = this;
			var exit = false;
			$('#popupExit').sfPopup({
					text:"Do you really want to exit the application?",
					buttons: ["Yes", "No"],
					defaultFocus: 1,
					keyhelp: {'return' : 'Return'},
					callback : function(selectedIndex){
						if (selectedIndex == 0) {
							exit = true;
						}
					}
				}).sfPopup('show');
			if (!exit)
				sf.key.preventDefault();
			else
				widgetApi.sendReturnEvent();
				//sf.core.exit(false);
			break;
	}
}
