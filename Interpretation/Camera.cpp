// Projet OSS117 : Eye origin 
// Version 0.0 
// Théo Falgarone, Alexis Hubert, Tristan Maunier, Christian Te


#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Camera.hpp"
#include "../protocol_constants.h"

Camera::Camera(){
	position = " " ;
	angle_horizontal = " " ;
	angle_vertical = " " ;
	for(int i=0 ; i<point ; i++){
		left[i] = true ;
		right[i] = true ;
	}
	left_pourcentage = 100 ;
	right_pourcentage = 100 ;
}

Camera::Camera(int pos, int angle_h, int angle_v){
	position = Position[pos] ;
	angle_horizontal = Angle_horizontal[angle_h] ;
	angle_vertical = Angle_vertical[angle_v] ;
	for(int i=0 ; i<point ; i++){
		left[i] = true ;
		right[i] = true ;
	}
	left_pourcentage = 100 ;
	right_pourcentage = 100 ;
}

void Camera::set_false_left (int pos){
	left[pos] = false ;
	left_pourcentage -= 100.0/point ;
	if(left_pourcentage > 100){
		left_pourcentage = 100 ;
	}
	if(left_pourcentage < 0){
		left_pourcentage = 0 ;
	}
}

void Camera::set_false_right (int pos){
	right[pos] = false ;
	right_pourcentage -= 100.0/point ;
	if(right_pourcentage > 100){
		right_pourcentage = 100 ;
	}
	if(right_pourcentage < 0){
		right_pourcentage = 0 ;
	}
}

void Camera::affiche(){
	std::cout << "\nPosition : " << position << std::endl ;
	std::cout << "Angle horizontal : " << angle_horizontal << std::endl ;
	std::cout << "Angle vertical : " << angle_vertical << std::endl ;
	std::cout << "Left : " << left_pourcentage << std::endl ;
	std::cout << "Right : " << right_pourcentage << std::endl ;
}

std::string Camera::get_position(){
	return position ;
}

bool Camera::get_left(int pos){
	return left[pos] ;
}

bool Camera::get_right(int pos){
	return right[pos] ;
}

double Camera::get_left_pourcentage(){
	return left_pourcentage ;
}

double Camera::get_right_pourcentage(){
	return right_pourcentage ;
}
