import Hyperswarm from 'hyperswarm'
import goodbye from 'graceful-goodbye'
import crypto from 'hypercore-crypto'
import b4a from 'b4a'
import { Buffer } from 'node:buffer';

import Queue from './queue.mjs'
import * as peer_controller from './peer-controller.mjs'

const swarm = new Hyperswarm()
goodbye(() => {
  swarm.destroy()
})

const prev_searches = new Queue();

const payload_type = {
  search: 0,
  response: 1
}

export let isOnline = false

// Keep track of all connections and console.log incoming data
const conns = []
swarm.on('connection', conn => {
  const name = b4a.toString(conn.remotePublicKey, 'hex')
  debug(`* got a connection from: ${name} *`)
  conns.push(conn)
  conn.once('close', () => conns.splice(conns.indexOf(conn), 1))
  conn.on('data', data => {

    //check to see if we have gotten data b4
    if (prev_searches.contains(`${data}`)) return

    processPayload(data)

    //propogate message to rest of swarm
    sendToPeers(data)
  })
})

// Join a common topic
const topic = b4a.from('e3cd816e9dd448a50ec7f89865e5399fadb7d41657baf9dac2c29c9cf734c790', 'hex')
const discovery = swarm.join(topic, { client: true, server: true })

// The flushed promise will resolve when the topic has been fully announced to the DHT
discovery.flushed().then(() => {
  console.log('joined topic:', b4a.toString(topic, 'hex'))
  isOnline = true
})

//Send search query
export function sendSearch(embedded_search_val) {
  let query = buildSearchPayload(embedded_search_val)
  processPayload(query)
  sendToPeers(query)
}

//Write to swarm
const sendToPeers = (data) => {
  prev_searches.push(`${data}`)

  for (const conn of conns) {
    conn.write(data)
  }
}
/* 
 * Type: String
 * with index 0 being the payload type, index 1 to 65 being the users key,
 * and index 65+ being the embedded search value
 */
const buildSearchPayload = (embedded_search_val) => {
  let d = `${payload_type.search}${b4a.toString(swarm.keyPair.publicKey, 'hex')}${embedded_search_val}`
  debug(`My Search Payload: ${d}\n`)
  return b4a.from(d)
}
/* 
 * Type: String
 * with index 0 being the payload type, index 1 to 65 being the users key,
 * and index 65+ being the url_list
 */
const buildResponsePayload = (user_key, url_list) => {
  let d = `${payload_type.response}${user_key}${url_list}`
  debug(`My Response Payload: ${d}\n`)
  return b4a.from(d)
}

//proccess payload and call respectve fucntions
const processPayload = (data) => {
  const string_data = `${data}`
  let type = parseInt(string_data.slice(0, 1))
  debug(`payload_type: ${type}`)
  switch (type) {
    case payload_type.search:
      debug(`peer: ${string_data.slice(1, 65)}\ndata: ${string_data.slice(65)}\n`)
      let user = string_data.slice(1, 65)
      let embedded_search_val = string_data.slice(65)

      const url_list = peer_controller.proccessSearch(Buffer.from(embedded_search_val))
      url_list.then((data) => {
        if (data.length == 0) {
          return
        }

        sendToPeers(buildResponsePayload(user, JSON.stringify(data)))
      })
      break;
    case payload_type.response:
      debug(`peer: ${string_data.slice(1, 65)}\nURL_LIST: ${string_data.slice(65)}\n`)

      if (b4a.toString(swarm.keyPair.publicKey, 'hex') === string_data.slice(1, 65)) {
        peer_controller.recieveURL(string_data.slice(65))
      }
      break;
    default:
  }
}


//process args
let debug_mode = false;
process.argv.forEach(element => {
  if (element == '-d') {
    debug_mode = true;
  }
});


//enable debug mode for message prints ;)
const debug = (msg) => {
  if (!debug_mode) {
    return
  }
  console.log(msg)
}