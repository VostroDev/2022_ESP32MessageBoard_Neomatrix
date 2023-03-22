      
function SendData()
{
  console.log("save button was clicked!");

  var xmessage = document.getElementById("data_form").Message.value;
  var xbrightness = document.getElementById("myRange").value;

  var data = {brightness:xbrightness, message:xmessage};

  var xhr = new XMLHttpRequest();
  var url = "/message";

  xhr.addEventListener("load", transferComplete);
  xhr.addEventListener("error", transferFailed);

  xhr.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      // Typical action to be performed when the document is ready:
      if(xhr.responseText != null){
        console.log(xhr.responseText);
        //? alert(xhr.responseText);
      }
    }
  };

  xhr.open("POST", url, true);
  xhr.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
  xhr.send(JSON.stringify(data));
  return true;
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

function rangeSlideRW(value) {
    document.getElementById('rangeValueRW').innerHTML = value; 
}
