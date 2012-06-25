alert('init.js included');
function onStart() {
	alert('onStart()');
	// TODO : Add your Initilize code here
	if (g_testVariable) {
		sf.scene.show('Main');
		sf.scene.focus('Main');
	}
	else {
		alert("############################################################");
		alert("app.json files property is not handled well!!!!!!!!!!!!!!!!");
		alert("############################################################");
	}
}

function onDestroy () {
	alert('onDestroy()');
	// stop your XHR or Ajax operation and put codes to distroy your application here

}

function getObjString(obj, prefix) {
	var result = "";
	try {
		for (var elmt in obj) {
			var pre = '';
			if (obj.constructor == Array) {
				pre = prefix + "[" + elmt + "]";
			}
			else {
				pre = prefix + "." + elmt;
			}
			
			if (typeof obj[elmt] == 'object') {
				result += getObjString(obj[elmt], pre);
			}
			else {
				result += pre + " = " + obj[elmt] + "<br>";
			}
		}
	}
	catch (e)
	{
		alert(e.message+" - "+e.filename+" ("+e.linenumber+")");
	}

	return result;
}

function makeBreakable (string) {
	var MAX_WORD_LENGTH = 3;
	var retValue = "";

	var breaker = "<wbr/>";
	var breakCounter = 0;
	for(var i=0; i<string.length; i++) {
		if (string.charAt(i) == " ") {
			retValue += '&nbsp;';
			breakCounter = 0;
		}
		else {
			if (breakCounter >= MAX_WORD_LENGTH && i < string.length-1) {
				retValue += breaker
				breakCounter = 0;
			}
			retValue += string.charAt(i);
			breakCounter++;
		}
	}
	return retValue;
}

alert("init.js loaded.");
