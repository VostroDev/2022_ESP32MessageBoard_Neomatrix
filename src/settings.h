// Parent: 2022_ESPMessageBoard_Neamatrix.cpp
#include <pgmspace.h>

const char settingspage[] PROGMEM = R"=====(

  <!DOCTYPE html><html lang="en">
  <head><meta charset="UTF-8" name="viewport" content="width=device-width, initial-scale=1, user-scalable=no"/>
  <title>Message Board</title>
  
    <style>
      #snackbar {
        visibility: hidden;
        min-width: 280px;
        background-color: #333;
        color: #fff;
        text-align: center;
        border-radius: 4px;
        padding-top: 12px;
        padding-bottom: 12px;
        position: fixed;
        z-index: 1;
        bottom: 30px;
        font-size: 17px;
      }
  
      #snackbar.show {
        visibility: visible;
        -webkit-animation: fadein 0.5s, fadeout 0.5s 2.5s;
        animation: fadein 0.5s, fadeout 0.5s 2.5s;
      }
  
      @-webkit-keyframes fadein {
        from {bottom: 0; opacity: 0;} 
        to {bottom: 30px; opacity: 1;}
      }
  
      @keyframes fadein {
        from {bottom: 0; opacity: 0;}
        to {bottom: 30px; opacity: 1;}
      }
  
      @-webkit-keyframes fadeout {
        from {bottom: 30px; opacity: 1;} 
        to {bottom: 0; opacity: 0;}
      }
  
      @keyframes fadeout {
        from {bottom: 30px; opacity: 1;}
        to {bottom: 0; opacity: 0;}
      }
    h3{padding:5px;}
    input{width:250px;padding:5px;font-size:1em;text-align:left;}
    body{text-align: center;font-family:verdana;}
    button{border:0;border-radius:0.3rem;
        background-color:#1fa3ec;
        color:#fff;
        line-height:2.4rem;
        font-size:1.2rem;
        width:265px;}
    </style>
  
    <style>
      ul {
        list-style-type: none;
        margin: 0;
        padding: 0;
        overflow: hidden;
        background-color: #333;
      }
  
      li {
        float: left;
      }
  
      li a {
        display: block;
        color: white;
        text-align: center;
        padding: 14px 16px;
        text-decoration: none;
      }
  
      li a:hover:not(.active) {
        background-color: #111;
      }
  
      .active {
        background-color: #1fa3ec;
      }
    </style>
  
    <script>
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
        setTimeout(function() { window.location.href = "/"; }, 2000);
      }
  
      function transferFailed(evt)
      {
        var mySnack = document.getElementById("snackbar");
        mySnack.textContent  = "Oops, something went wrong..";
        mySnack.className = "show";
        setTimeout(function(){ mySnack.className = mySnack.className.replace("show", ""); }, 3000);
      }
    </script>
  
  </head>
  
  <body>
    <div style='display:inline-block;min-width:280px;'>
      <ul>
        <li><a href="/">Message</a></li>
        <li><a href="/time">Time</a></li>
        <li><a class="active" href="">Settings</a></li>
      </ul>
    </div>
    <div>
        <h3>Change Wifi password:</h3>
      <form id="data_form" name="frmText" autocomplete="off" action="javascript:SendPasswords()">
        <input 
            id="oldp" 
            type="text" 
            placeholder='enter old password' 
            maxlength="15" 
            pattern=".{8,}" 
            autocomplete="off"
            oninput="setCustomValidity(' ')"
            oninvalid="this.setCustomValidity('must be longer than 8 characters')"
            onchange="try{setCustomValidity('')}catch(e){}"
            required><br/><br/><br/>
        <input 
            id="newp" 
            type="text" 
            placeholder='enter new password' 
            maxlength="15" 
            pattern=".{8,}" 
            autocomplete="off"
            oninput="setCustomValidity(' ')"
            oninvalid="this.setCustomValidity('must be longer than 8 characters')"
            onchange="try{setCustomValidity('')}catch(e){}"
            required><br/><br/>
        <input 
            id="renewp" 
            type="text" 
            placeholder='re-enter new password' 
            maxlength="15" 
            pattern=".{8,}" 
            autocomplete="off"
            oninput="setCustomValidity(' ')"
            oninvalid="this.setCustomValidity('must be longer than 8 characters')"
            onchange="try{setCustomValidity('')}catch(e){}"
            required><br/><br/>
        <button type="submit" >submit</button>
      </form>
    </div>
    <div style='display:inline-block;min-width:280px;'>
      <div id="snackbar">Display by. Vostro</div>
    </div>
    
  </body>
  
  </html>

)=====";