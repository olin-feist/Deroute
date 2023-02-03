cd "../"
rm data/query.bin
echo -e "\033[31m- Deleted query.bin\033[0m"

OUTPUT=$(cat data/query_plain.txt)

bin/embed.exe bin/model.en.bin data/query.bin < data/query_plain.txt
echo -e "\033[32m- Embedded:\033[0m" query_plain.txt
echo
echo -e "\033[32mSearch Query:\033[0m" "$OUTPUT"
bin/search.exe data/vectors.bin 5 < data/query.bin