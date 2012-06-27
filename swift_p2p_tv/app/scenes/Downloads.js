function SceneDownloads(options) {
	this.options = options;
	
	this.progressopts = [{
		type: 'status',
		max: 100
	}];
	
	//this.downloadList;
	this.size = 5;
}

var statsURL = tv_url + "/stats";
var downloadList;
var allDownloads;
var progressBarList = [];
var elementList = [];
var linesList = [];
var focus = 0;
var count = 0;
var downloadNumber = 0;

SceneDownloads.prototype.initialize = function () {
	alert("SceneDownloads.initialize()");
	// this function will be called only once when the scene manager show this scene first time
	// initialize the scene controls and styles, and initialize your variables here 
	// scene HTML and CSS will be loaded before this function is called
	var downloadZero = [];
	var downloadOne = [];
	var downloadTwo = [];
	var downloadThree = [];
	var downloadFour = [];
	
	var elementZero = [];
	var elementOne = [];
	var elementTwo = [];
	var elementThree = [];
	var elementFour = [];
	
	$('#svecButton_EZNV').sfButton({text:'clear'});
	$('#svecLabel_4MQH').sfLabel({text:'up:'});
	$('#svecLabel_F9M8').sfLabel({text:'XX'});
	$('#svecLabel_TCTT').sfLabel({text:'down:'});
	$('#svecLabel_0KZJ').sfLabel({text:'XX'});
	$('#svecLabel_GB6Y').sfLabel({text:'Selected download:'});
	$('#svecLabel_1XTU').sfLabel({text:'X / X'});
	
	// First download 
	$('#nameLabel').sfLabel({text:'Name'});
	$('#roothashLabel').sfLabel({text:'roothash'});
	$('#completeLabel').sfLabel({text:'Complete'});
	$('#sizeLabel').sfLabel({text:'Size'});
	$('#progressLabel').sfLabel({text:'('});
	$('#statusLabel').sfLabel({text:'X'});
	$('#downSpeed').sfLabel({text:'DL: '});
	$('#upSpeed').sfLabel({text:'UL: '});
	$('#svecLabel_07NY').sfLabel({text:'seeders: '});
	$('#svecLabel_ZB04').sfLabel({text:'peers: '});
	$('#slashLabel').sfLabel({text:'/'});
	$('#seedersLabel').sfLabel({text:'..'});
	$('#peersLabel').sfLabel({text:'..'});
	$('#timeLabel').sfLabel({text:' remaining'});
	$('#svecLabel_QQM0').sfLabel({text:'XX'});
	$('#svecLabel_0660').sfLabel({text:'XXXX'});
	$('#svecLabel_AO6B').sfLabel({text:'XXXX'});
	$('#svecLabel_5911').sfLabel({text:'100'});
	$('#svecLabel_7ANW').sfLabel({text:'%)'});
	downloadZero.push('#sizeLabel');
	downloadZero.push('#completeLabel');
	downloadZero.push('#statusLabel');
	downloadZero.push('#nameLabel');
	downloadZero.push('#svecLabel_AO6B');
	downloadZero.push('#svecLabel_0660');
	downloadZero.push('#svecLabel_5911');
	downloadZero.push('#seedersLabel');
	downloadZero.push('#peersLabel');
	downloadZero.push('#svecLabel_QQM0');
	downloadZero.push('#roothashLabel');
	elementZero = downloadZero.slice(0);
	elementZero.push('#progressLabel');
	elementZero.push('#downSpeed');
	elementZero.push('#upSpeed');
	elementZero.push('#svecLabel_07NY');
	elementZero.push('#svecLabel_ZB04');
	elementZero.push('#slashLabel');
	elementZero.push('#timeLabel');
	elementZero.push('#svecLabel_7ANW');
	
	//Second download
	$('#svecLabel_5032').sfLabel({text:'Name'});
	$('#svecLabel_6WZA').sfLabel({text:'roothash'});
	$('#svecLabel_V7AX').sfLabel({text:'Complete'});
	$('#svecLabel_9JQR').sfLabel({text:'Size'});
	$('#svecLabel_09H7').sfLabel({text:'('});
	$('#svecLabel_4TO1').sfLabel({text:'X'});
	$('#svecLabel_GYID').sfLabel({text:'DL: '});
	$('#svecLabel_2V1Q').sfLabel({text:'UL: '});
	$('#svecLabel_VP16').sfLabel({text:'seeders: '});
	$('#svecLabel_GX9V').sfLabel({text:'peers: '});
	$('#svecLabel_0L1K').sfLabel({text:'/'});
	$('#svecLabel_S6BL').sfLabel({text:'..'});
	$('#svecLabel_20PQ').sfLabel({text:'..'});
	$('#svecLabel_K8Y8').sfLabel({text:'remaining'});
	$('#svecLabel_HLQI').sfLabel({text:'XX'});
	$('#svecLabel_BR0U').sfLabel({text:'XXXX'});
	$('#svecLabel_PR2L').sfLabel({text:'XXXX'});
	$('#svecLabel_YY6U').sfLabel({text:'100'});
	$('#svecLabel_Y145').sfLabel({text:'%)'});
	downloadOne.push('#svecLabel_9JQR');
	downloadOne.push('#svecLabel_V7AX');
	downloadOne.push('#svecLabel_4TO1');
	downloadOne.push('#svecLabel_5032');
	downloadOne.push('#svecLabel_PR2L');
	downloadOne.push('#svecLabel_BR0U');
	downloadOne.push('#svecLabel_YY6U');
	downloadOne.push('#svecLabel_S6BL');
	downloadOne.push('#svecLabel_20PQ');
	downloadOne.push('#svecLabel_HLQI');
	downloadOne.push('#svecLabel_6WZA');
	elementOne = downloadOne.slice(0);
	elementOne.push('#svecLabel_09H7');
	elementOne.push('#svecLabel_GYID');
	elementOne.push('#svecLabel_2V1Q');
	elementOne.push('#svecLabel_VP16');
	elementOne.push('#svecLabel_GX9V');
	elementOne.push('#svecLabel_0L1K');
	elementOne.push('#svecLabel_K8Y8');
	elementOne.push('#svecLabel_Y145');
	
	//Third download
	$('#svecLabel_J3JD').sfLabel({text:'Name'});
	$('#svecLabel_YLU6').sfLabel({text:'roothash'});
	$('#svecLabel_8MZF').sfLabel({text:'Complete'});
	$('#svecLabel_VJNN').sfLabel({text:'Size'});
	$('#svecLabel_N4Q2').sfLabel({text:'('});
	$('#svecLabel_CB8O').sfLabel({text:'remaining'});
	$('#svecLabel_JVQZ').sfLabel({text:'X'});
	$('#svecLabel_1L1N').sfLabel({text:'DL: '});
	$('#svecLabel_56OO').sfLabel({text:'UL: '});
	$('#svecLabel_1NU4').sfLabel({text:'seeders: '});
	$('#svecLabel_SS0E').sfLabel({text:'peers: '});
	$('#svecLabel_N9IC').sfLabel({text:'/'});
	$('#svecLabel_PDRT').sfLabel({text:'..'});
	$('#svecLabel_J5EW').sfLabel({text:'..'});
	$('#svecLabel_DMIE').sfLabel({text:'XX'});
	$('#svecLabel_Y6NL').sfLabel({text:'XXXX'});
	$('#svecLabel_ST5V').sfLabel({text:'XXXX'});
	$('#svecLabel_6IRF').sfLabel({text:'100'});
	$('#svecLabel_MB4M').sfLabel({text:'%)'});
	downloadTwo.push('#svecLabel_VJNN');
	downloadTwo.push('#svecLabel_8MZF');
	downloadTwo.push('#svecLabel_JVQZ');
	downloadTwo.push('#svecLabel_J3JD');
	downloadTwo.push('#svecLabel_ST5V');
	downloadTwo.push('#svecLabel_Y6NL');
	downloadTwo.push('#svecLabel_6IRF');
	downloadTwo.push('#svecLabel_PDRT');
	downloadTwo.push('#svecLabel_J5EW');
	downloadTwo.push('#svecLabel_DMIE');
	downloadTwo.push('#svecLabel_YLU6');
	elementTwo = downloadTwo.slice(0);
	elementTwo.push('#svecLabel_N4Q2');
	elementTwo.push('#svecLabel_CB8O');
	elementTwo.push('#svecLabel_1L1N');
	elementTwo.push('#svecLabel_56OO');
	elementTwo.push('#svecLabel_1NU4');
	elementTwo.push('#svecLabel_SS0E');
	elementTwo.push('#svecLabel_N9IC');
	elementTwo.push('#svecLabel_MB4M');
	
	//Fourth download
	$('#svecLabel_FDPV').sfLabel({text:'Name'});
	$('#svecLabel_LC37').sfLabel({text:'roothash'});
	$('#svecLabel_J8RO').sfLabel({text:'Complete'});
	$('#svecLabel_YPN4').sfLabel({text:'Size'});
	$('#svecLabel_K8OX').sfLabel({text:'('});
	$('#svecLabel_IUIV').sfLabel({text:'remaining'});
	$('#svecLabel_CXKL').sfLabel({text:'X'});
	$('#svecLabel_QF5M').sfLabel({text:'DL:'});
	$('#svecLabel_THE0').sfLabel({text:'UL:'});
	$('#svecLabel_P51S').sfLabel({text:'seeders: '});
	$('#svecLabel_CVU8').sfLabel({text:'peers: '});
	$('#svecLabel_M14B').sfLabel({text:'/'});
	$('#svecLabel_VLQT').sfLabel({text:'..'});
	$('#svecLabel_OCZP').sfLabel({text:'..'});
	$('#svecLabel_P71T').sfLabel({text:'XX'});
	$('#svecLabel_FXXA').sfLabel({text:'XXXX'});
	$('#svecLabel_S3AD').sfLabel({text:'XXXX'});
	$('#svecLabel_NNKJ').sfLabel({text:'100'});
	$('#svecLabel_AXTX').sfLabel({text:'%)'});
	downloadThree.push('#svecLabel_YPN4');
	downloadThree.push('#svecLabel_J8RO');
	downloadThree.push('#svecLabel_CXKL');
	downloadThree.push('#svecLabel_FDPV');
	downloadThree.push('#svecLabel_S3AD');
	downloadThree.push('#svecLabel_FXXA');
	downloadThree.push('#svecLabel_NNKJ');
	downloadThree.push('#svecLabel_VLQT');
	downloadThree.push('#svecLabel_OCZP');
	downloadThree.push('#svecLabel_P71T');
	downloadThree.push('#svecLabel_LC37');
	elementThree = downloadThree.slice(0);
	elementThree.push('#svecLabel_K8OX');
	elementThree.push('#svecLabel_IUIV');
	elementThree.push('#svecLabel_QF5M');
	elementThree.push('#svecLabel_THE0');
	elementThree.push('#svecLabel_P51S');
	elementThree.push('#svecLabel_CVU8');
	elementThree.push('#svecLabel_M14B');
	elementThree.push('#svecLabel_AXTX');

	
	//Fifth download
	$('#svecLabel_9N28').sfLabel({text:'Name'});
	$('#svecLabel_XIH7').sfLabel({text:'roothash'});
	$('#svecLabel_7CVC').sfLabel({text:'Complete'});
	$('#svecLabel_T44Z').sfLabel({text:'Size'});
	$('#svecLabel_0S90').sfLabel({text:'('});
	$('#svecLabel_GQFE').sfLabel({text:'remaining'});
	$('#svecLabel_UQS1').sfLabel({text:'X'});
	$('#svecLabel_7Q4C').sfLabel({text:'DL: '});
	$('#svecLabel_JYIQ').sfLabel({text:'UL: '});
	$('#svecLabel_6AAE').sfLabel({text:'seeders: '});
	$('#svecLabel_BA3X').sfLabel({text:'peers: '});
	$('#svecLabel_CQNS').sfLabel({text:'/'});
	$('#svecLabel_YEV7').sfLabel({text:'..'});
	$('#svecLabel_5RVG').sfLabel({text:'..'});
	$('#svecLabel_9H7R').sfLabel({text:'XX'});
	$('#svecLabel_CXDK').sfLabel({text:'XXXX'});
	$('#svecLabel_SLJE').sfLabel({text:'XXXX'});
	$('#svecLabel_BJKU').sfLabel({text:'100'});
	$('#svecLabel_XGFC').sfLabel({text:'%)'});
	downloadFour.push('#svecLabel_T44Z');
	downloadFour.push('#svecLabel_7CVC');
	downloadFour.push('#svecLabel_UQS1');
	downloadFour.push('#svecLabel_9N28');
	downloadFour.push('#svecLabel_SLJE');
	downloadFour.push('#svecLabel_CXDK');
	downloadFour.push('#svecLabel_BJKU');
	downloadFour.push('#svecLabel_YEV7');
	downloadFour.push('#svecLabel_5RVG');
	downloadFour.push('#svecLabel_9H7R');
	downloadFour.push('#svecLabel_XIH7');
	elementFour = downloadFour.slice(0);
	elementFour.push('#svecLabel_0S90');
	elementFour.push('#svecLabel_GQFE');
	elementFour.push('#svecLabel_7Q4C');
	elementFour.push('#svecLabel_JYIQ');
	elementFour.push('#svecLabel_6AAE');
	elementFour.push('#svecLabel_BA3X');
	elementFour.push('#svecLabel_CQNS');
	elementFour.push('#svecLabel_XGFC');

	
	$("#progressDownload").sfProgressBar(this.progressopts[0]);
	$("#progressDownload").sfProgressBar('setValue', 50);
	$("#progressDownload2").sfProgressBar(this.progressopts[0]);
	$("#progressDownload2").sfProgressBar('setValue', 50);
	$("#progressDownload3").sfProgressBar(this.progressopts[0]);
	$("#progressDownload3").sfProgressBar('setValue', 50);
	$("#progressDownload4").sfProgressBar(this.progressopts[0]);
	$("#progressDownload4").sfProgressBar('setValue', 50);
	$("#progressDownload5").sfProgressBar(this.progressopts[0]);
	$("#progressDownload5").sfProgressBar('setValue', 50);
	
	progressBarList.push("#progressDownload");
	progressBarList.push("#progressDownload2");
	progressBarList.push("#progressDownload3");
	progressBarList.push("#progressDownload4");
	progressBarList.push("#progressDownload5");
	
	linesList.push("line3");
	linesList.push("line4");
	linesList.push("line5");
	linesList.push("line6");
	linesList.push("line7");
	
	downloadList = [downloadZero, downloadOne, downloadTwo, downloadThree, downloadFour];
	elementList = [elementZero, elementOne, elementTwo, elementThree, elementFour];
}




