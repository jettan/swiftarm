function SceneScene1(options) {
	this.options = options;

   var index;
   var column, row;
}



SceneScene1.prototype.initialize = function () {
	alert("SceneScene1.initialize()");
	// this function will be called only once when the scene manager show this scene first time
	// initialize the scene controls and styles, and initialize your variables here 
	// scene HTML and CSS will be loaded before this function is called

	$('#svecButton_ZPQO').sfButton({text:'Button1', width:'200px'});
	$('#svecButton_AVN3').sfButton({text:'Button3', width:'200px'});
	$('#svecButton_NYCS').sfButton({text:'Button2', width:'200px'});
	$('#svecButton_FJM6').sfButton({text:'Button4', width:'200px'});
	
    $('#svecLabel_G6NW').sfLabel({text:'label', width:'131px'});
    $('#svecLabel_G6NW').sfLabel({text:'Please click the button.'});
	
	$('#progress1').sfProgressBar({
		type: 'loading',
		max: 100
	});
	$('#progress1').sfProgressBar('setValue', 50);
	//$('#progress1').sfProgressBar('show');
	
	$('#popup1').sfPopup({
		text: "Would You like to close this popup?",
		buttons: "OK"
	});
	
	// Multiple options mocht dat later nodig zijn
	/*$('#popup1').sfPopup({
    text: "Would You like to    close this popup?",
    buttons: ["Yes", "No"],
       defaultFocus: 1,    // index of default focused button. this    indicates array index of "buttons" option (zero-based)
    callback: function(selectedIndex) {
       alert("selected button index: " + selectedIndex);
    }
	});*/
	
    index = new Array(
        new Array('#svecButton_ZPQO', '#svecButton_NYCS'),
        new Array('#svecButton_AVN3', '#svecButton_FJM6')
    );  
    
    this.column = 0, this.row = 0;
  
}

var url1 = "http://localhost:1337/hello";
var url2 = "http://localhost:1337/test.txt";

SceneScene1.prototype.handleShow = function () {
	alert("SceneScene1.handleShow()");
	$('#svecLabel_G6NW').sfLabel({text:'We showing!!'});
    $('#svecButton_ZPQO').sfButton('focus');       // get focus
	sf.scene.getVisible();
	// this function will be called when the scene manager show this scene 
}

SceneScene1.prototype.handleHide = function () {
	alert("SceneScene1.handleHide()");
	// this function will be called when the scene manager hide this scene  
}

SceneScene1.prototype.handleFocus = function () {
	alert("SceneScene1.handleFocus()");
	// this function will be called when the scene manager focus this scene
}

SceneScene1.prototype.handleBlur = function () {
	alert("SceneScene1.handleBlur()");
	// this function will be called when the scene manager move focus to another scene from this scene
}

SceneScene1.prototype.handleKeyDown = function (keyCode) {
	alert("SceneScene1.handleKeyDown(" + keyCode + ")");
	// TODO : write an key event handler when this scene get focued
	switch (keyCode) {
		case sf.key.LEFT:
		case sf.key.RIGHT:  
            $(index[this.column][this.row]).sfButton('blur');
            this.row = (this.row ==0 ? 1 : 0);
            $(index[this.column][this.row]).sfButton('focus');  
            
			break;
            
		case sf.key.UP:
		case sf.key.DOWN:            
			$(index[this.column][this.row]).sfButton('blur');
            this.column = (this.column ==0 ? 1 : 0);
            $(index[this.column][this.row]).sfButton('focus');  
         
			break;

		case sf.key.ENTER:
            if(this.column == 0 && this.row == 0)
			{
				sf.scene.show('VideoPlayerPartial');
				sf.scene.focus('VideoPlayerPartial');
			}
            else if(this.column == 0 && this.row == 1)
			{
                $('#svecLabel_G6NW').sfLabel({text:'Clicked Button2!!'});
				doTimer();
			}
            else if(this.column == 1 && this.row == 0)     
			{
                $('#svecLabel_G6NW').sfLabel({text:'Clicked Button3!!'});
				$('#popup1').sfPopup('show'); 
			}
            else if(this.column == 1 && this.row == 1)
			{
                //$('#svecLabel_G6NW').sfLabel({text: $('#progress1').sfProgressBar('getValue')});
				alert($('#progress1').sfProgressBar('getValue'));
				var hoger = 74.5;
				$('#progress1').sfProgressBar('setValue', hoger);
				alert(hoger);
				alert($('#progress1').sfProgressBar('getValue'));
				/*int current = $('#progress1').sfProgressBar('getValue');
				int nieuw = current + 5 ;
				$('#progress1').sfProgressBar('setValue', nieuw);*/
			}
			break;
	}
}

var c=50;
var t;
var timer_is_on=0;

function timedCount()
{
	c++;
	$('#svecLabel_G6NW').sfLabel({text: c});
	$('#progress1').sfProgressBar('setValue', c);
	if(c < 100)
		t=setTimeout("timedCount()",1000);
	else
		stopCount();
}

function doTimer()
{
if (!timer_is_on)
  {
  timer_is_on=1;
  timedCount();
  }
}

function stopCount()
{
clearTimeout(t);
timer_is_on=0;
}

function httpGet(url) {
	request = new XMLHttpRequest();

	request.open("GET", url, true);
	request.onreadystatechange = processRequest;
	request.send(null);
}

function processRequest() {
	if (request.readyState == 4) {
		var result = request.responseText;
		//document.getElementById("textarea").value = result;
		$('#svecLabel_G6NW').sfLabel({text:result});
	}
}
