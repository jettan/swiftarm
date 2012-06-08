function SceneMain() {
	
}

var downloads    = [];
var stream       = '';
var downloadPath = '/dtv/usb/sda1';

/**
 * Function called at scene init
 */
SceneMain.prototype.initialize = function () {
	this.itemPerPage = 8;
	this.data = [
		'Browse',
		'Downloads',
		'Player',
		'Settings',
	];
	
	var listdata = [];
	for(var i=0; i<this.data.length; i++) {
		listdata.push(this.data[i]);
	}
	
	// Initialize left column menu with scenes
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

/**
 * Function called at scene show
 */
SceneMain.prototype.handleShow = function (data) {	
	sf.scene.show('Start');
}

/**
 * Function called at scene hide
 */
SceneMain.prototype.handleHide = function () {

}

/**
 * Function called at scene focus
 */
SceneMain.prototype.handleFocus = function () {
	var index = $('#category').sfList('getIndex');
	sf.scene.hide(this.data[index]);
	// If this scene was focused with the redirection label filled in, don't load elements just redirect to Player
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
			'return': 'Return',
			'red': 'Left column',
		});
		$('#MainBG').sfBackground('option', 'column', 'left');
		$('#MainBG').sfBackground(this.defaultOpts);
		$('#MainBG').sfBackground('option', 'column', 'left');
	}

}

/**
 * Function called at scene blur
 */
SceneMain.prototype.handleBlur = function () {
	$('#image').sfImage('hide');
	$('#label').sfLabel('hide');
	$('#MainBG').sfBackground(this.defaultOpts);
}

/**
 * Function called at scene key down
 */
SceneMain.prototype.handleKeyDown = function (keyCode) {
	switch (keyCode) {
		case sf.key.LEFT:
			break;
		case sf.key.RIGHT:
		case sf.key.ENTER:
			$('#startimage').sfImage('hide');
			$('#category').sfList('blur');            
			$('#category').sfList('hide');
			$('#image').sfImage('hide');
			$('#label').sfLabel('hide');
			$('#MainBG').sfBackground(this.defaultOpts);
			
			var index = $('#category').sfList('getIndex');
			sf.scene.show(this.data[index]);
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
	}
}
