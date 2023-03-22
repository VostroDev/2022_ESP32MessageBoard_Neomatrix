
Number.prototype.pad = function(size) {
  var s = String(this);
  while (s.length < (size || 2)) {s = "0" + s;}
  return s;
}

function SendTime()
{
  console.log("time button was clicked!");

  var d = new Date();
  var dY = d.getFullYear();
  var dM = (d.getMonth() + 1).pad(2);
  var dD = d.getDate().pad(2);
  var dh = d.getHours().pad(2);
  var dm = d.getMinutes().pad(2);

  var message = String(dD)+"."+String(dM)+"."+String(dY)+" "+String(dh)+":"+String(dm);
  console.log(message);

  var xhr = new XMLHttpRequest();
  var url = "/time/send";

  xhr.addEventListener("load", transferComplete);
  xhr.addEventListener("error", transferFailed);

  xhr.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      // Typical action to be performed when the document is ready:
      if(xhr.responseText != null){
        console.log(xhr.responseText);
      }
    }
  };

  xhr.open("POST", url, true);
  xhr.send(message);
}

function CustomTime()
{
  window.open("/timepicker","_self")
}

function transferComplete(evt)
{
  var mySnack = document.getElementById("snackbar");
  mySnack.textContent  = "Data successfully sent";
    mySnack.className = "show";
    setTimeout(function(){ mySnack.className = mySnack.className.replace("show", ""); }, 3000);
}

function transferFailed(evt)
{
  var mySnack = document.getElementById("snackbar");
  mySnack.textContent  = "Oops, something went wrong..";
    mySnack.className = "show";
    setTimeout(function(){ mySnack.className = mySnack.className.replace("show", ""); }, 3000);
}
