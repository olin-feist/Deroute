const express = require('express');
const cors = require('cors');
const app = express();
const port = 59361;
app.use(express.json());
app.use(cors());


app.get('/ping', (req, res) => {
    res.json({
        status: "success"
    })
});

app.post('/query', (req, res) => {
    if (req.body) {
        console.log(req.body);
    }
    res.json({
        query: req.body.query,
        results: [
            {
                url: "https://www.wikihow.com/Tie-Your-Shoes",
                relevance: 0.5
            },
            {
                url: "https://www.birdwatchingusa.org/do-pigeons-have-feelings",
                relevance: 0.3
            }
        ]
    })
});

app.post('/url', (req, res) => {
    if (req.body) {
        console.log(req.body);
    }
    res.json({
        status: "Success",
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