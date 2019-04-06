///
/// MIT License
/// Copyright (c) 2018-2019 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

// DyHexToUChar.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <filesystem>

#include <Expr/TEnumString.h>

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable:4267)
#pragma warning(disable:4996)
#endif

#include <cxxopts.hpp>

EXPR_DEFINE_ENUM(EMode,
  HexToUChar,
  UCharToHex
);

int main(int argc, char* argv[])
{
  // Set command line arguments option setting.
  cxxopts::Options options("DyHextoUChar", "CUI tool for conversion between Hex and UChar");
  options.add_options()
    ("h,hex_to_uchar", 
      "Convert hex chunk to uchar chunk.", 
      cxxopts::value<std::string>()->default_value(""))
    ("u,uchar_to_hex",
      "Convert uchar chunk to hex chunk.",
      cxxopts::value<std::string>()->default_value(""))
    ("o,output_path",
      "Output path with output file name.",
      cxxopts::value<std::string>()->default_value(""))
    ("v,verbose",
      "Log verbosely",
      cxxopts::value<bool>());

  const auto argsResult = options.parse(argc, argv);
  using namespace std::filesystem;

  EXPR_E(EMode) mode = EMode::__Error;
  path sourcePath, outputPath;

  // Get source path and mode.
  if (auto path = argsResult["hex_to_uchar"].as<std::string>();
      path.empty() == false)
  {
    mode = EMode::HexToUChar;
    sourcePath = path;
  }
  else 
    if (path = argsResult["uchar_to_hex"].as<std::string>();
        path.empty() == false)
  {
    mode = EMode::UCharToHex;
    sourcePath = path;
  }

  // Get output path (optional)
  if (const auto output = argsResult["output_path"].as<std::string>();
      output.empty() == false)
  {
    outputPath = output;
  }

  // Get verbosity (optional)
  const auto isEnabledVerbose = argsResult["verbose"].as<bool>();

  // Check error.
  if (mode == EMode::__Error)
  {
    std::cerr << "\n";
    std::cerr << "Failed to execute tool.\n";
    std::cerr << R"dy(Usage : ./DyHexToUChar.exe ([-h "source_file"] | [-u "source_file"]) [-o "output_file"] [-v])dy" << "\n";
    std::cerr << "\n";
    std::cerr << R"dy(  -h "source_file" : Convert hex chunk to uchar chunk from "source_file".)dy" << "\n";
    std::cerr << R"dy(  -u "source_file" : Convert uchar chunk to hex chunk from "source_file".)dy" << "\n";
    std::cerr << R"dy(  -o "output_file" : Create converted file as "output_file" path. This is optional.)dy" << "\n";
    std::cerr << R"dy(  -v : Log verbosely. This is optional.)dy" << "\n";
    std::cerr << "\n";
    return 0;
  }

  // Check file is not exist error.
  if (exists(sourcePath) == false)
  {
    std::cerr << "\n";
    std::cerr << "Failed to execute tool.\n";
    std::cerr << "Source file path is not exist. : " << sourcePath.string() << "\n";
    std::cerr << "\n";
    return 0;
  }

  // Check sourcePath has not filename.
  if (sourcePath.has_filename() == false)
  {
    std::cerr << "\n";
    std::cerr << "Failed to execute tool.\n";
    std::cerr << "Source file path is not a file path. : " << sourcePath.string() << "\n";
    std::cerr << "\n";
    return 0 ;
  }

  // Make default output path when output path is not specified explictly.
  if (outputPath.empty() == true)
  {
    const auto rootPath = sourcePath.root_path();
    const auto fileName = sourcePath.filename();

    outputPath = rootPath.string() + "out_" + fileName.string();
  }

  // Output information when `isEnabledVerbose` is enabled.
  if (isEnabledVerbose == true)
  {
    std::cout << "\n";
    std::cout << "Mode : " << EMode::ToString(mode) << "\n";
    std::cout << "Source Path : " << sourcePath << "\n";
    std::cout << "Output Path : " << outputPath << "\n";
    std::cout << "\n";
  }

  // Check source file can be parsed.
  FILE* fd = std::fopen(sourcePath.string().c_str(), "r");
  if (fd == nullptr)
  {
    std::cerr << "\n";
    std::cerr << "Failed to open source file. : " << sourcePath << "\n";
    std::cerr << "Failed to execute tool.\n";
    std::cerr << "\n";
    return 0;
  }

  // Get input file size to check EOF in process. 
  std::fseek(fd, 0, SEEK_END);
  const auto endCountOfFd = std::ftell(fd);
  std::fseek(fd, 0, SEEK_SET);
  
  switch (mode)
  {
  case EMode::HexToUChar: 
  {
    // Open output file and check error.
    FILE* fdOut = std::fopen(outputPath.string().c_str(), "w");
    if (fdOut == nullptr)
    {
      std::cerr << "\n";
      std::cerr << "Failed to write to output file. : " << outputPath << "\n";
      std::cerr << "Failed to execute tool.\n";
      std::cerr << "\n";

      std::fclose(fd);
      return 0;
    }

    // Do Conversion 
    unsigned value; 
    while (std::fscanf(fd, "%2x", &value) == 1)
    {
      // Write conversion result into output file descriptor.
      if (std::ftell(fd) != endCountOfFd)
      {
        std::fprintf(fdOut, "%u,", value);
        std::fseek(fd, 1, SEEK_CUR);
      }
      else
      {
        std::fprintf(fdOut, "%u", value);
      }

      // If verbosity is enabled, let it output conversion log.
      if (isEnabledVerbose == true)
      {
        static unsigned count = 0;
        count++;
        std::printf("{%2X => %3u} ", value, value);
        if (count == 8) { std::printf("\n"); count = 0; }
      }
    }

    // Close output file descriptor.
    fclose(fdOut);
  } break;
  case EMode::UCharToHex: 
  {
    // Open output file and check error.
    FILE* fdOut = std::fopen(outputPath.string().c_str(), "w");
    if (fdOut == nullptr)
    {
      std::cerr << "\n";
      std::cerr << "Failed to write to output file. : " << outputPath << "\n";
      std::cerr << "Failed to execute tool.\n";
      std::cerr << "\n";

      std::fclose(fd);
      return 0;
    }

    // Do Conversion 
    unsigned value; 
    while (std::fscanf(fd, "%u", &value) == 1)
    {
      // Write conversion result into output file descriptor.
      if (std::ftell(fd) != endCountOfFd)
      {
        std::fprintf(fdOut, "%02X,", value);
        std::fseek(fd, 1, SEEK_CUR);
      }
      else
      {
        std::fprintf(fdOut, "%02X", value);
      }

      // If verbosity is enabled, let it output conversion log.
      if (isEnabledVerbose == true)
      {
        static unsigned count = 0;
        count++;
        std::printf("{%3u => %02X} ", value, value);
        if (count == 8) { std::printf("\n"); count = 0; }
      }
    }

    // Close output file descriptor.
    fclose(fdOut);
  } break;
  default: break;
  }

  // Close input file descriptor.
  std::fclose(fd);

  // Output result
  std::cout << "\n";
  if (isEnabledVerbose == true)
  {
    std::cout << "Mode : " << EMode::ToString(mode) << "\n";
  }
  std::cout << "Conversion is done. : " << outputPath << "\n";
  std::cout << "\n";
  return 0;
}

#ifdef _WIN32
#pragma warning(pop)
#endif
