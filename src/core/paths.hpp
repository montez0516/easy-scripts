#pragma once

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
  std::filesystem::path sdk() const;
  std::filesystem::path resources() const;
  std::filesystem::path icons() const;
};
