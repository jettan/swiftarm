var PreviewVideoTutorial =
{
}

PreviewVideoTutorial.init = function (widgetID, callback, extra)
{
    var previewHTML = this.getPreviewHTML();
    
    callback(widgetID, previewHTML);
}

PreviewVideoTutorial.getPreviewHTML = function ()
{
    var previewHTML = "";
    
    previewHTML += "<div style='position: absolute; left: 10px; top: 34px; width: 310px; height: 128px>";
    previewHTML += "    <p style='text-align:center>Swift P2P TV</p>";
    previewHTML += "</div>";
    
    return previewHTML;
}
