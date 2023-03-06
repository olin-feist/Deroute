import {querySwarm, sendUrl} from './swarm.js';

let canMakeQuery = true;

chrome.webRequest.onBeforeRequest.addListener(details => {
    if (!canMakeQuery) {
        return
    }
    canMakeQuery = false;
    if (details.method != 'GET') {
        return
    }
    const query = details.url.substring(details.url.indexOf('=')+1, details.url.indexOf('&'));
    console.log(`User search for \"${query}\"`);
    querySwarm(query);
    canMakeQuery = true;
}, {urls: [
    "https://duckduckgo.com/?q=*&*",
    "https://www.google.com/search?q=*&*"
]});

chrome.history.onVisited.addListener((historyItem) => {
    if(historyItem.url.search('https:\/\/www.google.com\/search\?') == -1 && historyItem.url.search('https:\/\/duckduckgo.com\/\?q=') == -1) {
        sendUrl(historyItem.url)
    }
})