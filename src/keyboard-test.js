/// <reference path="./lib/ioX-native.d.ts" />
const iox = require('../build/Release/ioX');

const keyCodes = iox.getCodes('keyboard');
const xboxCodes = iox.getCodes('xboxSeries');
//const mouseCodes = iox.getCodes('mouse');

const screenSize = iox.getScreenSize();
console.log('Screen Size = ', screenSize);
console.log('Test: Click Xbox A to press keyboard A.');

const intervalId = setInterval(() => {
    iox.getEvents(ev => {
        if (ev.type === 'buttonChange' && ev.id === xboxCodes.a) {
            iox.pressKey(keyCodes.a, ev.value);
        }
    });
}, 15);

