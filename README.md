# Dèroute
Dèroute is an open source, fully decentralized, P2P search engine that is easily accessible through a browser extension. Upon execution, users will be able to search over the entire active P2P network and discover the web in a way entirely different then traditional search engines.

## Table Of Contents
* [Distributed Networking](#Distributed)
* [Browser Extension](#Browser)
* [Local Search Engine](#search)

<a name="Distributed"/> <a>
## Distributed Networking
The distributed networking provides communication functionalities between the [Browser Extension](#Browser) and [Local Search Engine](#search). When a search request is recieved from the [Browser Extension](#Browser), it sent to the [Local Search Engine](#search) and the connected swarm of peers. Each peer that recieves the search request will propogate it to their peers and process it, returning any relevant website urls back to the original searcher.
### Swarm:
The distributed networking is built ontop of [Hyperswarm](https://github.com/holepunchto/hyperswarm), a distributed networking stack for connecting peers. The current implementation uses the 'serverless' approach to swarm peers around a common topic. The common topic for Deroute is hardcoded to `e3cd816e9dd448a50ec7f89865e5399fadb7d41657baf9dac2c29c9cf734c790`. 
### Search Request:
Search requests are propogated to all connected peers. Upon recieving a search request for the first time, peers will process it on their [Local Search Engine](#search) to see if they have any relevant website urls to return back. Additionally, peers will propogate it on to their known peers. If they have recieved the search request recently (last 64 searches, configurable in [queue.mjs](https://github.com/olin-feist/Deroute/blob/main/Native/Holepunch/src/queue.mjs)), they will ignore it.

The search payload is built as follows:
```
 0: payload type
 1-65: original peer's public key
 65+: embedded search value in json format
```
### Search Response:
Response payloads are built when the [Local Search Engine](#search) returns relevant website urls to send back. They are propogated back across the swarm and retrieved by the original searcher. Once recieved, they are appended to an array that contains all results and is returned to the [Browser Extension](#Browser) after 1 second via a timeout promise.

The response payload is built as follows:
```
 0: payload type
 1-65: original peer's public key
 65+: url list in json format
```
### Usage:
```
cd .\Native\Holepunch\
npm install
node controller.mjs [-d]
```

<a name="search"/> <a>
## Local Search Engine
The local search engine is responsible for the searching and storage of websites on a single user's machine. Using natural language processing and similarity search it will find results accurately and efficiently. Built using [FastText](https://github.com/facebookresearch/fastText/) and [FAISS](https://github.com/facebookresearch/faiss) the search engine is designed to be small and fast while still being able to yield relevant results. The search engine is primarily written in C++ with a high level controller written in Python. The external interface for the search engine works via a local http WSGI server.\
\
The model used in the distribution to create document and query embeddings is a reduced form of the model [cc.en.300](https://fasttext.cc/docs/en/crawl-vectors.html) from E. Grave, P. Bojanowski, P. Gupta, A. Joulin, T. Mikolov, Learning Word Vectors for 157 Languages. The model was trained on [Common Crawl](https://commoncrawl.org/) and [Wikipedia](https://www.wikipedia.org/) using the CBOW model.

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


