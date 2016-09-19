# JLexx
JLexx is a lexical analyzer written in c++ It is very similiary to lexx, but I wrote it in c++ and is intended to be highly readable
so that if you really want to get into the internals and customize it to your specific needs you can do so. The source code for lexx
is fairly large and in my opinion the code is outdated. The code I wrote is fairly small around 1500 lines for the lexer plus the lines for the regexp engine


In order to use just pull the repo cd into InstallDir and run ./Install.sh
Then you gen your files with jlGen ${FILENAME}

At the moment there are examples in the TestFiles directory and particularly JLexxExample.txt is a good starting point
It will explain the basics but to run just cd into the dir

jlGen JLexxExample.txt
./LexMain < stdin



where ${FILENAME} = whatever file
