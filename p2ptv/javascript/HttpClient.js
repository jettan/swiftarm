var request;

HttpClient.httpGet(url) = function {
	response = "No response";
	request = new XMLHttpRequest();
	
	request.open("GET", url, true);
	request.onreadystatechange = processRequest;
	request.send(null);
	return response;
}

function processRequest() {
	if (request.readyState == 4) {
		response = request.responseText;
	}
}
