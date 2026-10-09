#pragma once

#include <filesystem>
#include <string>

class Paths
{
private:
  static std::filesystem::path root_;

public:
  Paths();
  static std::filesystem::path root();
  static std::filesystem::path bin();
  static std::filesystem::path runner();
  static std::filesystem::path python();
  static std::filesystem::path scripts();
  static std::filesystem::path sdk();
  static std::filesystem::path resources();
  static std::filesystem::path icons();
  static std::filesystem::path themes();
};
