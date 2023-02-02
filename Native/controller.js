var nativeMessage = require('chrome-native-messaging');

var input = new nativeMessage.Input();
var transform = new nativeMessage.Transform(messageHandler);
var output = new nativeMessage.Output();

process.stdin
    .pipe(input)
    .pipe(transform)
    .pipe(output)
    .pipe(process.stdout)
;

function messageHandler(msg, push, done) {
    if (msg.url) {
        //give to fastText
        done();
    }else if (msg.query) {
        //give to swarm
        done();
    } else {
        // Just echo the message:
        push(msg);
        done();
    }
}
