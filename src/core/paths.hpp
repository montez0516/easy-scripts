#ifndef PATHS_H
#define PATHS_H

#include <filesystem>
#include <string>

class Paths
{
private:
  std::filesystem::path root_;

public:
  Paths();
  std::filesystem::path root() const;
  std::filesystem::path bin() const;
  std::filesystem::path runner() const;
  std::filesystem::path python() const;
  std::filesystem::path scripts() const;
};

#endif