// Projet OSS117 : Eye origin 
// Version 1.0 
// Théo Falgarone, Alexis Hubert, Tristan Maunier, Christian Te


#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <stdio.h>
#include <math.h>
#include <string>

#include "constants.h"
#include "../protocol_constants.h"
#include "findEyeCenter.h"


/** Function Headers */
void detectAndDisplay( cv::Mat frame, std::string name, const char* save);
void write_eyes_pos(const char* save, std::string name, int leftx, int lefty, int rightx, int righty) ;
void findeyesandwrite() ;
std::string getpath() ;
std::string getdirectory(std::string pathf) ;
std::string exec(const char* cmd) ;

/** Global variables */
//-- Note, either copy these two files from opencv/data/haarscascades to your current folder, or change these locations
cv::String face_cascade_name = "haarcascade_frontalface_alt.xml";
cv::CascadeClassifier face_cascade;
cv::RNG rng(12345);
cv::Mat debugImage;
cv::Mat skinCrCbHist = cv::Mat::zeros(cv::Size(256, 256), CV_8UC1);
cv::Mat frame;

//get current path
std::string getpath(){
  std::string folder = "EyeLike/" ;
  std::string str = "pwd | sed -e 's/" + folder + "/'" ;
  const char* cmd_path = str.c_str() ;
  std::string path = exec(cmd_path) ;
  return path ;
}

//return result of a bash command
std::string exec(const char* cmd){
  char buffer[128];
  std::string result = "";
  FILE* pipe = popen(cmd, "r");
  if (!pipe) throw std::runtime_error("popen() failed!");
  try {
    while (!feof(pipe)) {
      if (fgets(buffer, 128, pipe) != NULL)
        result += buffer;
    }
  } catch (...) {
      pclose(pipe);
      throw;
  }
  pclose(pipe);
  size_t pos = 0 ;
  const std::string& search = "\n" ;
  const std::string& replace = "" ;
  while((pos = result.find(search, pos)) != std::string::npos){
    result.replace(pos, search.length(), replace) ;
    pos += replace.length() ;
  }
  return result;
}

//return user directory
std::string getdirectory(std::string path){
  std::cout << "Donnez le nom de l'utilisateur que vous souhaitez interpréter" << std::endl ;
  std::string user ;
  std::cin >> user ;
  std::string folder = "images/" ;
  std::string directory = path + folder + user + "/" ;
  return directory ;
}


//return the image's name
std::string transform_name(std::string dir, int i, int j, int k, int l){
  std::stringstream pos ;
  pos << i ;
  std::stringstream angle_h ;
  angle_h << j ;
  std::stringstream angle_v ;
  angle_v << k ;
  std::stringstream spot ;
  spot << l ;
  std::string extension = ".png" ;
  std::string name = dir + pos.str() + angle_h.str() + angle_v.str() + spot.str() + extension ;
  return name ;
}

void findeyesandwrite(){
  std::string path = getpath() ;
  std::string dir = getdirectory(path) ;
  std::string filesave = "eyelike.txt" ;
  std::string save2 = dir + filesave ;
  const char* save = save2.c_str() ;
  std::ofstream file(save) ;
  std::string entete = "Leftpupil.x Leftpupil.y Rightpupil.x Rightpupil.y" ;
  file << entete << std::endl ;
  for (int i=1 ; i<=nbpos ; i++){
  std::ofstream file(save,std::ios::app) ;
    for (int j=1 ; j<=nbangleh ; j++){
      for (int k=1 ; k<=nbanglev ; k++){
        file << i << " " << j << " " << k << " " << std::endl ;
        for (int l=1 ; l<=point ; l++){
          file << l << std::endl ;
          std::string name = transform_name(dir,i,j,k,l) ;
	  frame = cv::imread(name, CV_LOAD_IMAGE_COLOR) ;
        
          // Apply the classifier to the frame
          if( !frame.empty() ) {
            std::cout << name << std::endl ;
            detectAndDisplay(frame,name,save) ;
          }
          else {
            std::cout << " --(!) No captured frame -- ERROR ! \n" << name << std::endl ;
            file << "sha" << " pathimage\n" << 0 << " " << 0 << " " << 0 << " " << 0 << " " << std::endl ;
            //exit(0);
          }
        }
      }
    }
  }
}

