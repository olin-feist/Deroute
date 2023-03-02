filename=${PWD}/$1


cd "../"
rm data/vectors.bin
echo -e "\033[31m- Deleted vectors.bin\033[0m"
rm data/urls.bin
echo -e "\033[31m- Deleted urls.bin\033[0m"


model=bin/model.q.ftz
urls_b=data/urls.bin

# Count the number of lines in the file
lines=$(wc -l < $filename)

touch parsed.txt
echo
echo -e "\033[1m\033[34m- Parsing: \033[0m"
i=0
while read -r line; do
    
    python src/parse_website.py -output $urls_b -url "$line"

    cat data/website_plain.txt >> parsed.txt
    echo >> parsed.txt

    i=$((i+1))

    # Calculate the percentage completed
    percent=$((100 * $i / $lines))

    # Output the progress bar
    echo -ne "\r["
    for j in $(seq 1 10); do
      if [ $j -le $((percent / 10)) ]; then
        echo -ne "\033[32m#\033[0m"
      else
        echo -ne "\033[31m-\033[0m"
      fi
    done
    echo -ne "] $percent%"


    echo -e "\033[33m \tParsed URL ($i/$lines):\033[0m" "$line"
done < $filename

echo
echo -e "\033[1m\033[34m- Embedding: \033[0m"
i=0
cat parsed.txt | bin/embed.exe  $model data/vectors.bin | while read -t 1 line; do  
  i=$((i+1))

  # Calculate the percentage completed
  percent=$((100 * $i / $lines))

  # Output the progress bar
  echo -ne "\r["
  for j in $(seq 1 10); do
    if [ $j -le $((percent / 10)) ]; then
      echo -ne "\033[32m#\033[0m"
    else
      echo -ne "\033[31m-\033[0m"
    fi
  done
  echo -ne "] $percent%"

  url=$(sed -n "${i}p" "$filename")
  echo -e "\033[33m \tEmbedded URL ($i/$lines):\033[0m" "$url"
done

rm parsed.txt
echo -ne "\n"
echo -e "\033[32m- Built Dataset of $lines URLs \033[0m" 


