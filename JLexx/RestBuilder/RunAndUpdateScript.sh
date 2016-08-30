#!/bin/bash
#make sure to install and include express and multer in your package.json
make
./main
theSource=${PWD}/Rest/
objcTarget="/Users/josh/Documents/CS Projects/RestExample/RestExample/"
jsTarget="/Users/josh/Documents/CS Projects/RestGeneratorTester/routes/"
echo "Copying files at $theSource to   $objcTarget"

cp -f -R "${theSource}/OBJC" "${objcTarget}"
cp -f -R "${theSource}/JS/" "${jsTarget}"

make clean