// This file transforms your HTML code into a "string" that is read in under server.on()

const char* myHTML = R"=====(
<!DOCTYPE HTML>
<html>
<head>
  <title>MYGPT</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    /* CSS */
    /* Classes*/
    html { font-family: Arial, Helvetica, sans-serif; text-align: center; }
    h1 { font-size: 1.8rem; color: white; }
    .topnav { overflow: hidden; background-color: #143642; }
    body { margin: 0; }
    .content { padding: 30px; max-width: 600px; margin: 0 auto; }
    .card { background-color: #F8F7F9; box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5); padding-top:10px; padding-bottom:20px; }
    .button { padding: 15px 50px; font-size: 24px; text-align: center; outline: none; color: #fff; background-color: #0f8b8d; border: none; border-radius: 5px; user-select: none; }
    .button:active { background-color: #0f8b8d; box-shadow: 2 2px #CDCDCD; transform: translateY(2px); }
    .outputText { padding: 10px; font-size: 18px; width: 80%; margin: 0 auto; overflow: auto; }
  </style>
</head>
<body>
<div class="topnav">
  <h1>
    MyGPT Communications Services
  </h1>
</div>
<div class="content">
  <h2>
    Welcome to MyGPT! How can I be of assistance today?
  </h2>
</div>
<div class="content">
  <div class="card">
    <input type="text" id="inputText" placeholder="Type your message here...">
    <p><button id="sendButton" class="button">Send</button></p>
  </div>
</div>
<div class="content">
  <div class="card">
    <textarea id="outputText" class="outputText" rows="1" placeholder="Output text will appear here..." readonly></textarea>
  </div>
</div>

<script>
  // JavaScript
  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  window.addEventListener('load', onLoad);
  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage;
  }
  function onOpen(event) {
    console.log('Connection opened');
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }
  function onMessage(event) {
    console.log('Received message from server: ' + event.data);
    // Update the output text box with the received message
    var outputText = document.getElementById('outputText');
    outputText.value = ""; // Clear the output text box when result arrives
    outputText.value += event.data + '\n'; // Append the new message with a newline
    outputText.rows = outputText.value.split('\n').length; // Adjust the number of rows based on the number of lines
  }
  function onLoad(event) {
    initWebSocket();
    initButton();
  }
  function initButton() {
    document.getElementById('sendButton').addEventListener('click', sendMessage);
  }
  function sendMessage(){
    var input = document.getElementById('inputText').value;
    var outputText = document.getElementById('outputText');
    outputText.value = "Thinking..."; // Reset the output text box
    websocket.send(input);
  }
</script>
</body>
</html>

)=====";