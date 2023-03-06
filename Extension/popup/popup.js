import {pingSwarm} from '../swarm.js';

document.getElementById('ping').addEventListener('click', pingSwarm);

chrome.storage.onChanged.addListener((changes) => {
    if (changes.previousQuery) {
        loadLatestQuery();
    } else if (changes.previousUrlPost) {
        loadLatestUrlPost();
    } else if (changes.previousPing) {
        loadLatestPing();
    }
})

function loadLatestQuery() {
    document.getElementById('log').innerHTML = '';
    chrome.storage.session.get(["previousQuery"]).then((data) => {
        if (data.previousQuery && data.previousQuery.query && data.previousQuery.results){
            popupLog(`Swarm results for query: \"${data.previousQuery.query}\"`);
            data.previousQuery.results.map(popupLog);
        } else {
            popupLog("No queries made this session")
        }
    })
}

function loadLatestUrlPost() {
    document.getElementById('sentUrl').innerHTML = '';
    chrome.storage.session.get(["previousUrlPost"]).then((data) => {
        if (data.previousUrlPost && data.previousUrlPost.status && data.previousUrlPost.url){
            popupSentUrl(data.previousUrlPost.status + ' sending url: ' + data.previousUrlPost.url)
        } else {
            popupSentUrl("No urls sent this session")
        }
    })
}

function loadLatestPing() {
    document.getElementById('isConnected').innerHTML = '';
    chrome.storage.session.get(["previousPing"]).then((data) => {
        popupPing(data.previousPing && data.previousPing.status && data.previousPing.status ? 'connected to swarm' : 'swarm not reachable');
    })
}

function popupPing(msg) {
    var e = document.createElement('pre');
    e.appendChild(document.createTextNode(typeof msg === 'object' ? JSON.stringify(msg) : msg));
    document.getElementById('isConnected').appendChild(e);
}

function popupSentUrl(msg) {
    var e = document.createElement('pre');
    e.appendChild(document.createTextNode(typeof msg === 'object' ? JSON.stringify(msg) : msg));
    document.getElementById('sentUrl').appendChild(e);
}

function popupLog(msg) {
    var e = document.createElement('pre');
    e.appendChild(document.createTextNode(typeof msg === 'object' ? JSON.stringify(msg) : msg));
    document.getElementById('log').appendChild(e);
}

loadLatestQuery();
loadLatestUrlPost();
loadLatestPing();
