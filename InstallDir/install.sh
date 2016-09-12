#!/usr/bin/env bash

sudo cp ../JLexx/LexLoader/jlgen /usr/bin/jlGen
sudo chown root:${USER} /usr/bin/jlGen
sudo cp ../JLexx/LexLoader/LexRunner/jlrun /usr/bin/jlRun
sudo chown root:${USER} /usr/bin/jlRun

sudo cp -f -R ../RegularExpression/*  /usr/share/JRegexp
sudo cp -f -R ../JLexx/*              /usr/share/JLexx