SceneDownloads.prototype.handleShow = function () {
	alert("SceneDownloads.handleShow()");
	// this function will be called when the scene manager show this scene 
}

SceneDownloads.prototype.handleHide = function () {
	alert("SceneDownloads.handleHide()");
	// this function will be called when the scene manager hide this scene  
}

SceneDownloads.prototype.handleFocus = function () {
	alert("SceneDownloads.handleFocus()");
	// this function will be called when the scene manager focus this scene
	$('#MainBG').sfBackground('option', 'column', 'left');
	$('#MainBG').sfBackground(this.defaultOpts);
	$('#MainBG').sfBackground('option', 'column', 'left');
	
	$('#image').sfImage('show');
	$('#label').sfLabel('show');
	$('#scene_list').sfList('show');
	
	var d;
	for (d = 0; d < 5; d++)
		hideElement(d);
	
	focus = 0;
	focusElement(focus);
	
	startProgress();
	
	$("#keyhelp_bar").sfKeyHelp({
		'user': 'Help',		
		'move':'Move',
        'return': 'Return',
		'pause': 'Pause download/upload',
		'play': 'Start/Resume download',
		'stop': 'Stop download'
	});
}

SceneDownloads.prototype.handleBlur = function () {
	alert("SceneDownloads.handleBlur()");
	// this function will be called when the scene manager move focus to another scene from this scene
}

