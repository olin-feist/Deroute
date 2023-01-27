document.addEventListener('DOMContentLoaded', function() {
    chrome.history.search({text: ''}, function(histories) {
        const template = document.getElementById("li_template");
        const elements = new Set();
    
        for (const history of histories) {
            const element = template.content.firstElementChild.cloneNode(true);
    
            const title = history.title;
            const url = history.url;
    
            element.querySelector(".title").textContent = title;
            element.querySelector(".url").textContent = url;
    
            elements.add(element);
        }
    
        document.querySelector("ul").append(...elements);
    })
})