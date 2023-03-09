# Dèroute
Dèroute is an open source, fully decentralized, P2P search engine that is easily accessible through a browser extension. Upon execution, users will be able to search over the entire active P2P network and discover the web in a way entirely different then traditional search engines.

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
The local search engine manages the searching and storage of websites on a single user's machine. The local search engine has three main functionalities: store a website, generate a representation for a query, and search a local database. The backend search engine runs on the local host on port 5000 and listens for requests via http.
### Embedding a Website:
Given a valid URL will generate and store a dense vector representation
#### POST request URL
``` shell
http://127.0.0.1:5000/embedUrl
```
#### Example Config
```javascript
 var options = {
     url: 'http://127.0.0.1:5000/embedUrl',
     method: 'POST',
     body: { url:url_to_website }
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
#### POST request URL
``` shell
http://127.0.0.1:5000/embedQuery
```
#### Example Config
```javascript
var options = {
    url: 'http://127.0.0.1:5000/embedQuery',
    method: 'POST',
    body: { query:search_query }
};
```
#### Response
```
{vector: dense_vector}
```
### Searching:
Given a vector returned by `embedQuery` will return a list of nearest neighbors related to query
#### POST request URL
``` shell
http://127.0.0.1:5000/search
```
#### Example Config
```javascript
var options = {
    url: 'http://127.0.0.1:5000/search',
    method: 'POST',
    body: {vector: vector}
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

