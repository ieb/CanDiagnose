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
cp src/admin.html ../../data/admin.html
cp src/admin.js ../../data/admin.js
cp src/admin.css ../../data/admin.css

cd ../../

cp lib/TFTDisplay/images/*.jpg data/

pio run --target buildfs --environment nodemcu-32s
ls -l data
ls -l .pio/build/nodemcu-32s/spiffs.bin
pio run --target uploadfs --environment nodemcu-32s
