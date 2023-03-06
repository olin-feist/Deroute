import fetch from 'node-fetch';

queryFasttext({
    method: 'POST',
    headers: {
        'Content-Type': 'application/json',
    },
    encoding: null,
    body: JSON.stringify({ query: "dogs" }),
})
.then(vector => {
    fetch(`http://127.0.0.1:5000/search`, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({
            dense_vector: vector
        })
    })
    .then(response => {
        console.log(response);
    }) 
})
.catch(err => {
    console.log(err);
}) 

async function queryFasttext(options) {
    try {
        const embedResponse = await fetch('http://127.0.0.1:5000/embedQuery', options);
        return await embedResponse.buffer();
    } catch (error) {
        console.error(error);
    }
}