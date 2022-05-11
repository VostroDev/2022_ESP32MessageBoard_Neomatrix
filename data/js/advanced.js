function rgbToObj(rgb) {
  
  let colors = ["red", "green", "blue"]

  let colorArr = rgb.slice(
      rgb.indexOf("(") + 1, 
      rgb.indexOf(")")
  ).split(", ");

  let obj = new Object();

  colorArr.forEach((k, i) => {
      obj[colors[i]] = k
  })

  return obj;
}

function SendData() {
  console.log("save button was clicked!");
  
  var timedur = $('#timedur').find(":selected").val();
  var timecolor = rgbToObj($("#timecolor").css('Color'));
  var tempcolor = rgbToObj($("#tempcolor").css('Color'));
  var dowcolor  = rgbToObj($("#dowcolor").css('Color'));
  var datecolor = rgbToObj($("#datecolor").css('Color'));
  var msgcolor  = rgbToObj($("#msgcolor").css('Color'));
  var msgspeed = $('#msgspeed').find(":selected").val();
  var msgonly = "0";
  if ($('#msgonly').is(":checked")) {
    msgonly = "1";
  }

  var json = JSON.stringify({ 'timedur':timedur,
                              'timecolor':[timecolor['red'], timecolor['green'], timecolor['blue']] , 
                              'tempcolor':[tempcolor['red'], tempcolor['green'], tempcolor['blue']] ,                            
                              'dowcolor':[dowcolor['red'],  dowcolor['green'], dowcolor['blue']] , 
                              'datecolor':[datecolor['red'], datecolor['green'], datecolor['blue']] , 
                              'msgcolor':[msgcolor['red'], msgcolor['green'], msgcolor['blue']] , 
                              'msgspeed':msgspeed,
                              'msgonly':msgonly
                            });

  //console.log("Data sent: " + json);

  var xhr = new XMLHttpRequest();
  var url = "/advanced/send";

  xhr.addEventListener("load", transferComplete);
  xhr.addEventListener("error", transferFailed);

  xhr.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      // Typical action to be performed when the document is ready:
      if (xhr.responseText != null) {
        console.log(xhr.responseText);
        //alert(xhr.responseText);
      }
    }
  };

  xhr.open("POST", url, true);
  xhr.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
  xhr.send(json);
  return true;
}

function transferComplete(evt) {
  var mySnack = document.getElementById("snackbar");
  mySnack.textContent = "Data successfully sent";
  mySnack.className = "show";
  setTimeout(function () { mySnack.className = mySnack.className.replace("show", ""); }, 3000);
  // setTimeout(function () { window.location.href = "/"; }, 2000);
}

function transferFailed(evt) {
  var mySnack = document.getElementById("snackbar");
  mySnack.textContent = "Oops, something went wrong..";
  mySnack.className = "show";
  setTimeout(function () { mySnack.className = mySnack.className.replace("show", ""); }, 3000);
}

$("#timecolor").colorPick({
  'allowRecent': false,
  'allowCustomColor': false,
  'paletteLabel': 'LED COLOUR:',
  'initialColor' : '#000000', 
  'palette': ["#000000", "#ff0000", "#00ff00", "#0000ff", "#ffffff", "#00c864", "#64dd17", "#008080", "#00bcd4", "#00186f", "#d35400", "#e81e62", "#e91e63", "#d500f9","#9c27b0"],
  'onColorSelected': function() {
    if(this.color == '#000000'){
      console.log("The user has selected the color: Rainbow")
      this.element.css({'backgroundColor': 'Black', 'color': 'Black', 'background': 'linear-gradient(138deg, #ebf95e, #65f540, #2e85b5, #9147b5, #9a41b5, #fd1d1d, #fcb045)'});
    }
    else{
      console.log("The user has selected the color: " + this.color)
      this.element.css({'backgroundColor': this.color, 'color': this.color, 'background': this.color});
    }
  }
});

