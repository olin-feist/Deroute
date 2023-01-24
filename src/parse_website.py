from bs4 import BeautifulSoup
import re
import sys
import num2words
import justext

#cmd line args
arg = sys.argv[1]
 
page = open(arg,encoding="utf8")
soup = BeautifulSoup(page.read(), 'html.parser')

# Find the footer element in the HTML
footer = soup.find('footer')

# Remove the footer element from the HTML
if footer:
    footer.decompose()

updated_html = soup.encode("utf-8") #html after removing elements

paragraphs = justext.justext(updated_html, justext.get_stoplist("English"),70,140,0.2,0.3,0.43, 150, True, None, 'utf8', 'replace') #filter content

filtered_content=""
for paragraph in paragraphs:
    if(not paragraph.is_boilerplate):
        filtered_content+=paragraph.text
        filtered_content+='\n'

#pre-proccess data for fastText
filtered_content = filtered_content.replace("=", "equals")
filtered_content = filtered_content.replace("+", "plus" )
filtered_content=filtered_content.lower() #lowercase
filtered_content = re.sub(r"(\d+)", lambda x: num2words.num2words(int(x.group(0))), filtered_content) #convert numbers to words
filtered_content=re.sub(r'[^\w]', ' ', filtered_content) #remove non ascii character
regex = re.compile('[^a-zA-Z]') #remove non letters
filtered_content=regex.sub(' ', filtered_content) #remove non letter character
filtered_content=re.sub(r'\b[b-hj-z]\b', '', filtered_content) #remove single letter words that arent (a and i)
filtered_content=re.sub(r' +', ' ',filtered_content) #remove extra spaces
filtered_content=filtered_content.strip()
#save cotent
with open('data/website_plain.txt', 'w',encoding="utf-8") as f:
    f.write(filtered_content)