SceneDownloads.prototype.handleKeyDown = function (keyCode) {
	alert("SceneDownloads.handleKeyDown(" + keyCode + ")");
	switch (keyCode) {
		case sf.key.LEFT:
			hideElement(focus);
			break;
		case sf.key.RIGHT:
			showElement(focus);
			break;
		case sf.key.UP:
			if (focus == 0 && downloadNumber == 0)
				break;
				
			this.blurElement(focus);	
			if (focus == 0 && downloadNumber > 0) {
				focus = 4;
				prevPage();
			}
			else {
				focus = focus - 1;
				focusElement(focus);
			}
			break;
		case sf.key.DOWN:
			if (focus == (allDownloads.length -1) % 5 && downloadNumber == Math.floor(allDownloads.length / 5))
				break;
				
			this.blurElement(focus);
			if (focus == 4)
					nextPage();
			else {
				focus = focus + 1;
				focusElement(focus);
			}
			break;

		case sf.key.ENTER:
			//this.httpGetXML(statsURL);
			$('#MainBG').sfBackground('option', 'column', 'left');
			break;
		case sf.key.RETURN:
			this.stopProgress();
			sf.scene.focus('Main');
            sf.key.preventDefault();
			break;
	}
}

var statsRequest;
function httpGetXML(url) {
	statsRequest = new XMLHttpRequest();
	statsRequest.open("GET", url, true);
	statsRequest.onreadystatechange = processStatsResponse;
	statsRequest.send(null);
}

