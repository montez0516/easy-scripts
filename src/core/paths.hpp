#ifndef PATHS_H
#define PATHS_H

#include <filesystem>

class Paths
{
private:
  std::filesystem::path root_;
  std::filesystem::path determineRootFolder();

public:
  std::filesystem::path root() const;
  std::filesystem::path bin() const;
  std::filesystem::path runner() const;
  std::filesystem::path python() const;
  std::filesystem::path scripts() const;
  std::filesystem::path logs() const;
};

#endif