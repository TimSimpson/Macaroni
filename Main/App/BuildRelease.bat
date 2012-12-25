SET BOOST_LIB_SUFFIX=-mgw47-mt-1_52
bjam -d+2 -j8 --toolset=gcc cxxflags=-std=gnu++11 link=static release threading=multi
