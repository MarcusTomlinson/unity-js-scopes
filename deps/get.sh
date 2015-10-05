#!/bin/bash

DIR=$(readlink -f $(dirname ${BASH_SOURCE[0]}))
cd $DIR

# io.js

if [ -d "io.js" ]; then
    cd io.js; cd ..
else
    bzr branch http://bazaar.launchpad.net/~webapps/unity-js-scopes/io.js io.js
fi

# v8cpp

if [ -d "v8-cpp" ]; then
    cd v8-cpp; cd ..
else
    bzr branch http://bazaar.launchpad.net/~unity-team/v8-cpp/trunk v8-cpp
fi