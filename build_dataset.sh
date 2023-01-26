#get website and parse it
python src/parse_website.py https://en.wikipedia.org/wiki/Curse_of_dimensionality

#embedding
bin/embed.exe bin/model.en.bin data/website_plain.txt data/vectors.bin new
echo https://en.wikipedia.org/wiki/Curse_of_dimensionality

#repeat for all urls in urls.txt
while read -r line; do
    python src/parse_website.py "$line"
    bin/embed.exe bin/model.en.bin data/website_plain.txt data/vectors.bin append
    echo "$line"
done < "data/urls.txt"