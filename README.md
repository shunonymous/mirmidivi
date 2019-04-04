mirmidivi
=========

What's mirmidivi?
-----------------

mirmidivi is midi-visualization and making midi-animation tool. (mirmidivi is rendering midi as visual)

Required
--------

mirmidivi needs to build and run below softwares, so at the first, install it using package manager on your system.

* [CMake](https://cmake.org/)
* [gcc](https://gcc.gnu.org/) or [clang](https://clang.llvm.org/)
* curses (Now, tested only [ncurses](https://www.gnu.org/software/ncurses/))
* [fluidsynth](http://www.fluidsynth.org/)
* [Boost C++ Libraries](http://www.boost.org/)

Build
-----

Tested on Arch Linux only.

Clone repository and update submodule.

	$ git clone --recurse-submodules https://github.com/shunonymous/mirmidivi
	$ cd mirmidivi

CMake in build directory.

You can set install directory by `-DMIRMIDIVI_INSTALL_DIR`.

	$ mkdir build
	$ cd build
	$ cmake .. -DMIRMIDIVI_INSTALL_DIR=~/mirmidivi
	$ make -j4
	$ make install
	
On this example, mirmidivi installed to ~/mirmidivi (Currentaly, not recommended to install to system).

Quick Start
-------

Set pathes for mirmidivi, and run.

	$ export PATH=${PATH}:~/mirmidivi/bin
	$ export LD_LIBRARY_PATH=~/mirmidivi/lib
	$ mirmidivi
	
(n)curses interface is...

	$ mirmidivi -g curses 2> /dev/null

Now, mirmidivi opened midi port, so you can see it (named "fluidsynth for mirmidivi")

Serch mirmidivi's port number.

	$ aconnect -o | grep mirmidivi
	client 129: 'fluidsynth for mirmidivi' [type=user,pid=29865]
	    0 'fluidsynth for mirmidivi'

In this case, mirmidivi's port number is 129.

OK, Let's send MIDI to mirmidivi!

	$ aplaymidi -p 129 /path/to/your/favorite/smf/file.mid
	
mirmidivi has some options. see `mirmidivi -h`.

License
-------

mirmidivi is developed as a free(libre) software licensed under GNU GPL v3.

Author
------

Shun Terabayashi

mail:[shunonymous@gmail.com](mailto:shunonymous@gmail.com)

GitHub:[@shunonymous](https://github.com/shunonymous)

Twitter:[@shunonymous](https://twitter.com/shunonymous)(Japanese)

Mastodon:[@shunonymous@pawoo.net](https://pawoo.net/@shunonymous)(Japanese)
