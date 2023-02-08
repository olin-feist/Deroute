cd "../"
rm data/query.bin
echo -e "\033[31m- Deleted query.bin\033[0m"

OUTPUT=$(cat tests/query_plain.txt)
model=bin/model.en.bin

bin/embed.exe $model data/query.bin < tests/query_plain.txt
echo -e "\033[32m- Embedded:\033[0m" query_plain.txt
echo
echo -e "\033[32mSearch Query:\033[0m" "$OUTPUT"
bin/search.exe data/vectors.bin < data/query.bin
