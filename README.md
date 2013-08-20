# Gem #
Gem (Gielinor Emulator) aims to provide an accurate emulation of the Runescape server as of 2006. The core of Gem is written in C, with higher level game content written via a script API accessible through Python. Gem is written as an educational tool to better understand how a scalable MMO server engine can be implemented, and to explore the best practices for writing robust and stable server code. As such, it is not intended for commercial use, and the authors do not condone using the code for anything other than educational purposes. No proprietary code or resources are to be distributed with Gem, nor can we tell you where to obtain such a thing.

## Requirements ##
Gem depends on the following external libraries:

- libev
- zlib
- libgmp
- Python 3 (and development libraries)

Gem is written for Linux and compiled with GCC. It uses both Linux specific libraries, and GCC extensions, and as such it is unlikely to compile for dissimilar operating systems (ie. Windows). I am however open to accepting ports to other operating systems/compilers, provided you follow the submission critera given in the 'Contributing' section below.

## Building ##
When cloning this repo, it is important that you make sure that submodules are cloned also. The easiest way to do this is to clone with `--recursive`, like so:

	$ git clone --recursive git@github.com:kaye64/gem.git
	
Once you have the code, build by invoking make from the root directory:

	$ make
	$ ./gem

## Contributing ##
Contributions to the code are welcomed and encouraged provided your code adds value, and is written clearly and using the same conventions as are used throughout. In future I will write up a document explaining the coding style that the project uses, but for now just refer to the code for examples.

When submitting code back to the project, please use the Github fork/pull model. That is, fork the repository, make your commits there, and submit them back to the main repository via a Pull Request, detailing what changes you've made as best you can. For more info on this workflow see [here](https://help.github.com/articles/using-pull-requests). If your submission is related to any of the issues in the issue tracker, please reference them by their ids in your pull request so I can update the issues if needed.

I am committed to keeping the codebase as clean as possible, which means I may make suggestions for improvement or turn down some pull requests. Please don't be disheartened by this, it's probably nothing personal. If you think it's appropriate, feel free to modify your submission and re-submit it for me to consider.

All contributions will be noted and your name will be noted in the Authors file (once I get around to creating one, that is..). You'll also receive fame and endless breadsticks for being an awesome person.

## License ##
Gem is licensed under the GNU GPL (General Public License) Version 3. The GNU GPL v3 can be viewed in full inside the LICENSE file of this directory, but for a brief introduction to what this license means for you, see [here](http://www.gnu.org/licenses/quick-guide-gplv3.html).
