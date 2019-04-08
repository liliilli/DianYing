#include <precompiled.h>
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

/// Header file
#include <Script/PlanetScript.h>
#include <Dy/Management/MInput.h>
#include <Dy/Element/FActor.h>
#include <Dy/Management/MWorld.h>

#include <Dy/Core/GEngine.h>
#include <Dy/Component/CCamera.h>
#include <Dy/Component/CSoundSource.h>
#include <Dy/Component/CTransform.h>
#include <Math/Utility/XLinearMath.h>

#include <execution>
#include <Math/Utility/XRandom.h>

std::array<dy::DVec3, 4> PlanetScript::mRotations;

void PlanetScript::Initiate() 
{
  using namespace dy;

  for (auto& rotation : mRotations)
  {
    rotation.X = math::RandomUniformReal(-15.f, 15.f);
    rotation.Y = math::RandomUniformReal(-15.f, 15.f);
    rotation.Z = math::RandomUniformReal(-15.f, 15.f);
  }
}

void PlanetScript::Start()
{
  auto ptrTransform = this->GetActorReference().GetTransform();
  ptrTransform->SetWorldEulerAngle(dy::DVec3{30, 0, 15});

  using namespace dy;
  auto ptr = this->GetActorReference().GetGeneralComponent<CSoundSource>();
  if (ptr.has_value() == true)
  {
    this->mPtrSoundSource = ptr.value();
    this->mPtrSoundSource->SetNewSound("Back_Astronaut");
    this->mPtrSoundSource->Set2D(true);
    this->mPtrSoundSource->SetLoop(true);
  }
     
  for (size_t i = 0; i < 1500; ++i)
  {
    DTransform transform;
    const auto xz = math::RandomVector2Range<TReal>(math::ERandomPolicy::Uniform, 200, 300);
    transform.mWorldRelPosition.X = xz.X;
    transform.mWorldRelPosition.Z = xz.Y;
    transform.mWorldRelPosition.Y = math::RandomUniformReal(-15.f, 15.f);

    transform.mLocalEulerAngle.X = math::RandomUniformReal<TReal>(0, 180);
    transform.mLocalEulerAngle.Y = math::RandomUniformReal<TReal>(0, 180);
    transform.mLocalEulerAngle.Z = math::RandomUniformReal<TReal>(0, 180);

    transform.mLocalScale.X = math::RandomUniformReal(0.01f, 0.04f);
    transform.mLocalScale.Y = transform.mLocalScale.X;
    transform.mLocalScale.Z = transform.mLocalScale.X;

    this->mAstroidBinders.emplace_back(
      std::pair(
        MWorld::GetInstance().CreateActor("Astroid", "PB_Rock", transform, &this->GetActorReference()),
        math::RandomUniformInteger(0, 3)
      )
    );
  }

  for (size_t i = 0; i < 500; ++i)
  {
    DTransform transform;
    const auto basis = math::RandomVector2Length<TReal>(1);
    const auto range = 
        math::RandomVector2Length<TReal>(1) 
      * math::RandomVector2Range<TReal>(math::ERandomPolicy::Uniform, 50, 75);

    transform.mWorldRelPosition.X = basis.X * 250.f;
    transform.mWorldRelPosition.Z = basis.Y * 250.f;

    {
      const auto x = DVec3{basis.X * -1.f, 0, basis.Y * -1.f};
      const auto y = DVec3::UnitY();
      const auto z = Cross(y, x);

      const auto r = DVec3{0, range.X, range.Y};
      const auto result = DVec3{Dot(x, r), Dot(y, r), Dot(z, r)};

      transform.mWorldRelPosition += result;
    }

    transform.mLocalEulerAngle.X = math::RandomUniformReal<TReal>(0, 180);
    transform.mLocalEulerAngle.Y = math::RandomUniformReal<TReal>(0, 180);
    transform.mLocalEulerAngle.Z = math::RandomUniformReal<TReal>(0, 180);

    transform.mLocalScale.X = math::RandomUniformReal(0.2f, 0.6f);
    transform.mLocalScale.Y = transform.mLocalScale.X;
    transform.mLocalScale.Z = transform.mLocalScale.X;

    this->mCrystalBinders.emplace_back(
      std::pair(
        MWorld::GetInstance().CreateActor("Crystal", "PB_Crystal", transform, &this->GetActorReference()),
        math::RandomUniformInteger(0, 3)
      )
    );
  }
}

void PlanetScript::Update(TF32 dt)
{
  using namespace dy;

  if (this->mPtrSoundSource != nullptr)
  {
    static bool isPlayed = false;
    if (this->mPtrSoundSource->IsPlayable() == true && isPlayed == false) 
    { 
      this->mPtrSoundSource->PlaySound(); 
      isPlayed = true;
    }
  }

  auto ptrTransform = this->GetActorReference().GetTransform();
  ptrTransform->AddWorldEulerAngle(DVec3{0, 5 * dt, 0});

  std::for_each(
    std::execution::par, 
    MDY_BIND_BEGIN_END(this->mAstroidBinders), 
    [dt](std::pair<DActorBinder, TU32>& iPair)
    {
      auto& [binder, index] = iPair;
      if (binder.IsActorCreated() == true)
      {
        auto transform = binder->GetTransform();
        transform->AddLocalEulerAngle(mRotations[index] * dt);
      }
    }
  );
}
