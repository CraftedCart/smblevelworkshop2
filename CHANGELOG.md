SMB Level Workshop 2 Changelog
==============================

## Unreleased

These features are still a work in progress. You can download the latest development build from https://bintray.com/craftedcart/the-workshop/smblevelworkshop2-develop  
[ ![Download](https://api.bintray.com/packages/craftedcart/the-workshop/smblevelworkshop2-develop/images/download.svg) ](https://bintray.com/craftedcart/the-workshop/smblevelworkshop2-develop/_latestVersion)

### Added

- ws2editor
    - You can now define the fallout Y position

- ws2lz
    - Added support for exporting runtime reflective surfaces
    
- ws2common
    - Added support for `stageModel` parsing in XML configs
    - Added support for defining runtime reflective surfaces in the XML config (`<runtimeReflective>true</runtimeReflectime>` in `stageModel`)
    
### Deprecated

- ws2common
    - Using `levelModel` in XML configs has been deprecated - prefer `stageModel` instead

### Fixed

- ws2editor
    - Typing in the properties panel spinboxes no longer tries to move your caret around
    - Plugin loading now only loads relevant files (.so/.dylib/.dll - Fixes "5 plugins failed to load" on Windows)

## v1.0.0-beta.1 - 2018-06-23

[ ![Download](https://api.bintray.com/packages/craftedcart/the-workshop/smblevelworkshop2-beta/images/download.svg?version=v1.0.0-beta.1) ](https://bintray.com/craftedcart/the-workshop/smblevelworkshop2-beta/v1.0.0-beta.1/link)

Less janky to use than Workshop 1, still janky (This is beta 1 :P), animation not supported yet in the editor, and I hope you don't need to save anything.

### Added

- ws2editor
    - New graphical application
    - Lets you import models
    - You can place down the player start, goals, bumpers, bananas, jamabars, and wormholes
    - You can export your stage an an uncompressed LZ for Super Monkey Ball 2
    - There is a plugin system for extending the editor

    - Plugins
        - gizmosplugin
            - Provides translation manipulators to drag around selected objects
        - lzexportproviderplugin
            - Provides the Super Monkey Ball 2 raw LZ export option
        - propertiesproviderplugin
            - Provides an editable properties panel for selected objects
        - rendercolorsplugin
            - Colors placeables (Goals, bumpers, bananas, etc.) such that they are not just all white
        - ws2editorexampleplugin
            - A hello world example plugin
            - This just prints "Hello, world! - From ws2editorexampleplugin" when initialized, and "ws2editorexampleplugin: Window consructed!" when the StageEditorWindow is created

- ws2lzfrontend
    - New command line application
    - Lets you convert stage config XMLs and models to uncompressed LZs for Super Monkey Ball 2

- ws2lz
    - New library
    - Handles the conversion of scene data to a Super Monkey Ball 2 uncompressed LZ
    - Optimizes collision!

- ws2common
    - New library
    - Handles shared functionality and data across all other ws2 projects
    - Mostly handles stage and scene graph data, config parsing, and model importing

### Known issues

- Typing values into spinboxes is janky
- Resizing dockable panels may not work until you float and redock one of them (At least on Linux) - I believe this to not be my problem?
- Don't use the manipulators to drag around stage/background objects in the scene - it won't work
- Selecting nodes in the outliner tree doesn't select them in the scene view
- You can parent anything to anything, even if it doesn't make sense
- Please only drag one object at a time in the outliner - some stuff seems to disappear at random for whatever reason (And undo doesn't exist yet)
- No animation for ws2editor yet