$("#tempcolor").colorPick({
  'initialColor' : '#00c864',
  'palette': ["#000000", "#ff0000", "#00ff00", "#0000ff", "#ffffff", "#00c864", "#64dd17", "#008080", "#00bcd4", "#00186f", "#d35400", "#e81e62", "#e91e63", "#d500f9","#9c27b0"],
  'onColorSelected': function() {
    if(this.color == '#000000'){
      console.log("The user has selected the color: Rainbow")
      this.element.css({'backgroundColor': 'Black', 'color': 'Black', 'background': 'linear-gradient(138deg, #ebf95e, #65f540, #2e85b5, #9147b5, #9a41b5, #fd1d1d, #fcb045)'});
    }
    else{
      console.log("The user has selected the color: " + this.color)
      this.element.css({'backgroundColor': this.color, 'color': this.color, 'background': this.color});
    }
  }
});

$("#dowcolor").colorPick({
  'initialColor' : '#d35400',
  'palette': ["#000000", "#ff0000", "#00ff00", "#0000ff", "#ffffff", "#00c864", "#64dd17", "#008080", "#00bcd4", "#00186f", "#d35400", "#e81e62", "#e91e63", "#d500f9","#9c27b0"],
  'onColorSelected': function() {
    if(this.color == '#000000'){
      console.log("The user has selected the color: Rainbow")
      this.element.css({'backgroundColor': 'Black', 'color': 'Black', 'background': 'linear-gradient(138deg, #ebf95e, #65f540, #2e85b5, #9147b5, #9a41b5, #fd1d1d, #fcb045)'});
    }
    else{
      console.log("The user has selected the color: " + this.color)
      this.element.css({'backgroundColor': this.color, 'color': this.color, 'background': this.color});
    }
  }
});

$("#datecolor").colorPick({
  'initialColor' : '#008080',
  'palette': ["#000000", "#ff0000", "#00ff00", "#0000ff", "#ffffff", "#00c864", "#64dd17", "#008080", "#00bcd4", "#00186f", "#d35400", "#e81e62", "#e91e63", "#d500f9","#9c27b0"],
  'onColorSelected': function() {
    if(this.color == '#000000'){
      console.log("The user has selected the color: Rainbow")
      this.element.css({'backgroundColor': 'Black', 'color': 'Black', 'background': 'linear-gradient(138deg, #ebf95e, #65f540, #2e85b5, #9147b5, #9a41b5, #fd1d1d, #fcb045)'});
    }
    else{
      console.log("The user has selected the color: " + this.color)
      this.element.css({'backgroundColor': this.color, 'color': this.color, 'background': this.color});
    }
  }
});

$("#msgcolor").colorPick({
  'initialColor' : '#000000',
  'palette': ["#000000", "#ff0000", "#00ff00", "#0000ff", "#ffffff", "#00c864", "#64dd17", "#008080", "#00bcd4", "#00186f", "#d35400", "#e81e62", "#e91e63", "#d500f9","#9c27b0"],
  'onColorSelected': function() {
    if(this.color == '#000000'){
      console.log("The user has selected the color: Rainbow")
      this.element.css({'backgroundColor': 'Black', 'color': 'Black', 'background': 'linear-gradient(138deg, #ebf95e, #65f540, #2e85b5, #9147b5, #9a41b5, #fd1d1d, #fcb045)'});
    }
    else{
      console.log("The user has selected the color: " + this.color)
      this.element.css({'backgroundColor': this.color, 'color': this.color, 'background': this.color});
    }
  }
});


/*
char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";

 "{sensor:gps,time:1351824120,data:[48.756080,2.302038]}";

DynamicJsonDocument doc(1024);
deserializeJson(doc, json);

const char* sensor = doc["sensor"];
long time          = doc["time"];
double latitude    = doc["data"][0];
double longitude   = doc["data"][1];
*/

