// Projet OSS117 : Eye origin 
// Version 0.0 
// Théo Falgarone, Alexis Hubert, Tristan Maunier, Christian Te


#ifndef CAMERA_H__
#define CAMERA_H__

#include <iostream>
#include <string>
#include <vector>

#include "../protocol_constants.h"

class Camera{
	protected :
		std::string position ;
		std::string angle_horizontal ;
		std::string angle_vertical ;
		bool left[point] ;
		bool right[point] ;
		double left_pourcentage ;
		double right_pourcentage ;
	public :
		Camera() ;
		Camera(int position, int angle_horizontal, int angle_vertical) ;
		void set_false_left(int pos) ;
		void set_false_right(int pos) ;
		void affiche() ;
		std::string get_position() ;
		std::string get_angle_horizontal() ;
		std::string get_angle_vertical() ;
		bool get_left(int pos) ;
		bool get_right(int pos) ;
		double get_left_pourcentage() ;
		double get_right_pourcentage() ;
};

#endif
