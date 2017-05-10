// Projet OSS117 : Eye origin 
// Version 1.0 
// Théo Falgarone, Alexis Hubert, Tristan Maunier, Christian Te


#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <stdexcept>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <string>

#include "../protocol_constants.h"

//headers
std::string exec(const char* cmd) ;
std::string getpath() ;
std::string mkdir_user() ;
std::string transform_name(std::string path, int i, int j, int k, int l) ;


//get current path
std::string getpath(){
  std::string folder = "Protocol/" ;
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


//créer le sous-dossier de l'utilisateur dans le dossier images 
std::string mkdir_user(){
  std::string path = getpath() ;
  std::string user ;
  std::cout << "Donnez le nom de l'utilisateur effectuant ce test" << std::endl ;
  std::cin >> user ;
  std::string folder = "images/" ;
  path = path + folder + user + "/" ;
  std::string command2 = "mkdir " + path ; 
  const char* command = command2.c_str() ;
  int b = system(command) ;
  /*std::string erreur = "Attention cet utilisateur existe déjà, vous allez tout effacer\n" ;
  std::string message = "mkdir: impossible de créer le répertoire «" + path + "»: Le fichier existe" ;
  bool a = true ;
  while(a){
    bool b = false ;
    try{
      int c = system(command) ;
      throw message.c_str() ;
    }
    catch( const char* errormessage){
      std::cout << erreur << std::endl ;
      b = true ;
    }
    if(b){
      mkdir_user() ;
    }
  a = false ;
  }*/
  return path ;
}

//return the image's name
std::string transform_name(std::string path, int i, int j, int k, int l){
  i++,j++,k++ ;
  std::stringstream pos ;
  pos << i ;
  std::stringstream angle_h ;
  angle_h << j ;
  std::stringstream angle_v ;
  angle_v << k ;
  std::stringstream spot ;
  spot << l ;
  std::string extension = ".png" ;
  std::string name = path + pos.str() + angle_h.str() + angle_v.str() + spot.str() + extension ;
  return name ;
}

int main( int argc, const char** argv ) {
  cv::Mat frame ;
  std::string path = mkdir_user() ;
  std::cout << "Début du test" << std::endl ;
  for (int i=0 ; i<nbpos ; i++){
    for (int j=0 ; j<nbangleh ; j++){
      for (int k=0 ; k<nbanglev ; k++){
        std::cout << "Position camera : " << Position[i] << std::endl ;
        std::cout << "Angle horizontal camera : " << Angle_horizontal[j] << std::endl ;
        std::cout << "Angle vertical camera : " << Angle_vertical[k] << std::endl ;
        for (int l=1 ; l<=point ; l++){
          std::string name = transform_name(path,i,j,k,l) ;
          std::cout << "Point ecran " << l << std::endl ;
          std::cout << name << std::endl ;
          std::string a ;
          std::cin >> a ;
          cv::VideoCapture capture(1); //-1 équivaut à la webcam de l'ordinateur ; 1 correspond à la caméra connectée à l'ordinateur par le port USB
          capture.read(frame);

          // mirror it
          cv::flip(frame, frame, 1);

          // Apply the classifier to the frame
          if( !frame.empty() ) {
            imwrite(name,frame) ;
          }
          else {
            std::cout << " --(!) No captured frame -- ERROR \n Try Again or EXIT!\n" << name << std::endl ;
            l--;
          }
        }
      }
    }
  }
  return 0;
}
