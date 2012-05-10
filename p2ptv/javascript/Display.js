/**
 * The display.
 */
var Display = {
	statusDiv : null,
	FIRSTIDX : 0,
	LASTIDX : 4,
	currentWindow : 0,
	
	SELECTOR : 0,
	LIST : 1,
	
	videoList : new Array()
}

/**
 * Initialise the display.
 */
Display.init = function() {
	var success = true;
	
	this.statusDiv = document.getElementById("status");
	
	if (!this.statusDiv) {
		success = false;
	}
	
	return success;
}

/**
 * Set the total duration of the video on display.
 * @param total: The duration of the video to be set on display in seconds.
 */
Display.setTotalTime = function(total) {
	this.totalTime = total;
}

/**
 * Set the time positions of the video on display.
 */
Display.setTime = function(time) {
	var timePercent     = (100 * time) / this.totalTime;
	var timeElement     = document.getElementById("timeInfo");
	var timeHTML        = "";
	var timeHour        = 0;
	var timeMinute      = 0;
	var timeSecond      = 0;
	var totalTimeHour   = 0;
	var totalTimeMinute = 0;
	var totalTimesecond = 0;
	
	document.getElementById("progressBar").style.width = timePercent + "%";
	
	if(Player.state == Player.PLAYING) {
		totalTimeHour = Math.floor(this.totalTime / 3600000);
		timeHour = Math.floor(time / 3600000);
		
		totalTimeMinute = Math.floor((this.totalTime % 3600000) / 60000);
		timeMinute = Math.floor((time % 3600000) / 60000);
		
		totalTimeSecond = Math.floor((this.totalTime % 60000) / 1000);
		timeSecond = Math.floor((time % 60000) / 1000);
		
		timeHTML = timeHour + ":";
		
		if (timeMinute == 0) {
			timeHTML += "00:";
		} else if(timeMinute < 10) {
			timeHTML += "0" + timeMinute + ":";
		} else {
			timeHTML += timeMinute + ":";
		}
		
		if(timeSecond == 0) {
			timeHTML += "00/";
		} else if(timeSecond < 10) {
			timeHTML += "0" + timeSecond + "/";
		} else {
			timeHTML += timeSecond + "/";
		}
		
		timeHTML += totalTimeHour + ":";
		
		if(totalTimeMinute == 0) {
			timeHTML += "00:";
		} else if(totalTimeMinute < 10) {
			timeHTML += "0" + totalTimeMinute;
		} else {
			timeHTML += totalTimeMinute;
		}
		
		if(totalTimeSecond == 0) {
			timeHTML += "00";
		} else if(totalTimeSecond < 10) {
			timeHTML += "0" + totalTimeSecond;
		} else {
			timeHTML += totalTimeSecond;
		}
	} else {
		timeHTML = "0:00:00/0:00:00";
	}
	
	widgetAPI.putInnerHTML(timeElement, timeHTML);
}

/**
 * Set text in the status box.
 * @param status: The text to be shown.
 */
Display.status = function(status) {
	widgetAPI.putInnerHTML(this.statusDiv, status);
}

/**
 * Sets the volume on display.
 * @param level: The volume level.
 */
Display.setVolume = function(level) {
	document.getElementById("volumeBar").style.width = level + "%";
	
	var volumeElement = document.getElementById("volumeInfo");
	
	widgetAPI.putInnerHTML(volumeElement, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" + Audio.getVolume());
}

/**
 * Set the video list on display.
 * @param nameList: List of names that should appear on the list.
 */
Display.setVideoList = function(nameList) {
	var listHTML = "";
	var i        = 0;
	
	// Put all names found from nameList in videoList.
	for (var name in nameList) {
		this.videoList[i] = document.getElementById("video" + i);
		listHTML = nameList[name] ;
		widgetAPI.putInnerHTML(this.videoList[i], listHTML);
		i++;
	}
	
	this.videoList[this.FIRSTIDX].style.backgroundImage= "url(Images/listBox/selector.png)";
	
	// We only have space for 5 items, so if there are more items, enable scrolling.
	if( i > 5) {
		document.getElementById("next").style.opacity = '1.0';
		document.getElementById("previous").style.opacity = '1.0';
	}
	
	listHTML = "1 / " + i;
	widgetAPI.putInnerHTML(document.getElementById("videoCount"), listHTML);
}

/**
 * Handle moving in the videoList.
 * @param position: The current position.
 * @param move: The direction to move.
 */
Display.setVideoListPosition = function(position, move) {
	var listHTML = "";
	
	listHTML = (position + 1) + " / " + Data.getVideoCount();
	widgetAPI.putInnerHTML(document.getElementById("videoCount"), listHTML);
	
	// Deal with videoList with 5 elements or more.
	if (Data.getVideoCount() < 5) {
		for (var i = 0; i < Data.getVideoCount(); i++) {
			if (i == position) {
				this.videoList[i].style.backgroundImage= "url(Images/listBox/selector.png)";
			} else {
				this.videoList[i].style.backgroundImage= "url(none)";
			}
		}
	} else if ((this.currentWindow != this.LASTIDX && move == Main.DOWN) || (this.currentWindow != this.FIRSTIDX && move == Main.UP)) {
		if (move == Main.DOWN) {
			this.currentWindow ++;
		} else {
			this.currentWindow --;
		}
		
		for (var i = 0; i <= this.LASTIDX; i++) {
			if (i == this.currentWindow) {
				this.videoList[i].style.backgroundImage= "url(Images/listBox/selector.png)";
			} else {
				this.videoList[i].style.backgroundImage= "url(none)";
			}
		}
	} else if (this.currentWindow == this.LASTIDX && move == Main.DOWN) {
		if (position == this.FIRSTIDX) {
			this.currentWindow = this.FIRSTIDX;
			
			for (var i = 0; i <= this.LASTIDX; i++) {
				listHTML = Data.videoNames[i] ;
				widgetAPI.putInnerHTML(this.videoList[i], listHTML);
				
				if (i == this.currentWindow) {
					this.videoList[i].style.backgroundImage= "url(Images/listBox/selector.png)";
				} else {
					this.videoList[i].style.backgroundImage= "url(none)";
				}
			}
		} else {
			for (var i = 0; i <= this.LASTIDX; i++) {
				listHTML = Data.videoNames[i + position - this.currentWindow] ;
				widgetAPI.putInnerHTML(this.videoList[i], listHTML);
			}
		}
	} else if (this.currentWindow == this.FIRSTIDX && move == Main.UP) {
		if (position == Data.getVideoCount() - 1) {
			this.currentWindow = this.LASTIDX;
			
			for (i = 0; i <= this.LASTIDX; i++) {
				listHTML = Data.videoNames[i + position - this.currentWindow] ;
				widgetAPI.putInnerHTML(this.videoList[i], listHTML);
				
				if (i == this.currentWindow) {
					this.videoList[i].style.backgroundImage= "url(Images/listBox/selector.png)";
				} else {
					this.videoList[i].style.backgroundImage= "url(none)";
				}
			}
		} else {
			for(i = 0; i <= this.LASTIDX; i++) {
				listHTML = Data.videoNames[i + position] ;
				widgetAPI.putInnerHTML(this.videoList[i], listHTML);
			}
		}
	}
}

Display.setDescription = function(description) {
	var descriptionElement = document.getElementById("description");
    
    widgetAPI.putInnerHTML(descriptionElement, description);
}

Display.hide = function()
{
    document.getElementById("main").style.display="none";
}

Display.show = function()
{
    document.getElementById("main").style.display="block";
}

