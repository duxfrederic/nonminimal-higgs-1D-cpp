// file by: K. Steiner, J. Dominski, N. Ohana
#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <map>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

using namespace std;

class ConfigFile{
  
  public:
      ConfigFile(const std::string& filename);
      ~ConfigFile();
      
      template<typename T> T get(const std::string& key) const;
      
      void process(const std::string& lineread);
      
      std::string toString() const;
      
      void printOut(std::string path) const;
      
  private:
      std::string trim(std::string str);
      
      std::map<std::string, std::string> configMap;
};

#endif
