import { derouteSearch as deroute_search } from "./peer.mjs"

//BE -> SS -> SWARM
export function search(distance_score, vector){
    deroute_search.derouteSearch(distance_score, vector)
}

//SWARM -> SS -> SWARM
export function proccessSearch(distance_score, vector){
    //TODO: Send to controller -> SS
    return []
}

//SWARM -> BE
export function recieveURL(url_list){
    //TODO: Send to controller -> BE
    console.log(url_list)
}