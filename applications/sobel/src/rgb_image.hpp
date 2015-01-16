/*
 * rgb_image.hpp
 * 
 * Created on: Sep 9, 2013
 * 			Author: Amir Yazdanbakhsh <a.yazdanbakhsh@gatech.edu>
 */

 #ifndef __RGB_IMAGE_HPP__
 #define __RGB_IMAGE_HPP__

 #include <boost/shared_ptr.hpp>
 #include <boost/algorithm/string.hpp>
 #include <boost/algorithm/string/regex.hpp>
 #include <boost/lexical_cast.hpp>


 #include <vector>
 #include <fstream>
 #include <stdlib.h>
 #include <iostream>


 #define DEBUG 0

 class Pixel {
 public:
 	Pixel (float r, float g, float b)
 	{
 		this->r = r ;
 		this->g = g ;
 		this->b = b ;
 	}
 	float r ;
 	float g ;
 	float b ;
 } ;

 class Image {
 public:
 	int 			width ;
 	int 			height ;
 	std::vector 	<std::vector<boost::shared_ptr<Pixel> > > pixels ;
 	std::string 	meta ;

 	// Constructor
 	Image()
 	{
 		this->width  = 0 ;
 		this->height = 0 ;
 	}

 	int loadRgbImage (std::string filename) ;
 	int saveRgbImage (std::string outFilename, float scale) ;
 	void makeGrayscale() ;
 	void printPixel(int x, int y) ;
 } ;

 #endif