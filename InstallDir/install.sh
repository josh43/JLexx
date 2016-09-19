#!/usr/bin/env bash
# super advanced script incoming

sudo cp ../JLexx/LexLoader/jlgen /usr/bin/jlGen
sudo chown root:${USER} /usr/bin/jlGen
sudo cp ../JLexx/LexLoader/LexRunner/jlrun /usr/bin/jlRun
sudo chown root:${USER} /usr/bin/jlRun



sudo chmod +770 /usr/bin/jlRun
sudo chmod +770 /usr/bin/jlGen

sudo rm -f -R /usr/share/JLexx
sudo mkdir /usr/share/JLexx



sudo cp -f -R ../JLexx              /usr/share/JLexx/JLexx
sudo cp -f -R ../RegularExpression  /usr/share/JLexx/RegularExpression

sudo rm -f -R /usr/share/JLexx/JLexx/LexLoader/TestFiles
sudo rm -f -R /usr/share/JLexx/JLexx/LexLoader/GenFiles
sudo rm -f -R /usr/share/JLexx/JLexx/LexLoader/JLDebug
sudo rm -f -R /usr/share/JLexx/JLexx/LexLoader/*.o
sudo rm -f -R /usr/share/JLexx/JLexx/LexLoader/LexGenMain
sudo rm -f -R /usr/share/JLexx/JLexx/LexLoader/jlgen


sudo chown -R root:${USER} /usr/share/JLexx
sudo chmod -R +770 /usr/share/JLexx