const port = 59361;

export function pingSwarm() {
    sendHttpRequest('GET', `http://localhost:${port}/ping`)
    .then(responseJson => {
        chrome.storage.session.set({"previousPing": responseJson})
    })
    .catch(error => {
        console.log(error);
    })
}

export function querySwarm(query) {
    sendHttpRequest('POST', `http://localhost:${port}/query`, {
        query: query
    })
    .then((responseJson) => {
        chrome.storage.session.set({"previousQuery": responseJson})
    })
    .catch((error) => {
        console.log(error);
    })
}

export function sendUrl(url) {
    sendHttpRequest('POST', `http://localhost:${port}/url`, {
        url: url
    })
    .then(function (responseJson) {
        chrome.storage.session.set({"previousUrlPost": responseJson})
    })
    .catch(function (error) {
        console.log(error);
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