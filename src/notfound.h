// Parent: 2022_ESPMessageBoard_Neamatrix.cpp
#include <pgmspace.h>

const char notfoundpage[] PROGMEM = R"=====(
  
  
  <!DOCTYPE html><html lang="en">
  <head><meta charset="UTF-8" name="viewport" content="width=device-width, initial-scale=1, user-scalable=no"/>
  <title>Message Board</title>
  
    <style>
      body {
        height: 100%;
        text-align: center;font-family:verdana;
      }
      body {
        display: flex;
        align-items: center;
        justify-content: center;
        font-family:"Nunito Sans";
        color:#0e0620;
        font-size: 1em;
      }
      button {
        font-family:"Nunito Sans";
      }
      ul {
        list-style-type: none;
        padding-inline-start: 35px;
      }
      h1 {
        font-size: 6.5em;
        margin: 15px 0px;
        font-weight:bold;
      }
      h2 {
        font-weight:bold;
        font-size: 2em;
      }
      p {
        word-wrap: break-word;
        width: 270px;
        text-align: center;
        
      }
      button{border:0;border-radius:0.3rem;
        text-align: center;font-family:verdana;
        background-color:#1fa3ec;
        color:#fff;
        line-height:2.4rem;
        font-size:1.2rem;
        width:265px;
      }
      
    </style>
 
  </head>
  
  <body>
    <div class="container">
      <div class="row">
        
        <div class="col-md-6 align-self-center">
          <h1>404</h1>
          <h2>UH OH! You're lost.</h2>
          <p>The page you are looking for does not exist.
            How you got here is a mystery. But you can click the button below
            to go back to the homepage.
          </p>
          <button onclick="location.href='/'" type="button">HOME</button>
        </div>
      </div>
    </div>
    
  </body>
  
  </html>

  )=====";


  