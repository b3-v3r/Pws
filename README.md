# Pws

Pws is open source application for watching your activity on all applicatoins and your projects. Based on X11 lib.
## Features
 - [x]  Application activity tracking
 - [x] Track changes in your projects
 - [x] Time using appilications
 ## Requirements
	 
 - X11lib 
 - nlohmanjson ( [https://github.com/nlohmann/json](https://github.com/nlohmann/json) )
 

## Install
    > git clone https://github.com/b3-v3r/Pws.git
	> cd Pws
	> mkdir build
	> mkdir stats
	> cd build
	> cmake ..
	> make
## Using 
- Example make configuration file ( *recomended filename config.json*  )

	    {
		    "projects": ["path/to/project1", "path/to/project2"]
	    }
	    
- Start tool 

		./PwsCore config.json

- Watch your statistics on local server

		http://127.0.0.1:2020/main.html
		
---
![](https://github.com/b3-v3r/Pws/blob/master/doc/example_screenshot_1.png?raw=true)

	
	
