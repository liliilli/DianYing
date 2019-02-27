#ifndef GUARD_DY_JSONTYPEWRITERHELPER_H
#define GUARD_DY_JSONTYPEWRITERHELPER_H
#include <nlohmann/json.hpp>

namespace dy
{

struct DMeta final
{
  std::string mFontSpecifierName;
  std::string mFontStyleSpecifier;
  int32_t     mHoriLinefeed;
};

inline void to_json(nlohmann::json& j, const DMeta& p)
{
  j = nlohmann::json
  {
      {"FontSpecifierName",   p.mFontSpecifierName},
      {"FontStyleSpecifier",  p.mFontStyleSpecifier},
      {"HoriLinefeed",        p.mHoriLinefeed}
  };
}

inline void from_json(const nlohmann::json& j, DMeta& p)
{
  p.mFontSpecifierName  = j.at("FontSpecifierName").get<std::string>();
  p.mFontStyleSpecifier = j.at("FontStyleSpecifier").get<std::string>();
  p.mHoriLinefeed       = j.at("HoriLinefeed").get<int32_t>();
}

} /// ::dy namespace

#endif /// GUARD_DY_JSONTYPEWRITERHELPER_H