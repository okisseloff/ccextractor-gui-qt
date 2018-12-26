# ccextractor-gui-qt
![logo](cc.png)
Qt-based GUI for CCExtractor (https://github.com/CCExtractor/ccextractor)

## Compilation guide

`1. Install QTCreator on your computer:   [QT Creator link](https://www.qt.io/download) with QT5.`

`2. Download or clone the repository on your computer.`

`3. Open .pro file with QTCreator.`

![.pro file](https://ia601500.us.archive.org/9/items/makarov18042003_gmail_1/1.png)

`4. Make sure that you set Release mode(If not just change it from debug to release).`

![release mode](https://ia601507.us.archive.org/14/items/release_20181203/release.png)

`5. Then click on build -> build project`

![building in qt](https://ia601504.us.archive.org/33/items/makarov18042003_gmail_3/3.png)

`6. Open a folder where you extracted project by step 2 and choose folder that was created after you built CCAligner_GUI in qt creator and open release folder. Your application must be there.`

Note: If you are on Linux or Mac it must already work for you. But if you are on windows read the next points.

`7. Then open your QT(not QT creator) in console and write next: windeployqt.exe --release "executable_path"
Here executable path is path to release folder when your .exe file is situated. `

`8. Finally, you will get all your dependencies in release folder. So you can just move it to the folder with your executable file and run.`

#### To work with CCExtractor_GUI_QT you have to compile ccextractor(console version) and save it to the same folder where CCExtractor_GUI_QT executable is situated. If you are on linux/mac name of console version must be ./ccextractor if you are on windows: ccextractor.exe. Or you can write it in the System PATH so you don't need to move it to another folder.
