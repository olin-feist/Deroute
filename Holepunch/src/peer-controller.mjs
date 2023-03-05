import { sendSearch } from "./peer.mjs"

//BE -> SS -> SWARM
export function search(distance_score, vector){
    sendSearch(distance_score, vector)
}

//SWARM -> SS -> SWARM
export function proccessSearch(distance_score, vector){
    //TODO: Send to controller -> SS
    return ["www.w", "b.com"]
}

//SWARM -> BE
export function recieveURL(url_list){
    //TODO: Send to controller -> BE
    console.log(url_list)
}

// Broadcast stdin to all connections
process.stdin.on('data', data => {
    search(0.9, data)
  })