void findEyes(cv::Mat frame_gray, cv::Rect face, std::string name, const char* save) {
  cv::Mat faceROI = frame_gray(face);
  cv::Mat debugFace = faceROI;

  if (kSmoothFaceImage) {
    double sigma = kSmoothFaceFactor * face.width;
    GaussianBlur( faceROI, faceROI, cv::Size( 0, 0 ), sigma);
  }
  //-- Find eye regions and draw them
  int eye_region_width = face.width * (kEyePercentWidth/100.0);
  int eye_region_height = face.width * (kEyePercentHeight/100.0);
  int eye_region_top = face.height * (kEyePercentTop/100.0);
  cv::Rect leftEyeRegion(face.width*(kEyePercentSide/100.0),
                         eye_region_top,eye_region_width,eye_region_height);
  cv::Rect rightEyeRegion(face.width - eye_region_width - face.width*(kEyePercentSide/100.0),
                          eye_region_top,eye_region_width,eye_region_height);

  //-- Find Eye Centers

  cv::Point leftPupil = findEyeCenter(faceROI,leftEyeRegion);
  cv::Point rightPupil = findEyeCenter(faceROI,rightEyeRegion);

  // change eye centers to face coordinates
  rightPupil.x += rightEyeRegion.x;
  rightPupil.y += rightEyeRegion.y;
  leftPupil.x += leftEyeRegion.x;
  leftPupil.y += leftEyeRegion.y;

  // change eye centers to originate image coordinates
  rightPupil.x += face.x;
  rightPupil.y += face.y;
  leftPupil.x += face.x;
  leftPupil.y += face.y;

  //Write in the file
  write_eyes_pos(save, name, leftPupil.x, leftPupil.y, rightPupil.x, rightPupil.y) ;
}

//Write in the file
void write_eyes_pos(const char* save, std::string name, int leftx, int lefty, int rightx, int righty){
  std::string sha = "sha1sum " ;
  std::string command2 = sha + name ;
  const char* command = command2.c_str() ;
  std::string shaandpath = exec(command) ;
  std::ofstream file(save, std::ios::app) ;
  file << shaandpath << "\n" << leftx << " " << lefty << " " << rightx << " " << righty << " " << std::endl ;
} 

/**
 * @function detectAndDisplay
 */
void detectAndDisplay(cv::Mat frame, std::string name, const char* save) {
  std::vector<cv::Rect> faces;
  //cv::Mat frame_gray;

  std::vector<cv::Mat> rgbChannels(3);
  cv::split(frame, rgbChannels);
  cv::Mat frame_gray = rgbChannels[2];

  //cvtColor( frame, frame_gray, CV_BGR2GRAY );
  //equalizeHist( frame_gray, frame_gray );
  //cv::pow(frame_gray, CV_64F, frame_gray);
  //-- Detect faces
  face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE|CV_HAAR_FIND_BIGGEST_OBJECT, cv::Size(150, 150) );

  for( int i = 0; i < faces.size(); i++ )
  {
    rectangle(frame, faces[i], 1234);
  }
  //-- Show what you got
  if (faces.size() > 0) {
    findEyes(frame_gray, faces[0], name, save);
  }
  else{
    //Write in the file
    write_eyes_pos(save, name, 0, 0, 0, 0) ;
  }
}


/**
 * @function main
 */
int main( int argc, const char** argv ) {

  // Load the cascades
  if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading face cascade, please change face_cascade_name in source code.\n"); return -1; };

  ellipse(skinCrCbHist, cv::Point(113, 155.6), cv::Size(23.4, 15.2),
          43.0, 0.0, 360.0, cv::Scalar(255, 255, 255), -1);
  findeyesandwrite() ;
  return 0;
}
