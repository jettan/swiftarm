function SceneStart() {
  
}

SceneStart.prototype.initialize = function () {
	alert('SceneStart.initialize()');
	
	$('#startimage').sfImage({src:'images/logobg22.png'});
	
}

SceneStart.prototype.handleShow = function () {
	alert('SceneStart.handleShow()');
	$('#startimage').sfImage('show');
}

SceneStart.prototype.handleHide = function () {
	alert('SceneStart.handleHide()');
	$('#startimage').sfImage('hide');
}

SceneStart.prototype.handleFocus = function () {
	alert('SceneStart.handleFocus()');
    
	$('#MainBG').sfBackground('option', 'column', 'left');
	$('#MainBG').sfBackground(this.defaultOpts);
	$('#MainBG').sfBackground('option', 'column', 'left');
	
	$('#image').sfImage('show');
	$('#label').sfLabel('show');
	$('#category').sfList('show');
	
	$('#startimage').sfImage('show');
	//startProgress();
	
	$("#Main_keyhelp").sfKeyHelp({
		'user': 'Help',		
		'move':'Move',
        'return': 'Return'
	});
}

SceneStart.prototype.handleBlur = function () {
	alert('SceneStart.handleBlur()');
}

SceneStart.prototype.handleKeyDown = function (keyCode) {
	alert('SceneStart.handleKeyDown(' + keyCode + ')');
	switch (keyCode) {
		case sf.key.LEFT:
                sf.scene.focus('Main');       
			break;
		case sf.key.RIGHT:
			break;
		case sf.key.UP:
			break;
		case sf.key.DOWN:  
			break;
		case sf.key.ENTER:
			break;
		case sf.key.RETURN:
		sf.scene.focus('Main');
        sf.key.preventDefault();
			break;
		case sf.key.RED:			
			break;
		case sf.key.GREEN:			
			break;
		case sf.key.YELLOW:            
			break;
		case sf.key.BLUE:
			break;
	}
}

