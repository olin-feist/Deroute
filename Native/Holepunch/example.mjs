import fetch from 'node-fetch';

queryFasttext({
    method: 'POST',
    headers: {
        'Content-Type': 'application/json',
    },
    encoding: null,
    body: JSON.stringify({ query: "dog" }),
})
.then(vector => {
    fetch(`http://127.0.0.1:5000/search`, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: vector
    })
    .then(response => {
        return response.json(); 
    })
    .then(data => {
        console.log(data);
    })
    .catch(error => {
        console.error(error);
    })
})
.catch(err => {
    console.log(err);
})

async function queryFasttext(options) {
    try {
        const embedResponse = await fetch('http://127.0.0.1:5000/embedQuery', options);
        return await embedResponse.arrayBuffer();
    } catch (error) {
        console.error(error);
    }
}