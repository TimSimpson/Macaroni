How to Build Macaroni
=====================


Summary
-------

The next version of Macaroni is always built by using it's own last version and
changing the generatorPath to the newer version (this allows certain new
features to be used even while compiling Macaroni itself).

After that, unit tests are run, and finally, the ubuntu version is built using
Vagrant bash scripts.

Once this is finished, integration tests are run to verify that everything
is ok, and the website is build. This process uses Macaroni project files.


Pre-reqs
--------

1. Install Boost Build. You need to be able to access bjam from the terminal.
2. Install the latest version of Macaroni so its available on the PATH.

Option for speeding up the build:
You may want to change $USERS/Macaroni/init.lua (%USERS%\Macaroni\init.lua) to
contain the following:

    properties.bjam_options = "-d+2 -j8 --toolset=msvc-11.0" or whatever.

The j8 option causes Boost Build to run on eight threads which can speed up
compilations on speedier machines (and slow it down if there aren't eight
processor cores).



Building
--------

1. Enter the directory Main/App and run the following:

    macaroni clean build generatorPath ..\Generators

  This compiles Macaroni using itself along with the version of Generators the
  version of Macaroni you're compiling will use.


Running Bleeding Edge version "Cavatappi"
-----------------------------------------

The bleeding edge version of Macaroni named "Cavatappi", which is a word for
macaroni (the food) shaped like a corkscrew. It's the kind of macaroni that
most resembles the tilde.

Macaroni has multiple arguments to change paths to stuff, such as the built-in
libraries or generators (the ones installed next to Macaroni). The directory
"Main/Cavatappi" has a batch file named "cavatappi" to run this in Windows.

1. Add Main/Cavatappi to the PATH, and invoke the it using "cavatappi".


Creating a Release
------------------

Making an official release is much trickier and involves building the Ubuntu version and running unit tests. To do it, you need to build Macaroni and make that "Cavatappi" version available. You will also need to install the tool Vagrant.

1. Enter the Ports/Vagrant/Ubuntu32 and run "vagrant up" which will create a 32-bit Ubuntu VM if none exists.

2. Type "vagrant ssh" to enter this VM (or shell into it somehow).

3. Run:

    $ /vagrant/mbuild.sh install
    $ /vagrant/mbuild.sh build
        Tip: If you run into errors, back on your host machine rebuild with "macaroni build generatorPath ..\Generators" to update the C++ code and try to run "mbuild.sh build" again. Note that the "build" phase copies code to a directory called "PureCpp", which simplifies the build process for Ubuntu is simplified.
    $ /vagrant/mbuild.sh all-tests
        Tip: If things fail, mbuild.sh "run" can run the built Macaroni (Cavatappi) in the VM. "debug" will debug it using gdb, which you'll need to install first with "sudo apt-get install gdb".
    $ /vagrant/mbuild.sh build-release

4. Enter the Ports/Vagrant/Ubuntu64 directory and run "vagrant up". Now, ssh into it the way you did the 32 bit VM and repeat step 3.

5. Finally, enter the "Next" directory and run "cavatappi clean install"
   Sometimes, the Boost tests won't compile. At various points, I've had to
   define BOOST_TEST_DYN_LINK and then at other times had to undefine it. At
   this point, I've stopped caring. Instead, if it complains about linker
   errors, just run this at the command prompt:

   > SET CL=/DBOOST_TEST_DYN_LINK

   That will define the symbol BOOST_TEST_DYN_LINK in case its needed.
