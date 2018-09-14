#ifndef GUARD_DY_LIBRARY_LICENSE_STRING_H
#define GUARD_DY_LIBRARY_LICENSE_STRING_H
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///
#if defined(MDY_FLAG_IN_EDITOR)

#include <array>
#include <string_view>
#include <Dy/Helper/GlobalType.h>

namespace dy::editor
{

///
/// @class DDyLibraryLicenseStringInfo
/// @brief fff
///
class DDyLibraryLicenseStringInfo final
{
public:
  constexpr DDyLibraryLicenseStringInfo(
      std::string_view libraryName,
      std::string_view versionName,
      std::string_view licenseStatement,
      std::string_view githubLink,
      std::string_view homepageLink) noexcept :
      mLibraryName(libraryName),
      mVersionName(versionName),
      mLicenseStatement(licenseStatement),
      mGithubLink(githubLink),
      mHomePageLink(homepageLink)
  {}

  [[nodiscard]] constexpr const char* GetLibraryName() const noexcept
  {
    return this->mLibraryName.data();
  }

  [[nodiscard]] constexpr const char* GetVersionName() const noexcept
  {
    return this->mVersionName.data();
  }

  [[nodiscard]] constexpr const char* GetLicenseStatement() const noexcept
  {
    return this->mLicenseStatement.data();
  }

  [[nodiscard]] constexpr bool IsGithubUrlEmpty() const noexcept
  {
    return this->mGithubLink.empty();
  }

  [[nodiscard]] constexpr bool IsHomepageUrlEmpty() const noexcept
  {
    return this->mHomePageLink.empty();
  }

#if defined(_WIN32)
  /// @reference https://stackoverflow.com/questions/8032080/how-to-convert-char-to-wchar-t
  [[nodiscard]] std::wstring GetGithubUrl() const noexcept
  {
    wchar_t textWchar[256] = {0,};
    size_t len = this->mGithubLink.size();
    mbstowcs_s(&len, textWchar, this->mGithubLink.data(), this->mGithubLink.size());

    return textWchar;
  }

  [[nodiscard]] std::wstring GetHomepageUrl() const noexcept
  {
    wchar_t textWchar[256] = {0,};
    size_t len = this->mHomePageLink.size();
    mbstowcs_s(&len, textWchar, this->mHomePageLink.data(), this->mHomePageLink.size());

    return textWchar;
  }
#else
  [[nodiscard]] const char* GetHomepageUrl() const noexcept
  {
    return this->mHomePageLink.data();
  }

  [[nodiscard]] const char* GetGithubUrl() const noexcept
  {
    return this->mGithubLink.data();
  }
#endif

private:
  const std::string_view mLibraryName;
  const std::string_view mVersionName;
  const std::string_view mLicenseStatement;
  const std::string_view mGithubLink;
  const std::string_view mHomePageLink;
};

constexpr TU32 __ddylibrarylicensestringinfoCount = 11;
constexpr std::array<DDyLibraryLicenseStringInfo, __ddylibrarylicensestringinfoCount> kLibraryLicenseInfo =
{
  DDyLibraryLicenseStringInfo\
  /// Assimp
  { R"dy(Assimp)dy", R"dy(4.1.0)dy",
    R"dy(Assimp is released as Open Source under the terms of a 3-clause BSD license.
An informal summary is: do whatever you want, but include Assimp's license text with your product - and don't sue us if our code doesn't work. Note that, unlike LGPLed code, you may link statically to Assimp. For the legal details, see the LICENSE file.)dy",
    R"dy(https://github.com/assimp/assimp)dy",
    R"dy(http://assimp.sourceforge.net/)dy"},
  /// bullet

  /// glad
  { R"dy(glad)dy", R"dy(0.1.27 on Tue Aug 28 10:32:57 2018)dy",
    R"dy(The MIT License (MIT)

Copyright (c) 2013-2018 David Herberth

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.)dy",
    R"dy(https://github.com/Dav1dde/glad/)dy",
    R"dy(https://glad.dav1d.de/)dy"},
  /// glfw
  { R"dy(glfw)dy",
    R"dy(3.2)dy",
    R"dy(GLFW is licensed under the zlib/libpng license, a BSD-like license that allows static linking with closed source software. It is reproduced in its entirety below.

Copyright © 2002-2006 Marcus Geelnard
Copyright © 2006-2011 Camilla Berglund

This software is provided ‘as-is’, without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:)dy",
    R"dy(https://github.com/glfw/glfw)dy",
    R"dy(http://www.glfw.org/index.html)dy"
  },
  /// glm
  { R"dy(glm)dy",
    R"dy(0.9.9.0 2018-05-22)dy",
    R"dy(The source code and the documentation, including this manual, are licensed under the Happy Bunny License (Modified MIT) or the MIT License.)dy",
    R"dy(https://github.com/g-truc/glm)dy",
    R"dy(https://glm.g-truc.net/)dy",
  },
  /// imgui
  { R"dy(Dear ImGui)dy",
    R"dy(1.6.2)dy",
    R"dy(The MIT License (MIT)

Copyright (c) 2014-2018 Omar Cornut

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:)dy",
    R"dy(https://github.com/ocornut/imgui)dy",
    R"dy()dy",
  },
  /// lua
  { R"dy(Lua)dy",
    R"dy(5.3.5)dy",
    R"dy(Copyright © 1994–2018 Lua.org, PUC-Rio.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:)dy",
    R"dy()dy",
    R"dy(https://www.lua.org/home.html)dy"
  },
  /// nlohmann
  { R"dy(JSON for Modern C++)dy",
    R"dy(3.2.0)dy",
    R"dy(MIT License

Copyright (c) 2013-2018 Niels Lohmann

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.)dy",
    R"dy(https://github.com/nlohmann/json)dy",
    R"dy(https://nlohmann.github.io/json/)dy",
  },
  /// phitos
  { R"dy(Phitos)dy",
    R"dy(0.0.1)dy",
    R"dy(BSD 2-clause)dy",
    R"dy()dy",
    R"dy()dy",
  },
  /// sol2
  { R"dy(sol2)dy",
    R"dy(v2.20 bugfix)dy",
    R"dy(The MIT License (MIT)

Copyright (c) 2013-2018 Rapptz, ThePhD, and contributors

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.)dy",
    R"dy(https://github.com/ThePhD/sol2)dy",
    R"dy()dy",
  },
  /// spdlog
  { R"dy(spdlog)dy",
    R"dy(1.1.0)dy",
    R"dy(The MIT License (MIT)

Copyright (c) 2016 Gabi Melman.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.)dy",
    R"dy(https://github.com/gabime/spdlog)dy",
    R"dy()dy"
  },
  /// stb
  { R"dy(stb_image)dy",
    R"dy(2.19)dy",
    R"dy(Public domain)dy",
    R"dy(https://github.com/nothings/stb)dy",
    R"dy()dy"
  }
};

} /// ::dy::editor namespace

#endif
#endif /// GUARD_DY_LIBRARY_LICENSE_STRING_H