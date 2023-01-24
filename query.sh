bin/embed.exe bin/model.en.bin data/query_plain.txt data/query.bin new

cat data/query_plain.txt
echo
bin/search.exe data/vectors.bin data/query.bin 5