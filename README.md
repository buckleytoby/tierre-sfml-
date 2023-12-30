# Coding Patterns
## Program flow
* Use cascading hierarchy of classes, each parent class contains an instance of each of its children classes.
* Generally, parent class' responsibility is to either
    * handle the program call itself
    * figure out which children needs to handle the program call
    * convert all required data into a datatype compatible with the child method before calling it and passing that data
    * Children should return an enum of functions that it can't execute itself, but requires the parent to do so.

        main
        |
-----------------
v               V
titlescreen    gameplay
                |
           --------------
           v      v      v
         GUI   viewport  map
.
.
.
etc.


## User Inputs
* follow the program flow.
* define enum class {classname}Inputs for each child class
* For immediate changes: HandleInput enums / methods should execute immediate changes themselves.
* For real-time changes: HandleInput methods should set bitflags in its class's {class}Actions enum. The class's update function then executes the appropriate functions based on the {class}Actions enum.