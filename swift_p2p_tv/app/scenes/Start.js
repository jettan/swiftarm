/**
 * Constructor of the scene Start.
 */
function SceneStart() {}

/**
 * Function called at scene init.
 */
SceneStart.prototype.initialize = function () {
	$('#startimage').sfImage({src:'images/logobg22.png'});
}

/**
 * Function called at scene show.
 */
SceneStart.prototype.handleShow = function () {
	$('#startimage').sfImage('show');
}

/**
 * Function called at scene hide.
 */
SceneStart.prototype.handleHide = function () {
	$('#startimage').sfImage('hide');
}

/**
 * Function called at scene focus.
 */
SceneStart.prototype.handleFocus = function () {
	alert('SceneStart.handleFocus()');
    
	$('#MainBG').sfBackground('option', 'column', 'left');
	$('#MainBG').sfBackground(this.defaultOpts);
	$('#MainBG').sfBackground('option', 'column', 'left');
	
	$('#image').sfImage('show');
	$('#label').sfLabel('show');
	$('#category').sfList('show');
	
	$('#startimage').sfImage('show');
	
	$("#Main_keyhelp").sfKeyHelp({
		'user': 'Help',		
		'move':'Move',
        'return': 'Return'
	});
}

/**
 * Function called at scene blur.
 */
SceneStart.prototype.handleBlur = function () {
	alert('SceneStart.handleBlur()');
}

/**
 * The remote control key handler of this scene.
 */
SceneStart.prototype.handleKeyDown = function (keyCode) {
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
		// Prevent widget from exiting.
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
