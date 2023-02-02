import Hyperswarm from 'hyperswarm'
import goodbye from 'graceful-goodbye'
import crypto from 'hypercore-crypto'
import b4a from 'b4a'
import Hyperbeam from 'hyperbeam'

import Queue from './queue.mjs'

const swarm = new Hyperswarm()
goodbye(() => {
  swarm.destroy()
})

const prev_searches = new Queue();

// Keep track of all connections and console.log incoming data
const conns = []
swarm.on('connection', conn => {
  const name = b4a.toString(conn.remotePublicKey, 'hex')
  console.log('* got a connection from:', name, '*')
  conns.push(conn)
  conn.once('close', () => conns.splice(conns.indexOf(conn), 1))
  conn.on('data', data => {

    if(prev_searches.contains(`${data}`)) return //check to see if we have gotten data b4

    console.log(`${name}: `, `\n original sender:`, `${data}`.slice(0,64), `\n data:`, `${data}`.slice(64)) //incoming data from hyperswarm
    
    prev_searches.push(`${data}`) //store to not propogate repeatively 
    for (const conn of conns) { //propogate to all conn
      conn.write(data)
    }
  })
})

// Broadcast stdin to all connections
process.stdin.on('data', d => {
  if (`${d}`.startsWith('beam')) {  //interupt send
    console.log("pong")
    return
  }
  //enter message into buffer before sending
  d = b4a.toString(swarm.keyPair.publicKey, 'hex') + `${d}`
  d = b4a.from(d)
  //send
  for (const conn of conns) {
    conn.write(d)
  }
})

// Join a common topic
const topic = process.argv[2] ? b4a.from(process.argv[2], 'hex') : crypto.randomBytes(32)
const discovery = swarm.join(topic, { client: true, server: true })

// The flushed promise will resolve when the topic has been fully announced to the DHT
discovery.flushed().then(() => {
  console.log('joined topic:', b4a.toString(topic, 'hex'))
})

//below is WIP

const beam = new Hyperbeam((`DerouteConnectBack` + b4a.toString(swarm.keyPair.publicKey, 'hex')))
  // beam.key gives the passphrase
console.log('-->passphrase: ', beam.key)


const buildPayload = (distance_score, data) =>{
  let d = swarm.keyPair.publicKey + distance_score + data
  return b4a.from(d)
}

const debuildPayload = (data) => {
  console.log(`${data}`.slice(0,64), `\n data:`, `${data}`.slice(64)) //incoming data from hyperswarm
}