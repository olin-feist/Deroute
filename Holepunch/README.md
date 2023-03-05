# Deroute

### Currently done
- Fixed error that causes clients to all timeout when someone leaves the swarm
- Peer churn preforms after multiple passes of messages between peers
- propogate message ONCE to all peers
- two payload data flows 
* Search:
``` shell
- Recieve {vector, distance score}
- Translate to buffer with {type, ID + vector + distance score}
- Send buffer
```
* Receive buffer:
``` shell
- translate to {ID + vector + distance score}
- pass (vector, distance score) to simularity search
 - recieve URL[]
- if not empty, then translate to buffer {type, + ORIGNAL_ID + url_list} and send
```
* Catch response: 
``` shell 
    - check to see if ORIGINAL_ID matches your ID, read in url_list if it does
```
- setup a controller for the exterior/main controller to import
### Usage
Install the required dependencies
``` shell
npm install
```
run
``` shell
node peer.mjs e3cd816e9dd448a50ec7f89865e5399fadb7d41657baf9dac2c29c9cf734c790
```
### Debug
To enable lots of prints for debugging, set
``` shell
const debug_mode
```
to `true` at the bottom of peer.mjs

notes *for phil :)
//https://nodejs.org/api/buffer.html#buftostringencoding-start-end (toString())
// https://www.php.net/manual/en/function.sodium-crypto-generichash.php