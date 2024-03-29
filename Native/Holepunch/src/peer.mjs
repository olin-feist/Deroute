import Hyperswarm from 'hyperswarm'
import goodbye from 'graceful-goodbye'
import b4a from 'b4a'
import Queue from './queue.mjs'
import * as peer_controller from './peer-controller.mjs'

export let isOnline = false

const payload_type = {
  search: 0,
  response: 1
}

const swarm = new Hyperswarm()
goodbye(() => {
  swarm.destroy()
})

const prev_searches = new Queue();

// Keep track of all connections and console.log incoming data
const conns = []
swarm.on('connection', conn => {
  const name = b4a.toString(conn.remotePublicKey, 'hex')
  debug(`* got a connection from: ${name} *`)
  conns.push(conn)
  conn.once('close', () => conns.splice(conns.indexOf(conn), 1))
  conn.once('error', (err) => {
    //socket closed by peer
    if(err.code === 'ECONNRESET'){
      conn.destroy()
      return
    }
    console.log(err)
  })
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

async function processPayload(data) {
  const string_data = `${data}`
  const type = parseInt(string_data.slice(0, 1))
  debug(`payload_type: ${type}`)
  switch (type) {
    case payload_type.search:
      debug(`peer: ${string_data.slice(1, 65)}\ndata: ${string_data.slice(65)}\n`)
      const user = string_data.slice(1, 65)
      const embedded_search_val = string_data.slice(65)
      const url_list = await peer_controller.proccessSearch(embedded_search_val)

      if (url_list.length == 0) {
        return
      }
      if (user === b4a.toString(swarm.keyPair.publicKey, 'hex')) {
        processPayload(buildResponsePayload(user, JSON.stringify(url_list)))
      } else {
        sendToPeers(buildResponsePayload(user, JSON.stringify(url_list)))
      }

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

//enable -d for message prints
const debug = (msg) => {
  if (!debug_mode) {
    return
  }
  console.log(msg)
}