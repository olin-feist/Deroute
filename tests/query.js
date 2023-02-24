const { spawn } = require('child_process');

//embed commands
const embed_cmd = '../bin/embed.exe';
const embed_args = ['../bin/model.q.ftz'];

//search command
const search_cmd = '../bin/search.exe';
const search_args = ['../data/vectors.bin','../data/urls.bin'];

const embed_process = spawn(embed_cmd, embed_args);


// call embed.exe to embed query
function embed_query(input) {
  return new Promise((resolve, reject) => {

    embed_process.stdin.write(input);

    embed_process.stdout.once('data', (data) => {
      resolve(data);
    });

    
    embed_process.stdin.on('error', (err) => {
      reject(err);
    });

    embed_process.stdout.on('error', (err) => {
      reject(err);
    });

  });
}

// call search.exe to search query
function search(buffer) {
  search_process = spawn(search_cmd, search_args);

  return new Promise((resolve, reject) => {
    let output = "";

    search_process.stdin.write(buffer);

    search_process.stdout.on('data', (data) => {
      output += data.toString('utf8');
    });

    search_process.stdout.on('end', () => {
      resolve(output);
    });

    search_process.stdin.on('error', (err) => {
      reject(err);
    });

    search_process.stdout.on('error', (err) => {
      reject(err);
    });
  });
}

// Example usage
async function run() {

  

  //embed query1
  const output1 = await embed_query('similarity search vector\n');

  //search query 1
  const query1 = await search(output1);
  console.log(query1);
  search_process.stdin.end(); //end search


  //embed query2
  const output2 = await embed_query('ravenna bridge\n');

  //search query 2
  const query2 = await search(output2);
  console.log(query2);
  search_process.stdin.end(); //end search

  
  embed_process.stdin.end(); //end embeddings
}

run();