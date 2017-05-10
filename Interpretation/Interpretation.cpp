// Projet OSS117 : Eye origin 
// Version 0.0 
// Théo Falgarone, Alexis Hubert, Tristan Maunier, Christian Te


#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

#include "Camera.hpp"
#include "../protocol_constants.h"


bool compare_pos(int pos1, int pos2){
  int diff = std::abs(pos2 - pos1) ;
  if (diff > 3){
    return true ;
  }
  return false ;
}

bool compare_int(int a, int b){
  if(a==b){
    return true ;
  }
  return false ;
}

bool compare_string(std::string a, std::string b){
  if(a.compare(b)==0){
    return true ;
  }
  return false ;
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

//get current path
std::string getpath(){
  std::string folder = "Interpretation/" ;
  std::string str = "pwd | sed -e 's/" + folder + "/'" ;
  const char* cmd_path = str.c_str() ;
  std::string path = exec(cmd_path) ;
  return path ;
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

//retourne le fichier de l'algorithme à tester
std::string getalgodir(std::string dir){
  std::cout << "Donnez le nom de l'algorithme que vous souhaitez utiliser" << std::endl ;
  std::string algo ;
  std::cin >> algo ;
  std::transform(algo.begin(),algo.end(),algo.begin(), ::tolower) ;
  std::string extension = ".txt" ;
  std::string algofile = dir + algo + extension ;
  return algofile ;
}

//retourne le fichier de reference
std::string getrefdir(std::string dir){
  std::string save = "positionReference.txt" ;
  std::string reffile = dir + save ;
  return reffile ;
}

std::vector<Camera> collect_data(std::vector<Camera> cam){
  std::string path = getpath() ;
  std::string dir = getdirectory(path) ;
  std::string algofile2 = getalgodir(dir) ;
  const char* algofile = algofile2.c_str() ;
  std::string reffile2 = getrefdir(dir) ;
  const char* reffile = reffile2.c_str() ;
  std::ifstream file(algofile) ; //on ouvre le premier fichier
  std::ifstream file2(reffile) ; //on ouvre le deuxième fichier
  std::string ligne ;
  if(file && file2){
    while(getline(file,ligne)&&getline(file2,ligne)){
      int position , angle_horizontal , angle_vertical ;
      file >> position ;
      file >> angle_horizontal ;
      file >> angle_vertical ;
      int position2 , angle_horizontal2 , angle_vertical2 ;
      file2 >> position2 ;
      file2 >> angle_horizontal2 ;
      file2 >> angle_vertical2 ;
      position-- , angle_horizontal-- , angle_vertical-- ;
      Camera c (position,angle_horizontal,angle_vertical) ; //on cree un objet camera 
      for(int i=0 ; i<point ; i++){
        int nbimg, nbimg2, x1, x2, y1, y2 ;
        std::string sha, sha2, path, path2 ;
        file >> nbimg ; //recupere le numero de l'image cad du point apparu a l'ecran
        file2 >> nbimg2 ;
        file >> sha ; //recupere le sha
        file2 >> sha2 ;
        file >> path ; //recupere le chemin
        file2 >> path2 ;
        if(compare_int(nbimg, nbimg2)  && compare_string(sha, sha2) && compare_string(path, path2)){
          file >> x1 ; //on recupere les coordonnes des pixels pour left
          file2 >> x2 ;
          file >> y1 ;
          file2 >> y2 ;
          if(compare_pos(x1,x2) || compare_pos(y1,y2)){ //on test les pixels pour left
            c.set_false_left(i) ;
          }
          file >> x1 ; //on recupere les coordonnes des pixels pour right
          file2 >> x2 ;
          file >> y1 ;
          file2 >> y2 ;
          if(compare_pos(x1,x2) || compare_pos(y1,y2)){ //on test les pixels pour right
            c.set_false_right(i) ;
          }
        }
        else{
          c.set_false_right(i) ;
          c.set_false_left(i) ;
          std::cout << "Erreur lecture image " << path << std::endl ;
          for(int j=0 ; j<4 ; j++){
            file >> x1 ;
            file2 >> x2 ;
          }
        }
      }
      cam.push_back(c) ;
    }
  }
  else{
    std::cout << "ERREUR OUVERTURE FICHIER" << std::endl ;
  }
  cam.pop_back() ;
  return cam ;
}

//affiche tout les objets cameras
void affiche(std::vector<Camera> cam){
	for (int i=0 ; i<cam.size() ; i++)
	{
		cam[i].affiche(); 
	}
}

//affiche les cameras avec 100% de reussite pour right et left
void affiche100(std::vector<Camera> cam){
	for (int i=0 ; i<cam.size() ; i++)
	{
		if(cam[i].get_left_pourcentage() == 100 && cam[i].get_right_pourcentage() == 100){
			cam[i].affiche();
		}
	}
}

//on détermine si un couple de caméra est correct cad si l'on détecte correctement les deux yeux pour les 9 points
void interpret(std::vector<Camera> cam, int j, int nextpos){
  for(int k=j+nextpos; k<cam.size() ; k++){
    if(cam[k].get_left_pourcentage()==100 && cam[k].get_right_pourcentage()==100){
    }
    else{
      for(int i=0 ; i<point ; i++){
        int test = i ;
        test++ ;
        if(cam[j].get_left(i)==false && cam[k].get_left(i)==false){
          break ;
        }
        else if(cam[j].get_right(i)==false && cam[k].get_right(i)==false){
          break ;
        }
        else if(test==point){
          std::cout << "\nLe couple de caméra suivant est correct : " << std::endl ;
          cam[j].affiche() ;
          cam[k].affiche() ;
        }
      }
    }
  }
}

void affiche_interpret(std::vector<Camera> cam){
  for(int j=0 ; j<cam.size() ; j++){
    if(cam[j].get_left_pourcentage()==100 && cam[j].get_right_pourcentage()==100){
    }
    else{
      int diff = j + nbcombangle ;
      diff %= nbcombangle ;
      int nextpos = nbcombangle - diff ;
      interpret(cam,j,nextpos) ;
    }
  }
}

void globalmean(std::vector<Camera> cam){
  double meanl = 0 , meanr = 0 ;
  for(int i=0 ; i<cam.size() ; i++){
    meanl+=cam[i].get_left_pourcentage() ;
    meanr+=cam[i].get_right_pourcentage() ;
  }
  meanl/=cam.size() ;
  meanr/=cam.size() ;
  std::cout << "Moyenne oeil gauche : " << meanl << "\nMoeynne oeil droit : " << meanr << std::endl ;
}

void menu()
{
  std::cout << "\n_____________MENU_____________\n"<< std::endl ;
  std::cout << "1- Changer d'utilisateur à interpréter" << std::endl ;
  std::cout << "2- Afficher toutes les caméras" << std::endl ;
  std::cout << "3- Afficher les caméras ayant 100% aux deux yeux" << std::endl ;
  std::cout << "4- Afficher la moeynne des pourcentages de réussite sur l'ensemble des caméras pour chaque oeil" << std::endl ;
  std::cout << "5- Afficher les couples de caméras correct" << std::endl ;
  std::cout << "0- Quitter" << std::endl ;
}

int main(){
  std::vector<Camera> cam ;
  cam = collect_data(cam) ;
  //affiche100(cam) ;
  //affiche(cam) ;
  //affiche_interpret(cam) ;
  int rep(0) ;
  bool loop = true ;
  while(loop){
    menu() ;
    std::cin >> rep ;
    switch(rep){
      case 0 : loop = false ; break ;
      case 1 : cam = collect_data(cam) ; break ;
      case 2 : affiche(cam) ; break ;
      case 3 : affiche100(cam) ; break ;
      case 4 : globalmean(cam) ; break ;
      case 5 : affiche_interpret(cam) ; break ;
      default : std::cout << "Erreur saisie ! Veuillez recommencer" << std::endl ; break ;
    }
  }
  return 0 ;
}
