import express from 'express';
import cors from 'cors';
import fetch from 'node-fetch';
import * as pController from './src/peer-controller.mjs';

const app = express();
const port = 59361;
app.use(express.json());
app.use(cors());


app.get('/ping', (req, res) => {
    res.json({
        status: pController.pingSwarm()
    })
});

app.post('/query', (req, res) => {
    queryFasttext({
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        encoding: null,
        body: JSON.stringify({ query: req.body.query }),
    })
    .then(vector => {
        pController.search(vector)
        .then(results => {
            console.log(results);
            res.json({
                query: req.body.query,
                results: results.results
            })
        })
    })
});

app.post('/url', (req, res) => {
    fetch(`http://127.0.0.1:5000/embedUrl`, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({
            url: req.body.url
        })
    })
    .then(response => {
        console.log(response);
    })
    .catch(err => {
        console.log(err);
    })

    res.json({
        status: "success",
        url: req.body.url
    })
});

app.listen(port, (error) => {
    if (!error) {
        console.log(`Example app listening on port ${port}`)
    } else {
        console.log("Error occured launching server: " + port);
    }
});


async function queryFasttext(options) {
    try {
        const embedResponse = await fetch('http://127.0.0.1:5000/embedQuery', options);
        return await embedResponse.buffer();
    } catch (error) {
        console.error(error);
    }
}