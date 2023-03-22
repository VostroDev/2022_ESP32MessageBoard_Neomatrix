
function SendPasswords()
{
  console.log("save button was clicked!");

  var oldp = document.getElementById("oldp").value;
  var newp = document.getElementById("newp").value;
  var renewp = document.getElementById("renewp").value;

  if(newp != renewp) {  //Passwords Don't Match
    alert("Error: New password and confirmation password don't match");
    return false;
  } else {
    var data = {oldpassword:oldp, newpassword:newp, renewpassword:renewp};

    var xhr = new XMLHttpRequest();
    var url = "/settings/send";

    xhr.addEventListener("load", transferComplete);
    xhr.addEventListener("error", transferFailed);
  
    xhr.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        // Typical action to be performed when the document is ready:
        if(xhr.responseText != null){
          console.log(xhr.responseText);
          alert(xhr.responseText);
        }
      }
    };
 
    xhr.open("POST", url, true);
    xhr.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
    xhr.send(JSON.stringify(data));
    return true;
    }
}
//new message: {"oldpassword":"123","newpassword":"456","renewpassword":"789"}

function transferComplete(evt)
{
  var mySnack = document.getElementById("snackbar");
  mySnack.textContent  = "Data successfully sent";
  mySnack.className = "show";
  setTimeout(function(){ mySnack.className = mySnack.className.replace("show", ""); }, 3000);
  /* setTimeout(function(){ window.location.href = "/"; }, 2000); */
}

function transferFailed(evt)
{
  var mySnack = document.getElementById("snackbar");
  mySnack.textContent  = "Oops, something went wrong..";
  mySnack.className = "show";
  setTimeout(function(){ mySnack.className = mySnack.className.replace("show", ""); }, 3000);
}

function OTA()
{
  window.open("/update","_self")
}

function advancedSettings()
{
  window.open("/advanced","_self")
}
