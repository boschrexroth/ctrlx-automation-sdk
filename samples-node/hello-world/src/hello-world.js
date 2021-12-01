#!/usr/bin/env node
function helloWorld() {
    setTimeout(function () {
        var now = new Date();
        console.log('Hello World from node.js!', now.toUTCString());
        helloWorld();
    }, 1000)
};

helloWorld();