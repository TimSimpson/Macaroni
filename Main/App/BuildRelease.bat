bjam -d+2 -j8 --toolset=msvc GeneratedSource link=static release threading=multi
bjam -d+2 -j8 --toolset=gcc cxxflags=-std=gnu++11 GeneratedSource link=static release threading=multi
