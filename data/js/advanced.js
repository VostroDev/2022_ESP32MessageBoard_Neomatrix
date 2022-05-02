function SendPasswords() {
  console.log("save button was clicked!");

  var oldp = document.getElementById("oldp").value;
  var newp = document.getElementById("newp").value;
  var renewp = document.getElementById("renewp").value;

  if (newp != renewp) {
    //Passwords Don't Match
    alert("Error: New password and confirmation password don't match");
    return false;
  } else {
    var data = { oldpassword: oldp, newpassword: newp, renewpassword: renewp };

    var xhr = new XMLHttpRequest();
    var url = "/settings/send";

    xhr.addEventListener("load", transferComplete);
    xhr.addEventListener("error", transferFailed);

    xhr.onreadystatechange = function () {
      if (this.readyState == 4 && this.status == 200) {
        // Typical action to be performed when the document is ready:
        if (xhr.responseText != null) {
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

function transferComplete(evt) {
  var mySnack = document.getElementById("snackbar");
  mySnack.textContent = "Data successfully sent";
  mySnack.className = "show";
  setTimeout(function () { mySnack.className = mySnack.className.replace("show", ""); }, 3000);
  setTimeout(function () { window.location.href = "/"; }, 2000);
}

function transferFailed(evt) {
  var mySnack = document.getElementById("snackbar");
  mySnack.textContent = "Oops, something went wrong..";
  mySnack.className = "show";
  setTimeout(function () { mySnack.className = mySnack.className.replace("show", ""); }, 3000);
}

function OTA() {
  window.open("/update", "_self");
}

$("#timecolor").colorPick({
  'allowRecent': true,
  'allowCustomColor': false,
  'paletteLabel': 'LED COLOUR:',
  'initialColor' : '#ECF0F1',
  'palette': ["#ffffff", "#16a085", "#2ecc71", "#27ae60", "#3498db", "#2980b9", "#9b59b6", "#8e44ad", "#34495e", "#2c3e50", "#f1c40f", "#f39c12", "#e67e22", "#d35400", "#e74c3c", "#c0392b", "#ecf0f1"],
  'onColorSelected': function() {
    if(this.color == '#FFFFFF'){
      console.log("The user has selected the color: Rainbow")
      this.element.css({'backgroundColor': 'White', 'color': 'White', 'background': 'linear-gradient(138deg, #ebf95e, #65f540, #2e85b5, #9147b5, #9a41b5, #fd1d1d, #fcb045)'});
    }
    else{
      console.log("The user has selected the color: " + this.color)
      this.element.css({'backgroundColor': this.color, 'color': this.color, 'background': this.color});
    }
  }
});

$("#tempcolor").colorPick({
  'initialColor' : '#00c864',
  'palette': ["#ffffff", "#16a085", "#2ecc71", "#27ae60", "#3498db", "#2980b9", "#9b59b6", "#8e44ad", "#34495e", "#2c3e50", "#f1c40f", "#f39c12", "#e67e22", "#d35400", "#e74c3c", "#c0392b", "#ecf0f1"],
  'onColorSelected': function() {
    if(this.color == '#FFFFFF'){
      console.log("The user has selected the color: Rainbow")
      this.element.css({'backgroundColor': 'White', 'color': 'White', 'background': 'linear-gradient(138deg, #ebf95e, #65f540, #2e85b5, #9147b5, #9a41b5, #fd1d1d, #fcb045)'});
    }
    else{
      console.log("The user has selected the color: " + this.color)
      this.element.css({'backgroundColor': this.color, 'color': this.color, 'background': this.color});
    }
  }
});

$("#dowcolor").colorPick({
  'initialColor' : '#d35400',
  'palette': ["#ffffff", "#16a085", "#2ecc71", "#27ae60", "#3498db", "#2980b9", "#9b59b6", "#8e44ad", "#34495e", "#2c3e50", "#f1c40f", "#f39c12", "#e67e22", "#d35400", "#e74c3c", "#c0392b", "#ecf0f1"],
  'onColorSelected': function() {
    if(this.color == '#FFFFFF'){
      console.log("The user has selected the color: Rainbow")
      this.element.css({'backgroundColor': 'White', 'color': 'White', 'background': 'linear-gradient(138deg, #ebf95e, #65f540, #2e85b5, #9147b5, #9a41b5, #fd1d1d, #fcb045)'});
    }
    else{
      console.log("The user has selected the color: " + this.color)
      this.element.css({'backgroundColor': this.color, 'color': this.color, 'background': this.color});
    }
  }
});

$("#datecolor").colorPick({
  'initialColor' : '#008080',
  'palette': ["#ffffff", "#16a085", "#2ecc71", "#27ae60", "#3498db", "#2980b9", "#9b59b6", "#8e44ad", "#34495e", "#2c3e50", "#f1c40f", "#f39c12", "#e67e22", "#d35400", "#e74c3c", "#c0392b", "#ecf0f1"],
  'onColorSelected': function() {
    if(this.color == '#FFFFFF'){
      console.log("The user has selected the color: Rainbow")
      this.element.css({'backgroundColor': 'White', 'color': 'White', 'background': 'linear-gradient(138deg, #ebf95e, #65f540, #2e85b5, #9147b5, #9a41b5, #fd1d1d, #fcb045)'});
    }
    else{
      console.log("The user has selected the color: " + this.color)
      this.element.css({'backgroundColor': this.color, 'color': this.color, 'background': this.color});
    }
  }
});

$("#msgcolor").colorPick({
  'initialColor' : '#ECF0F1',
  'palette': ["#ffffff", "#16a085", "#2ecc71", "#27ae60", "#3498db", "#2980b9", "#9b59b6", "#8e44ad", "#34495e", "#2c3e50", "#f1c40f", "#f39c12", "#e67e22", "#d35400", "#e74c3c", "#c0392b", "#ecf0f1"],
  'onColorSelected': function() {
    if(this.color == '#FFFFFF'){
      console.log("The user has selected the color: Rainbow")
      this.element.css({'backgroundColor': 'White', 'color': 'White', 'background': 'linear-gradient(138deg, #ebf95e, #65f540, #2e85b5, #9147b5, #9a41b5, #fd1d1d, #fcb045)'});
    }
    else{
      console.log("The user has selected the color: " + this.color)
      this.element.css({'backgroundColor': this.color, 'color': this.color, 'background': this.color});
    }
  }
});


