/*
 * Config.cpp
 *
 *  Created on: Jul 16, 2013
 *      Author: sam
 */

#include "Config.h"

#if defined(EMBEDDED_MODE)
#else
//
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <cassert>

Config::Config() :
    modified(false), persistenceMode(true)
{
}

Config::Config(const std::string& name, const std::string& path) :
    name(name), path(path), modified(false), persistenceMode(true)
{
}

Config::~Config()
{
}

void Config::setName(const std::string& name)
{
  this->name = name;
}

void Config::setPath(const std::string& path)
{
  this->path = path;
}

void Config::setPersist(const bool& persistenceMode)
{
  this->persistenceMode = persistenceMode;
}

inline void configSplit(const std::string &s, const char& delim, std::vector<std::string>& tokens)
{
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim))
  {
    tokens.push_back(item);
  }
}

void Config::resurrect()
{
  std::stringstream ss;
  if (path.empty())
    ss << name << ".cfg";
  else
    ss << path << "/" << name << ".cfg";
  std::string input(ss.str());
  std::ifstream inputStream(input.c_str());
  if (inputStream.is_open())
  {
    std::string str;
    while (getline(inputStream, str))
    {
      if (str.length())
      {
        unsigned int found = str.find_first_of("#");
        if ((found == std::string::npos) && (found > str.size()))
        {
          std::vector<std::string> tokens;
          configSplit(str, '=', tokens);
          assert(tokens.size() == 2); // Only name=value pairs
          values.insert(std::make_pair(tokens[0], tokens[1]));
        }
      }
    }
    inputStream.close();
    std::cout << "INFO! config=" << input << " resurrected!" << std::endl;
  }
  else
  {
    std::cout << "INFO! config=" << input << " empty!" << std::endl;
  }
}

void Config::persist()
{
  std::stringstream ss;
  if (path.empty())
    ss << name << ".cfg";
  else
    ss << path << "/" << name << ".cfg";
  std::string output(ss.str());

  if (persistenceMode)
  {
    if (modified)
    {
      std::ofstream outputStream(output.c_str());
      if (outputStream.is_open())
      {
        for (Values::const_iterator iter = values.begin(); iter != values.end(); ++iter)
          outputStream << iter->first << "=" << iter->second << std::endl;
      }
      else
        std::cerr << "ERROR! config=" << output << " cannot be written!" << std::endl;
      outputStream.close();
    }
    else
      std::cout << "INFO! config=" << output << " not modified!" << std::endl;
  }
  else
    std::cout << "INFO! config=" << output << " persistence mode disabled!" << std::endl;
}
#endif
