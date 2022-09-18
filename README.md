# Arithmetic game

A game class with multiple command line games.

### Compilation

Run make

Dependencies: Rapidxml

### Info

./main.out {command} {folder}

**argv[1]->command**

The command can be "default", "run", "state", "init".

"default" initializes and sets up a default config in state.xml in the folder.

"run" loads state.xml from the folder and starts the game cycle.

"state" loads state.xml from the folder and starts a cli to make changes to state.xml 

"init" creates state.xml

**argv[2]->folder**

A folder with can be used for storing state.xml and game data.

**state.xml**

state.xml consists of 4 layers -> packages, package, game, mod

with each of those node layers has an attribute "name".

Package names can be whatever.

Game names must be one of those declared in _games static variable namely "doomsday", "arithmetic".

mod names can be whatever.

Those names are also used for navigating the xml tree when "state" cli is used.

**Game cycle**

The mod layer has attributes which serve as input to the game's functions.

A game cycle consists of running each mod of each game of each package, collecting

user input and storing data.

Each game mod generates a query and provides a string solution which is compared with the input.
