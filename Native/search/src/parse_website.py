from bs4 import BeautifulSoup
import re
import requests
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


def parse_website(url,debug):
    url = url.strip()

    #check valid url
    if(not verify_url(url)):
        print("Error: "+url+" is not a valid URL")
        return -1

    html = requests.get(url) #get website

    #check response
    if(not html.ok):
        print("Error: Response Code",html.status_code)
        return -1


    #html parsing
    soup = BeautifulSoup(html.content, 'html.parser')

    # Find the element in the HTML
    #header = soup.find('header')
    #head = soup.find('head')
    maths = soup.find_all('math')
    annotations = soup.find_all('annotation')
    mstyles = soup.find_all('mstyle')
    
    # Remove the element from the HTML
    #if header:
    #    header.decompose()

    #if head:
    #    head.decompose()

    for math in maths:
        math.decompose()

    for annotation in annotations:
        annotation.decompose()

    for mstyle in mstyles:
        mstyle.decompose()
        
    updated_html = soup.encode("utf-8") #html after removing elements

    #get stopwords
    with open("stoplists/English.txt", "r") as f:
        stopwords = f.read()
    stoplist=frozenset(w.lower() for w in stopwords.splitlines())

    #filter for content
    paragraphs = justext.justext(updated_html, stoplist,70,140,0.2,0.3,0.43, 150, True, None, 'utf8', 'replace')

    filtered_content=""
    for paragraph in paragraphs:
        if(not paragraph.is_boilerplate):
            filtered_content+=paragraph.text
            filtered_content+='\n'
            
    filtered_content = filtered_content.replace("\n", "")
    
    return filtered_content



