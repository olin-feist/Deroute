import { sendSearch, isOnline } from "./peer.mjs"
import fetch from 'node-fetch';

const results = { results: [] }
let searching = false

//BE -> SS -> SWARM
export async function search(buf_with_embedded) {
    const embedded_search_val = `${buf_with_embedded}`
    results.results = []
    searching = true
    
    sendSearch(embedded_search_val)
    await new Promise(resolve => setTimeout(resolve, 1000));
    
    searching = false
    return results
}

//SWARM -> SS -> SWARM
export async function proccessSearch(buf_with_embedded) {
    //TODO: Send to controller -> SS
    // that call should be return an array of JSON objects
    //i.e. return Controller.SS(buf_with_embedded)
    const response = await fetch(`http://127.0.0.1:5000/search`, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: buf_with_embedded
    })

    if (!response.ok) {
		throw new Error(`HTTP error! status: ${response.status}`);
	}
	const data = await response.json();
    return data
}
//Used by peer.mjs
export function recieveURL(url_list) {
    if (!searching){
        return
    }
    console.log(url_list)
    JSON.parse(url_list).forEach(element => {
        results.results.push(element)
    })
}

export function pingSwarm() {
    return isOnline
}

// Broadcast stdin to all connections
process.stdin.on('data', data => {
    search(Buffer.from(data)).then((results)=> console.log(results))
})
