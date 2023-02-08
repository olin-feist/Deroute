from bs4 import BeautifulSoup
import re
import requests
import sys
import os.path
#import num2words
import justext

#verify input url is a url
def verify_url(url):
    regex = re.compile(
        r'^(?:http|ftp)s?://' # http:// or https://
        r'(?:(?:[A-Z0-9](?:[A-Z0-9-]{0,61}[A-Z0-9])?\.)+(?:[A-Z]{2,6}\.?|[A-Z0-9-]{2,}\.?)|' #domain...
        r'localhost|' #localhost...
        r'\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})' # ...or ip
        r'(?::\d+)?' # optional port
        r'(?:/?|[/?]\S+)$', re.IGNORECASE)
    
    test= re.match(regex, url) is not None

    return test

#check for duplicate entries in urls database
def check_dup(path,url):
    #if file does not exist yet
    if(not os.path.isfile(path)):
        return True

    #check for duplicates
    with open(path, "rb") as url_bin:
        url_b=url_bin.read(binary_buffer_size)
        while(url_b):
            
            url_s=url_b.decode("utf-8").strip().replace("\0", "")
            
            if(url_s==url):
                return False

            url_b=url_bin.read(binary_buffer_size)

    return True


urls_file_path="data/urls.bin"
binary_buffer_size=300


#cmd line args
url = sys.argv[1]
url = url.strip()

#check valid url
if(not verify_url(url)):
    print("Error: "+url+" is not a valid URL")
    exit(1)

html = requests.get(url) #get website

#check response
if(not html.ok):
    print("Error: Response Code",html.status_code)
    exit(1)

#if debug mode is on
if(len(sys.argv)==2):
    if(check_dup(urls_file_path,url)):
        with open(urls_file_path, "ab") as url_bin:
            url= url + "\0" * (binary_buffer_size - len(url))
            url_bin.write(bytes(url, encoding="utf-8"))
    else:
        print("Error: Duplicate Entry")
        exit(1)


#html parsing
soup = BeautifulSoup(html.content, 'html.parser')

# Find the element in the HTML
header = soup.find('header')
head = soup.find('head')
# Remove the element from the HTML
if header:
    header.decompose()
if head:
    head.decompose()
    
updated_html = soup.encode("utf-8") #html after removing elements

#filter for content
paragraphs = justext.justext(updated_html, justext.get_stoplist("English"),70,140,0.2,0.3,0.43, 150, True, None, 'utf8', 'replace')

filtered_content=""
for paragraph in paragraphs:
    if(not paragraph.is_boilerplate):
        filtered_content+=paragraph.text
        filtered_content+='\n'

#pre-proccess data for fastText (off for now)

#filtered_content = re.sub(r"(\d+)", lambda x: num2words.num2words(int(x.group(0))), filtered_content) #convert numbers to words

#save content
with open('data/website_plain.txt', 'w',encoding="utf-8") as f:
    f.write(filtered_content)



