# EasyTourney
A simple, easy to use tournament bracket manager which uses the multi-platform GUI library wxWidgets.

## Installation 

**Linux Installation: Building from Source**  
  
1. First, install the development files for GTK+2 (or whatever your windowing system is):
  ```  
  $ sudo apt-get install libgtk2.0-dev  
  OR  
  $ sudo yum install gtk2-devel
  ```
  
2. Download the wxWidgets source code from http://wxwidgets.org/downloads/

3. Open a terminal, extract the archive, cd into the wxWidgets directory, and create a new build directory:
  ```
  $ tar xf wxWidgets-3.0.2.tar.bz2
  $ cd wxWidgets-3.0.2  
  $ mkdir build-gtk
  ```
  
4. Cd into the new build directory and call the configure script, replacing gtk=2 with your windowing system (gtk, x11, osx-cocoa, etc)
  ```
  $ cd build-gtk
  $ ../configure --with-gtk=2 --disable-shared
  ```
  
5. Call make to build the wxWidgets library. Warning: this step takes about 10 minutes and 2GB of hard drive space (which you can safely delete after the end of this installation tutorial)
  ```
  $ make
  ```
  
6. Download the EasyTourney source code, cd into its base directory, and make a build directory: 
  ```
  $ cd EasyTourney/
  $ mkdir build/
  ```

7. Open the Makefile with a text editor and modify the first line so it has the **full** path to the wxWidgets build directory that you created in step 3. It will look something like: 
  ```
  WX_BUILD = /home/user/Downloads/wxWidgets-3.0.2/build-gtk
  ```

8. Now just run make, and the executable EasyTourney file should appear in the EasyTourney/build directory
  ```
  $ make
  ```
  
* * *
  
**OSX Installation: Building from Source**  

1. Make sure you have at least OSX 10.7 and development tools (XCode) installed.  

2. Download the wxWidgets source code from http://wxwidgets.org/downloads/  

3. Open a terminal, extract the archive, cd into the wxWidgets directory, and create a new build directory:  
  ```
  $ tar xf wxWidgets-3.0.2.tar.bz2
  $ cd wxWidgets-3.0.2  
  $ mkdir build-osx
  ```  
  
4. Cd into the new build directory and call the configure script.  
  ```
  $ cd build-osx
  $ ../configure --disable-shared --disable-webview --disable-webview-webkit --with-macosx-version-min=10.7
  ```  
  
5. Call make to build the wxWidgets library. Warning: this step takes about 10 minutes and 2GB of hard drive space (which you can safely delete after the end of this installation tutorial)  
  ```
  $ make
  ```  
  
6. Download the EasyTourney source code, cd into its base directory, and make a build directory:  
  ```
  $ cd EasyTourney/
  $ mkdir build/
  ```  
  
7. Open Makefile-OSX with a text editor and modify the first line so it has the **full** path to the wxWidgets build directory that you created in step 3. It will look something like:  
  ```
  WX_BUILD = /Users/username/Downloads/wxWidgets-3.0.2/build-osx
  ```  

8. Now just run make with Makefile-OSX, and the executable EasyTourney file should appear in the EasyTourney/build directory  
  ```
  $ make -f Makefile-OSX
  ```  

**Windows Installation: Building from Source**  
See how to set up wxWidgets in Microsoft Visual Studio here: https://wiki.wxwidgets.org/Microsoft_Visual_C%2B%2B_Guide
