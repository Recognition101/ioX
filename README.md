# Input-Output Multiplexer: ioX

This is a minimalist framework for writing NodeJS scripts that map gamepad input to keyboard and mouse output. It works as follows:

 * The NodeJS C++ Addon uses SDL to read gamepad state and provides it to NodeJS.
 * The NodeJS C++ Addon provides OS-specific C++ functions that emulate key presses or mouse movement/presses.
 * The NodeJS script runs an event loop that reads certain controller inputs and triggers keyboard/mouse output.

## Setup

### Setup: macOS

 1. Install [homebrew](https://brew.sh).
 2. Install dependencies: `brew install node sdl2 sdl2_image`
 3. In this directory, build: `npm run build`
 4. Run a script, ex: `node ./src/mouse.js`

### Setup: Windows

 1. Install [Chocolatey](https://chocolatey.org)
 2. Install NodeJS: `choco install nodejs.install`
 3. Install Visual Studio: `choco install visualstudio2022community`
 4. Open Visual Studio &rarr; Workloads &rarr; Desktop development with C++ &rarr; <kbd>Install</kbd>
 5. In this directory, build: `npm run build`
 6. Run a script, ex: `node ./src/mouse.js`
