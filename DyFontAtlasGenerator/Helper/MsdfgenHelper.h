#ifndef GUARD_DY_MSDFGENHELPER_H
#define GUARD_DY_MSDFGENHELPER_H

#undef INFINITE
#undef max
#undef min
#include <freetype/freetype.h>
#include <msdfgen.h>

namespace dy
{

///
/// @struct DDyFreeTypeContext
/// @brief
///
struct DDyFreeTypeContext
{
  msdfgen::Point2   mPosition;
  msdfgen::Shape*   mShape;
  msdfgen::Contour* mContour;
};

static msdfgen::Point2 ftPoint2(const FT_Vector& vector)
{
  return msdfgen::Point2(vector.x / 64., vector.y / 64.);
}

static int ftMoveTo(const FT_Vector* to, void* user)
{
  DDyFreeTypeContext *context  = reinterpret_cast<DDyFreeTypeContext*>(user);
  context->mContour   = &context->mShape->addContour();
  context->mPosition  = ftPoint2(*to);
  return 0;
}

static int ftLineTo(const FT_Vector* to, void* user)
{
  DDyFreeTypeContext *context = reinterpret_cast<DDyFreeTypeContext*>(user);
  context->mContour->addEdge(new msdfgen::LinearSegment(context->mPosition, ftPoint2(*to)));
  context->mPosition = ftPoint2(*to);
  return 0;
}

static int ftConicTo(const FT_Vector* control, const FT_Vector* to, void* user)
{
  DDyFreeTypeContext *context = reinterpret_cast<DDyFreeTypeContext*>(user);
  context->mContour->addEdge(new msdfgen::QuadraticSegment(context->mPosition, ftPoint2(*control), ftPoint2(*to)));
  context->mPosition = ftPoint2(*to);
  return 0;
}

static int ftCubicTo(const FT_Vector* control1, const FT_Vector* control2, const FT_Vector* to, void* user)
{
  DDyFreeTypeContext *context = reinterpret_cast<DDyFreeTypeContext*>(user);
  context->mContour->addEdge(new msdfgen::CubicSegment(context->mPosition, ftPoint2(*control1), ftPoint2(*control2), ftPoint2(*to)));
  context->mPosition = ftPoint2(*to);
  return 0;
}

} /// ::dy namespace

#endif /// GUARD_DY_MSDFGENHELPER_H