function processStatsResponse() {
	if (statsRequest.readyState == 4) {
		var result = statsRequest.responseXML;
		allDownloads = [];
		var one = [];
		var two = [];
		var three = [];
		var four = [];
		var five = [];
		//downloadStats = [one , two, three, four, five];
		downloadStats = [];
		//downloadStats = new Array(5);
		alert(result);
		count = 0;
		var _THIS_ = this;
		$('DOWNLOAD',result).each(function(i) {
			//downloadStats = [];
			alert($(this).find("SIZE").text());
			allDownloads[count] = [];
			allDownloads[count].push($(this).find("SIZE").text());
			allDownloads[count].push($(this).find("COMPLETED").text());
			allDownloads[count].push($(this).find("STATUS").text());
			allDownloads[count].push($(this).find("NAME").text());
			allDownloads[count].push($(this).find("DSPEED").text());
			allDownloads[count].push($(this).find("USPEED").text());
			allDownloads[count].push($(this).find("PROGRESS").text());
			/*downloadStats.push($(this).find("RATIO").text());
			downloadStats.push($(this).find("UPLOADAMOUNT").text());
			downloadStats.push($(this).find("DOWNLOADAMOUNT").text());*/
			allDownloads[count].push($(this).find("SEEDERS").text());
			allDownloads[count].push($(this).find("PEERS").text());
			/*downloadStats.push($(this).find("TIMEDAYS").text());
			downloadStats.push($(this).find("TIMEHOURS").text());*/
			allDownloads[count].push($(this).find("TIMEMINUTES").text());
			//downloadStats.push($(this).find("TIMESECONDS").text());
			allDownloads[count].push($(this).find("HASH").text());
			alert(allDownloads[count].length);
			count++;
		});
		alert('Processing response');
		alert(allDownloads.length);
		var limit = (allDownloads.length >= 5 ? 5 : allDownloads.length);
		var d;
		for(d = 0; d< limit; d++) {
			alert('In de loop!');
			var counter;
			for(counter = 0; counter < 11; counter++)
				$(downloadList[d][counter]).sfLabel("option","text",allDownloads[d][counter]);
			
			$(progressBarList[d]).sfProgressBar('setValue', allDownloads[d][6]);
			
			showElement(d);
		}
		
	}
}

