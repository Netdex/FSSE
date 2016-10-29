# FSSE
Folder Spoofing Spreader Example

This application is an example of a spreading method which relies on user manipulation by replacing folders on removable media with a copy of itself,
spoofed to look akin to the original folder. When the unsuspecting user attempts to open the "folder" on another computer, 
the copy of itself will be run, and will spread to new removable devices plugged into that computer.

This application carries out the following sequence of logic:

1. If there exists a folder with the same name as this executable, open it to simulate opening the folder
2. Check for duplicate mutex
3. Add itself to startup
4. Wait for removable or network storage devices to be mounted
5. Hide all folders in root directory
6. Copy itself and rename itself to look like the hidden folder
7. Repeat from step 4
