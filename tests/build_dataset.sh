filename=${PWD}/$1


cd "../"
rm data/vectors.bin
echo -e "\033[31m- Deleted vectors.bin\033[0m"
rm data/urls.bin
echo -e "\033[31m- Deleted urls.bin\033[0m"


model=bin/model.en.bin
urls_b=data/urls.bin

# Count the number of lines in the file
lines=$(wc -l < $filename)

# Loop through the lines of the file
i=0
while read -r line; do

  # Embed urls
  python src/parse_website.py $urls_b "$line"
  bin/embed.exe  $model data/vectors.bin < data/website_plain.txt

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


  echo -e "\033[33m \tAdded URL ($i/$lines):\033[0m" "$line"

done < $filename
echo -ne "\n"
echo -e "\033[32m- Built Dataset of $lines URLs \033[0m" 