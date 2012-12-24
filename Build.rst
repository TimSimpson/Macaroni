How to Build
------------

1. Have the last version of Macaroni on hand.
2. Enter Main/App, and run "macaroni clean build". The new version will be
   created.
3. Add Main/Cavatappi to the PATH. Now, from Tests directory, run
   "cavatappi clean install." This will run all the integration tests.
4. Enter the Vagrant directory, and type "vagrant up". From this VM,
   run /macaroni/build install, then build. Run unit-tests and test-all to
   make sure it works.
5. From the Vagrant/Precise32 directory, type "vagrant up" and enter than VM.
   Do the same thing as before, but only do install. For "build", add the
   argument "instruction-set=i686 address-model=32 architecture=x86".
6. Now, back on the host machine, enter Release and type
   "cavatappi clean build"./
7. Enter Release/target/
