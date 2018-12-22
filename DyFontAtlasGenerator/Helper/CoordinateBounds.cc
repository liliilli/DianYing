#include "precompiled.h"
/// Header file
#include "CoordinateBounds.h"

namespace dy
{

DDyCoordinateBounds CreateCoordinateInformation(
    const int32_t canvasWidth,
    const int32_t canvasHeight,
    const int32_t textureWidth,
    const int32_t textureHeight,
    const int32_t id)
{
  Q_ASSERT(canvasWidth > 0);
  Q_ASSERT(canvasHeight > 0);
  Q_ASSERT(textureWidth > 0);
  Q_ASSERT(textureHeight > 0);

  static constexpr auto CHANNEL_LIMIT { 4 };

  const auto coordWidth           {static_cast<float>(textureWidth) / canvasWidth};
  const auto coordHeight          {static_cast<float>(textureHeight) / canvasHeight};
  const auto canvasHoriMaxCount   {static_cast<int32_t>(canvasWidth / textureWidth)};
  const auto canvasVertMaxCount   {static_cast<int32_t>(canvasHeight / textureHeight)};
  const auto localMapTextureCount { canvasVertMaxCount * canvasHoriMaxCount * CHANNEL_LIMIT};

  auto localId        {id % localMapTextureCount};
  const auto mapId    {id / localMapTextureCount};

  const auto channel  {localId % CHANNEL_LIMIT};
  localId /= CHANNEL_LIMIT;

  const auto YId {canvasHoriMaxCount - (localId / canvasHoriMaxCount) - 1};
  const auto XId {localId % canvasHoriMaxCount};

  auto resultInstance { DDyCoordinateBounds{} };
  resultInstance.mLeftDown = DDyVector2{coordWidth * XId      , coordHeight * YId};
  resultInstance.mRightUp  = DDyVector2{coordWidth * (XId + 1), coordHeight * (YId + 1)};
  resultInstance.mMapIndex = mapId;
  resultInstance.mChannel  = channel;

  return resultInstance;
}

} /// ::dy namespace