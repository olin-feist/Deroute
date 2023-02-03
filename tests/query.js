var exec = require('child_process').exec;

var embed_cmd = '.\\bin\\embed.exe bin/model.en.bin'; //generate dense vector command
var buffer;

//embedding command config
var embed_p=exec(embed_cmd, {encoding: 'binary', maxBuffer: 1208}, function(error,stdout) {
  if (error) {
    console.error(error);
    return;
  }

  buffer = Buffer.from(stdout, 'binary');
  exec_search(buffer);
});

//search query
embed_p.stdin.write("similarity search vector");
embed_p.stdin.end();


//call search.exe
function exec_search(buffer) {
  var search_cmd = '.\\bin\\search.exe data/vectors.bin 5';//command to search for 5 nearest neighbors

  //search command config
  var search_p=exec(search_cmd, {encoding: 'binary', maxBuffer: 1000}, function(error, stdout) {

    if (error) {
      console.error(error);
      return;
    }

    if (stdout) {
      console.log(stdout);
    }

  });

  //feed buffer
  search_p.stdin.write(buffer);
  search_p.stdin.end();
}