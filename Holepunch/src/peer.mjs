import Hyperswarm from 'hyperswarm'
import goodbye from 'graceful-goodbye'
import crypto from 'hypercore-crypto'
import b4a from 'b4a'

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
})

//Send search query
export function sendSearch(distance_score, vector) {
  let query = buildSearchPayload(distance_score, `${vector}`)
  sendToPeers(query)
}

//Write to swarm
const sendToPeers = (data) => {
  prev_searches.push(`${data}`)

  for (const conn of conns) {
    conn.write(data)
  }
}

const buildSearchPayload = (distance_score, vector) => {
  let d = `${payload_type.search}${b4a.toString(swarm.keyPair.publicKey, 'hex')}${distance_score.toPrecision(16)}${vector}`
  debug(`My Search Payload: ${d}\n`)
  return b4a.from(d)
}
const buildResponsePayload = (user_key, url_list) => {
  let d = '1' + user_key + url_list
  debug(`My Response Payload: ${d}\n`)
  return b4a.from(d)
}

//proccess payload and call respectve fucntions
const processPayload = (data) => {
  let type = parseInt(`${data}`.slice(0, 1))
  debug(`payload_type: ${type}\n`)
  switch (type) {
    case payload_type.search:
      debug(`peer: ${`${data}`.slice(1, 65)}\ndistance score: ${`${data}`.slice(65, 83)}\n vector: ${`${data}`.slice(83)}\n`)
      let user = `${data}`.slice(1, 65)
      let distance_score = `${data}`.slice(65, 83)
      let vector = `${data}`.slice(83)

      let url_list = peer_controller.proccessSearch(distance_score, vector)
      if (url_list.length == 0) {
        return
      }

      sendToPeers(buildResponsePayload(user, url_list))
      break;
    case payload_type.response:
      debug(`peer: ${`${data}`.slice(1, 65)}\nURL_LIST: ${`${data}`.slice(65, 83)}\n`)

      if (b4a.toString(swarm.keyPair.publicKey, 'hex') === `${data}`.slice(1, 65)) {
        peer_controller.recieveURL(`${data}`.slice(65))
      }
      break;
    default:
  }
}

//process args
let debug_mode = false;
process.argv.forEach(element => {
  if (element == '-d'){
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