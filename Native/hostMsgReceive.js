const express = require('express');
const cors = require('cors');
const app = express();
const port = 59361;
app.use(express.json());
app.use(cors());

app.get('/ping', (req, res) => {
    res.json(req.body)
});

app.post('/query', (req, res) => {
    if (req.body) {
        console.log(req.body);
    }
    res.json(req.body)
});

app.post('/url', (req, res) => {
    if (req.body) {
        console.log(req.body);
    }
    res.json(req.body)
});

app.listen(port, (error) => {
    if (!error) {
        console.log(`Example app listening on port ${port}`)
    } else {
        console.log("Error occured launching server: " + port);
    }
});