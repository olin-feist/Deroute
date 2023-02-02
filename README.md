# Deroute

- Currently done
Fixed error that causes clients to all timeout when someone leaves the swarm
Peer churn preforms after multiple passes of messages between peers
propogate message ONCE to all peers

-TODO
- two payload data flows 
  Search:
    Recieve {vector, distance score}
    Translate to buffer with {ID + vector + distance score}
    open hyperbeam to accept incoming data
    Send buffer
    
  Receive buffer:
    translate to {ID + vector + distance score}
    pass (vector, distance score) to simularity search
    recieve URL[]
    if not empty, then open and push URL[] through {DerouteConnectBack + ID} hyperbeam

- setup event for when URL[] gets incoming data

notes *for phil :)
//https://nodejs.org/api/buffer.html#buftostringencoding-start-end (toString())
// https://www.php.net/manual/en/function.sodium-crypto-generichash.php