var timer;
var timer_on=0;

function getProgress() {
	alert('Get progress!');
	httpGetXML(statsURL);
	t=setTimeout("getProgress()",4000);
}

function startProgress() {
	alert('Start progress!');
	if (!timer_on) {
		timer_on=1;
		getProgress();
	}
}

function stopProgress() {
	clearTimeout(timer);
	timer_on=0;
}

function nextPage() {
	if(downloadNumber < Math.floor(allDownloads.length / 5) - 1) {
		downloadNumber++;
		switchPage();
		focus = 0;
		focusElement(focus);
	} else if (downloadNumber == Math.floor(allDownloads.length / 5) - 1) {
		downloadNumber++;
		var numelements = allDownloads.length % 5;
		var hidecounter = 0;
		for (hidecounter = numelements; hidecounter < 5; hidecounter++)
			hideElement(hidecounter);
		switchPage();
		focus = 0;
		focusElement(focus);
	}
}

function prevPage() {
	
	if (downloadNumber == Math.floor(allDownloads.length / 5)) {
			var showcounter = 0;
			for (showcounter = 0; showcounter < 5; showcounter++)
				showElement(showcounter);
	}
		
	if(downloadNumber > 0) {
		downloadNumber--;
		switchPage();
		focus = 4;
		focusElement(focus);
	}
}

