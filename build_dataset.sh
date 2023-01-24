#get website and parse it
curl -s -L -o data/webhtml.html https://en.wikipedia.org/wiki/Curse_of_dimensionality
python src/parse_website.py data/webhtml.html

#embedding
bin/embed.exe bin/model.en.bin data/website_plain.txt data/vectors.bin new
echo https://en.wikipedia.org/wiki/Curse_of_dimensionality

#repeat for all urls in urls.txt
while read -r line; do
    curl -s -L -o data/webhtml.html "$line"
    python src/parse_website.py data/webhtml.html
    bin/embed.exe bin/model.en.bin data/website_plain.txt data/vectors.bin append
    echo "$line"
done < "data/urls.txt"