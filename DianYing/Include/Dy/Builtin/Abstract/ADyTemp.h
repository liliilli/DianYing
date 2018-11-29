#pragma once
#include <Dy/Builtin/Interface/IDyResource.h>
#include <Dy/Core/Reflection/RDyBuiltinResources.h>

namespace dy
{
struct IGLShaderResource : public IDyResource { static constexpr auto value = EDyResourceType::GLShader; };
struct ITextureResource : public IDyResource { static constexpr auto value = EDyResourceType::Texture; };

} /// ::dy namespace