function switchPage() {
	var end = 5;
	if ((allDownloads.length % 5) > 0)
		end = allDownloads.length % 5;
		
	var j;
	var start = downloadNumber * 5;
	for(j = 0; j < end; j++) {
		var counter;
		for(counter = 0; counter < 11; counter++)
			$(downloadList[j][counter]).sfLabel("option","text",allDownloads[j+start][counter]);
		
		$(progressBarList[j]).sfProgressBar('setValue', allDownloads[j+start][6]);
	}
}

function hideElement(index) {
	alert("SceneDownloads.hideElement()");
	var element = elementList[index];
	var i;
	for(i=0; i<element.length; i++)
		$(element[i]).sfLabel('hide');	
		
	var myElement = document.getElementById(progressBarList[index].substring(1));
	myElement.style.visibility="hidden";
	var myElement = document.getElementById(linesList[index]);
	myElement.style.visibility="hidden";
}

function showElement(index) {
	alert("SceneDownloads.showElement()");
	var element = elementList[index];
	var i;
	for(i=0; i<element.length; i++)
		$(element[i]).sfLabel('show');	
		
	var myElement = document.getElementById(progressBarList[index].substring(1));
	myElement.style.visibility="visible";
	var myElement = document.getElementById(linesList[index]);
	myElement.style.visibility="visible";
}

function focusElement(index) {
	alert("SceneDownloads.focusElement()");	
	var myElement = document.getElementById(linesList[index]);
	myElement.style.borderWidth="4px";
	myElement.style.borderBottomStyle="inset";
	myElement.style.borderBottomColor="#40e0d0";
}

function blurElement(index) {
	alert("SceneDownloads.blurElement()");	
	var myElement = document.getElementById(linesList[index]);
	myElement.style.borderWidth="2px";
	myElement.style.borderBottomStyle="groove";
	myElement.style.borderBottomColor="#e6e6fa";
}