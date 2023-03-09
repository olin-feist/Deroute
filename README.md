# Deroute
Deroute is a peer to peer search engine

## Table Of Contents
* [Distributed Networking](#Distributed)
* [Browser Extension](#Browser)
* [Local Search Engine](#search)

<a name="Distributed"/> <a>
## Distributed Networking
### Usage:
```
cd .\Native\Holepunch\
npm install
node controller.mjs [-d]
```
<a name="Browser"/> <a>
## Browser Extension
### Usage:
```
cd .\Extension\
npm install
```
- In Google Chrome, go to Extensions>Manage Extensions.
- Make sure Developer Mode is on.
- Click "Load Unpacked", then navigate to and select the `.\Extension\` folder.
<a name="search"/> <a>
## Local Search Engine
The backend search engine runs on the local host on port 5000: `http://127.0.0.1:5000/`
### Embedding a Website:
Given a valid URL will generate and store a dense vector representation
``` shell
http://127.0.0.1:5000/embedUrl
```
#### Javascript Example
```javascript
 var options = {
     url: 'http://127.0.0.1:5000/embedUrl',
     method: 'POST',
     json: { url:url_to_website }
 };
```
#### Response
```
{'response': 'Error'}
or
{'response': 'Done'}
```

### Embedding a Query:
Given a valid single line of text will generate and return a dense vector representation
``` shell
http://127.0.0.1:5000/embedQuery
```
#### Javascript Example
```javascript
var options = {
    url: 'http://127.0.0.1:5000/embedQuery',
    method: 'POST',
    json: { query:search_query },
    encoding: null // Set encoding to null to receive response as a Buffer
};
```
#### Response
```
buffer
```
### Searching:
Given a vector returned by `embedQuery` will return a list of nearest neighbors related to query
``` shell
http://127.0.0.1:5000/search
```
#### Javascript Example
```javascript
var options = {
    url: 'http://127.0.0.1:5000/search',
    method: 'POST',
    body: dense_vector
};
```
#### Response
```
[{"dist":float,"url":url},
...
{"dist":float,"url":url}]
```
### Test Scripts:
#### build_dataset.py
Builds a dataset of websites taken from urls.txt
#### query.py
Querys dataset built by previous script

