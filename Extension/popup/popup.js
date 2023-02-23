const port = 59361;

chrome.webRequest.onBeforeRequest.addListener(userSearched, {urls: ["<all_urls>"]});
document.getElementById('ping').addEventListener('click', function() {pingSwarm});
document.getElementById('getQuery').addEventListener('click', function() {querySwarm(document.getElementById('msg').value)});
document.getElementById('postUrl').addEventListener('click', function() {sendUrl(document.getElementById('msg').value)});
document.getElementById('clear').addEventListener('click', function() {
    document.getElementById('log').innerHTML = '';
});

function userSearched(details) {
    log(details.url);
}

function pingSwarm() {
    sendHttpRequest('GET', `http://localhost:${port}/ping`)
    .then(responseJson => {
        log(responseJson);
    })
    .catch(error => {
        log(error);
    })
    log('hi');
}

function querySwarm(query) {
    sendHttpRequest('POST', `http://localhost:${port}/query`, {
        query: query
    })
    .then(function (response) {
        log(response);
    })
    .catch(function (error) {
        log(error);
    })
}

function sendUrl(url) {
    sendHttpRequest('POST', `http://localhost:${port}/url`, {
        url: url
    })
    .then(function (response) {
        log(response);
    })
    .catch(function (error) {
        log(error);
    });
}

const sendHttpRequest = (method, url, data) => {
    return fetch(url, {
        method: method,
        body: JSON.stringify(data),
        headers: data ? {'Content-Type': 'application/json'} : {}
    }).then(response => {
        if (response.status >= 400) {
            response.json().then(errResData => {
                const error = new Error('Something went wrong!');
                error.data = errResData;
                throw error;
            });
        }
        return response.json();
    });
};

function log(msg) {
    console.log(msg);
    var e = document.createElement('pre');
    e.appendChild(document.createTextNode(typeof msg === 'object' ? JSON.stringify(msg) : msg));
    document.getElementById('log').appendChild(e);
}

var examples = {
    url: 'https://stackoverflow.com/questions/tagged/fibonacci',
    query: 'good breakfast food?'
};

Array.prototype.slice.call(document.querySelectorAll('[data-example]')).forEach(function(example) {
    example.addEventListener('click', function() {
        document.getElementById('msg').value = JSON.stringify(examples[example.dataset.example]);
    });
});
