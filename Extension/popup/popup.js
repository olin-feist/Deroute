

var application = 'deroute_controller';
var port = null;

document.getElementById('reconnect').addEventListener('click', reconnect);
document.getElementById('disconnect').addEventListener('click', disconnect);
document.getElementById('send').addEventListener('click', function() {send(document.getElementById('msg').value)});
document.getElementById('clear').addEventListener('click', function() {
    document.getElementById('log').innerHTML = '';
});



function reconnect() {
    disconnect();
    log('Attempting connection with native application');

    try {
        port = chrome.runtime.connectNative(application);
    } catch {
        return log("Failed to connect with native application");
    }

    port.onMessage.addListener(receiveMsg);

    port.onDisconnect.addListener(function(e) {
        log('Unexpected disconnect with native application');
        port = null;
    });
}

function disconnect() {
    try {
        port.disconnect();
        log('port.disconnect');// this doesn't seem to trigger the onDisconnect event
        port = null;
    } catch {

    }
}

function send(msgAsJson) {
    var msg;

    try {
        msg = JSON.parse(msgAsJson);
    } catch (err) {
        return log('invalid JSON: ' + json);
    }

    if (!port) {
        reconnect();
        if (!port) {
            return log("Failed to send: " + msg);
        }
    }
    port.postMessage(msg);
    log('Sent: ');
    log(msg);
}

function receiveMsg(msg){
    log("Recevied: ")
    log(msg);
}

function log(msg) {
    console.log(msg);

    var e = document.createElement('pre');
    e.appendChild(document.createTextNode(typeof msg === 'object' ? JSON.stringify(msg) : msg));
    document.getElementById('log').appendChild(e);
}

var examples = {
    ping: { ping: 'pong' },
    url: { url: 'https://stackoverflow.com/questions/tagged/fibonacci' },
    query: { query: 'good breakfast food?' }
};

Array.prototype.slice.call(document.querySelectorAll('[data-example]')).forEach(function(example) {
    example.addEventListener('click', function() {
        document.getElementById('msg').value = JSON.stringify(examples[example.dataset.example]);
    });
});
