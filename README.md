# MediaDownloader
## 1. reason

In previous crawler projects, the logic of the crawler's parsing data needs to be constantly adjusted due to the constant changes of the other party's website structure or web page structure; If you use C++to complete basic functions and LUA scripts to complete business logic, you can make flexible changes;



Most crawler projects are used for enterprise background work and crawling network data. However, for individual users to choose music or video applications, they are not suitable for the non-interface python technology route and the deployment of web architecture programs such as nodejs; In other words, sometimes crawlers are just tools for programmers to amuse themselves;



## 2. advantage



1) The project uses VC++/MFC to write the interface and basic network access function, and uses LUA to write the data analysis logic to facilitate the adjustment of the analysis logic with the website;



2) The compiled project is a binary program with fast execution speed; Convenient deployment; Users can directly exchange scripts and apply them directly;