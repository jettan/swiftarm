var request;
var infoRequest;
	
	response = "No response"
	request = new XMLHttpRequest();
	
	request.open("GET", url, true);
	request.onreadystatechange = processRequest;
	request.send(null);
	return response;
}

function getDownloadInfo() {
	infoRequest = new XMLHttpRequest();
	infoRequest.onreadystatechange = buildXML;
	infoRequest.open("GET", "http://130.161.158.52:1337/getDownloads", true);
	infoRequest.send();
}

function buildXML() {
var xmlDownload;
var downloadRow;
var cell;
	
	if(infoRequest.readyState == 4 && infoRequest.status == 200) 
			var result = infoRequest.responseXML;
			xmlDownload = "<center><table>";
			xmlDownload = xmlDownload +
						"<tr id='descriptions'>";
			xmlDownload = xmlDownload +
						"<td id ='filename', class='top'>Filename</td>";
			xmlDownload = xmlDownload +
						"<td id ='dspeed', class='top'>Download speed</td>";
			xmlDownload = xmlDownload +
						"<td id ='uspeed', class='top'>Upload speed</td>";
			xmlDownload = xmlDownload +
						"<td id ='progress', class='top'>Progress</td>";
			xmlDownload = xmlDownload +
						"</tr>";
			downloadRow = infoRequest.responseXML.documentElement.getElementsByTagName("DOWNLOAD");
			var i = 0;
			for(i = 0; i < downloadRow.length; i++) {
				xmlDownload = xmlDownload + "<tr id='download" + i + "'>";
				cell = downloadRow[i].getElementsByTagName("NAME"); {
					try {
						xmlDownload = xmlDownload + "<td id='filename" + i + "', class='name'>"
										+ cell[0].firstChild.nodeValue + "</td>";
					}
					catch(er) {
						xmlDownload = xmlDownload + "<td>&nbsp;</td>";
					}
				}
				cell = downloadRow[i].getElementsByTagName("DSPEED"); {
					try {
						xmlDownload = xmlDownload + "<td id='dspeed" + i + "', class='speed'>"
										+ cell[0].firstChild.nodeValue + "</td>";
					}
					catch(er) {
						xmlDownload = xmlDownload + "<td>&nbsp;</td>";
					}
				}
				cell = downloadRow[i].getElementsByTagName("USPEED"); {
					try {
						xmlDownload = xmlDownload + "<td id='uspeed" + i + "', class='speed'>"
										+ cell[0].firstChild.nodeValue + "</td>";
					} 
					catch(er) {
						xmlDownload = xmlDownload + "<td>&nbsp;</td>";
					}
				}
				cell = downloadRow[i].getElementsByTagName("PROGRESS"); {
					try {
						xmlDownload = xmlDownload + "<td id='progress" + i + "', class='progress'>"
										+ cell[0].firstChild.nodeValue + "</td>";
					}
					catch(er) {
						xmlDownload = xmlDownload + "<td>&nbsp;</td>";
					}
				}
			}
			xmlDownload = xmlDownload + "</table></center>";
			document.getElementById('Downloads').innerHTML = xmlDownload;
		}
}

function processRequest() {
	if (request.readyState == 4) {
		response = request.responseText;
	}
}
