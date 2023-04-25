import { sendSearch, isOnline } from "./peer.mjs"
import fetch from 'node-fetch';

const results = { results: [] }
let searching = false

export function pingSwarm() {
    return isOnline
}

/* 
 * Send a search request to the swarm with a json recieved from the search backend.
 * returns a promise to the browser extension with results of search.
 */ 
export async function search(buf_with_embedded) {
    const embedded_search_val = JSON.stringify(buf_with_embedded)
    results.results = []
    searching = true

    sendSearch(embedded_search_val)
    await new Promise(resolve => setTimeout(resolve, 1000));

    searching = false
    return results
}

/* 
 * Process a search request from a peer.
 * Sends a fetch with the search embedded buf and expects a url_list in a json back.
 */
export async function proccessSearch(vector) {
    const response = await fetch(`http://127.0.0.1:5000/search`, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: vector
    })

    if (!response.ok) {
        throw new Error(`HTTP error! status: ${response.status}`);
    }
    const data = await response.json();
    return data
}

//Used by peer.mjs, appends url to results
export function recieveURL(url_list) {
    if (!searching) {
        return
    }
    JSON.parse(url_list).forEach(element => {
        results.results.push(element)
    })
}