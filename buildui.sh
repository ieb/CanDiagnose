#!/bin/bash

FILES="
    calcs
    eink
    index
    json2
 "

function minify {
    npx minify $1 > $2
}

cd ui/einkweb
for i in ${FILES}
do
    minify src/${i}.js ../../data/${i}.min.js
done

cp src/index.html ../../data/index.html

cd ../../

pio run --target buildfs --environment nodemcu-32s
ls -l data
ls -l .pio/build/nodemcu-32s/spiffs.bin
pio run --target uploadfs --environment nodemcu-32s
