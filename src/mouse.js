/// <reference path="./lib/ioX-native.d.ts" />
const iox = require('../build/Release/ioX');

//const keyCodes = iox.getCodes('keyboard');
const xboxCodes = iox.getCodes('xboxSeries');
const mouseCodes = iox.getCodes('mouse');

const screenSize = iox.getScreenSize();

let mouseX = screenSize.width / 2;
let mouseY = screenSize.height / 2;
let mouseVelocityX = 0;
let mouseVelocityY = 0;
const mouseMaxSpeed = screenSize.width / 120;
const mouseDeadZone = 0.1;

console.log(`Mouse Control:

| Input                      | Output      |
| -------------------------- | ----------- |
| Right Stick                | Move Mouse  |
| Left Shoulder Button (LB)  | Left Click  |
| Right Shoulder Button (RB) | Right Click |
| Right Stick Click (R3)     | Exit App    |
`);

const intervalId = setInterval(() => {
    iox.getEvents(ev => {
        if (ev.type === 'axisChange' && ev.id === xboxCodes.rightStickX) {
            const isMax = ev.value > xboxCodes.rightStickXMax * mouseDeadZone;
            const isMin = ev.value < xboxCodes.rightStickXMin * mouseDeadZone;
            const multiplier = isMax ? 1 : isMin ? -1 : 0;
            const total = isMax
                ? xboxCodes.rightStickXMax
                : xboxCodes.rightStickXMin;

            mouseVelocityX = multiplier * ev.value / total * mouseMaxSpeed;
        }

        if (ev.type === 'axisChange' && ev.id === xboxCodes.rightStickY) {
            const isMax = ev.value > xboxCodes.rightStickYMax * mouseDeadZone;
            const isMin = ev.value < xboxCodes.rightStickYMin * mouseDeadZone;
            const multiplier = isMax ? 1 : isMin ? -1 : 0;
            const total = isMax
                ? xboxCodes.rightStickYMax
                : xboxCodes.rightStickYMin;

            mouseVelocityY = multiplier * ev.value / total * mouseMaxSpeed;
        }

        if (ev.type === 'buttonChange' && ev.id === xboxCodes.lb) {
            iox.setMouse(mouseX, mouseY, mouseCodes.left, ev.value);
        }
        if (ev.type === 'buttonChange' && ev.id === xboxCodes.rb) {
            iox.setMouse(mouseX, mouseY, mouseCodes.right, ev.value);
        }
        if (ev.type === 'buttonChange' && ev.id === xboxCodes.r3) {
            clearInterval(intervalId);
        }
    });

    if (mouseVelocityX !== 0 || mouseVelocityY !== 0) {
        mouseX += mouseVelocityX;
        mouseY += mouseVelocityY;
        iox.setMouse(mouseX, mouseY, mouseCodes.none, true);
    }

}, 15);

