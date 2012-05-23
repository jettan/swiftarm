var Data =
{
    videoNames : [ ],
    videoURLs : [ ],
    videoDescriptions : [ ]
}

Data.setVideoNames = function(list)
{
    this.videoNames = list;
}

Data.setVideoURLs = function(list)
{
    this.videoURLs = list;
}

Data.setVideoDescriptions = function(list)
{
    this.videoDescriptions = list;
}

Data.getVideoURL = function(index)
{
    var url = this.videoURLs[index];
    
    if (url)    // Check for undefined entry (outside of valid array)
    {
        return url;
    }
    else
    {
        return null;
    }
}

Data.getVideoCount = function()
{
    return this.videoURLs.length;
}

Data.getVideoNames = function()
{
    return this.videoNames;
}

Data.getVideoDescription = function(index)
{
    var description = this.videoDescriptions[index];
    
    if (description)    // Check for undefined entry (outside of valid array)
    {
        return description;
    }
    else
    {
        return "No description";
    }
}
