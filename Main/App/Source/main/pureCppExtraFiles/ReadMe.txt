                              
                              Macaroni for C++
                               How to Compile


********************************************************************************
       Ubuntu 
********************************************************************************
      
Make sure you have the Boost libraries installed.  You'll also need the file
"site-config.jam" to be in your home directory.

Instructions on setting up Boost can be found here:
http://www.boost.org/doc/libs/1_46_1/more/getting_started/unix-variants.html#link-your-program-to-a-boost-library

For example, in Ubuntu, lets say you run the bootstrap script and then bjam,
and the Boost headers get copied to /usr/local/include and the libraries move to
/usr/local/lib.  Then you'd need to create a file named site-config.jam in your
home directory that looks like this:

==========================~/site-config.jam=====================================
import project ;
project.initialize $(__name__) ;
project site-config ;

using boost : 1.46.1
   : <include>/usr/local/include
     <library>/usr/local/lib
  ;
================================================================================

Finally you'll also need to have the file bjam on your path (its included in the
root of the Boost download) and you'll need to set the environment variable
BOOST_ROOT to the directory containing bjam.

So if you unzip the download to ~/boost_1_46_1, you'd need to type this:
export PATH=$PATH:~/boost_1_46_1/
export BOOST_ROOT=~/boost_1_46_1/

Now go to the pure C++ directory and run the command "bjam clean" just to make
sure everything works.  If you're set, the following command will build:

bjam --cflags=MACARONI_COMPILE_TARGET_LINUX
