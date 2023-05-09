# Dèroute
Dèroute is an open source, fully decentralized, P2P search engine that is easily accessible through a browser extension. Upon execution, users will be able to search over the entire active P2P network and discover the web in a way entirely different then traditional search engines.

Explore the [wiki](https://github.com/olin-feist/Deroute/wiki) for more in depth information about the implementation of Dèroute.

## Table Of Contents
* [Distributed Networking](#Distributed)
* [Local Search Engine](#search)
* [Browser Extension](#Browser)

<a name="Distributed"/> <a>
## Distributed Networking
The distributed networking provides communication functionalities between the [Browser Extension](#Browser) and [Local Search Engine](#search). When a search request is received from the [Browser Extension](#Browser), it sent to the [Local Search Engine](#search) and the connected swarm of peers. Each peer that receives the search request will propagate it to their peers and process it, returning any relevant website urls back to the original searcher.
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


