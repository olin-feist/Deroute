import { sendSearch, isOnline } from "./peer.mjs"

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
export function proccessSearch(buf_with_embedded) {
    //TODO: Send to controller -> SS
    // that call should be return an array of JSON objects
    //i.e. return Controller.SS(buf_with_embedded)

    const test_results = [{ relevance: "test44", url: "test45" }]
    return test_results
}
//Used by peer.mjs
export function recieveURL(url_list) {
    if (!searching){
        return
    }
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
