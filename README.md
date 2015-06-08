Command Chat
============

Installing
----------
This application requires development package of libncurses.

For compiling run *make compile*

Running
-------
For running server run *make run* (it will listen on port 1234) or *./commandchat --server port*

For running client run *make run-client* (it will connect to port 1234) or *./commandchat 1234*

Client Commands
---------------
*/N name* - Changes name to name
*/J channel* - Joins to channel
*/L* - Leaves current channels
*/C channel* - Selects current channel

Server Commands
---------------
*users* - Prints list of users
*channels* - Prints list of channels
*quit* - Quits