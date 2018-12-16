
#ifdef false
///
/// @brief
///
void DyGlTempInitializeResource()
{
  //auto& manInfo = dy::MDyIOData::GetInstance();

  //!
  //! Shader
  //!

  {
    auto animAsyncTask = std::async(std::launch::async, [&manInfo] {
      dy::PDyModelConstructionDescriptor modelDesc;
      {
        modelDesc.mModelName = "TestModel";
        modelDesc.mModelPath = "./TestResource/bun_zipper.ply";
      }
      MDY_CALL_ASSERT_SUCCESS(manInfo.CreateModelInformation(modelDesc));
      return true;
    });

    if (animAsyncTask.get()) { MDY_LOG_DEBUG_D("OK"); };
  }
#endif

#ifdef false
  {
    dy::PDyShaderConstructionDescriptor desc;
    desc.mShaderName = "TestSkeletalAnimShader";
    {
      dy::PDyShaderFragmentInformation vertexShaderInfo;
      vertexShaderInfo.mShaderType = dy::EDyShaderFragmentType::Vertex;
      vertexShaderInfo.mShaderPath = "./glMeshVertSAnim.vert";
      desc.mShaderFragments.emplace_back(vertexShaderInfo);
    }
    {
      dy::PDyShaderFragmentInformation fragmentShaderInfo;
      fragmentShaderInfo.mShaderType = dy::EDyShaderFragmentType::Pixel;
      fragmentShaderInfo.mShaderPath = "./glShader.frag";
      desc.mShaderFragments.emplace_back(fragmentShaderInfo);
    }
    MDY_CALL_ASSERT_SUCCESS(manInfo.CreateShaderInformation(desc));
  }

  {
    shaderDesc.mShaderFragments.clear();
    shaderDesc.mShaderName = "MeshNanosuitShader";
    {
      dy::PDyShaderFragmentInformation vertexShaderInfo;
      vertexShaderInfo.mShaderType = dy::EDyShaderFragmentType::Vertex;
      vertexShaderInfo.mShaderPath = "./glMeshVert.vert";
      shaderDesc.mShaderFragments.emplace_back(vertexShaderInfo);
    }
    {
      dy::PDyShaderFragmentInformation fragmentShaderInfo;
      fragmentShaderInfo.mShaderType = dy::EDyShaderFragmentType::Pixel;
      fragmentShaderInfo.mShaderPath = "./glMeshDNSFrag.frag";
      shaderDesc.mShaderFragments.emplace_back(fragmentShaderInfo);
    }
  }
  MDY_CALL_ASSERT_SUCCESS(manInfo.CreateShaderInformation(shaderDesc));

  //!
  //! Model
  //!

  auto tempAsyncTask = std::async(std::launch::async, [&manInfo] {
    dy::PDyModelConstructionDescriptor bunnyModel;
    {
      bunnyModel.mModelName = "Bunny";
      bunnyModel.mModelPath = "./TestResource/bun_zipper.ply";
    }
    MDY_CALL_ASSERT_SUCCESS(manInfo.CreateModelInformation(bunnyModel));
    return true;
  });

  auto modelAsyncTask = std::async(std::launch::async, [&manInfo] {
    dy::PDyModelConstructionDescriptor modelDesc;
    {
      modelDesc.mModelName = "TestModel";
      modelDesc.mModelPath = "./TestResource/nanosuit/nanosuit.obj";
    }
    MDY_CALL_ASSERT_SUCCESS(manInfo.CreateModelInformation(modelDesc));
    return true;
  });
#endif

#ifdef false
  {
    dy::PDyModelConstructionDescriptor modelDesc;
    {
      modelDesc.mModelName = "Sponza";
      modelDesc.mModelPath = "./TestResource/crytek-sponza/sponza.obj";
    }
    MDY_CALL_ASSERT_SUCCESS(manInfo.CreateModelInformation(modelDesc));
  };

  std::unordered_map<std::string, std::string> populatedMaterialList = {};
  dy::PDyMaterialPopulateDescriptor popDesc;
  {
    popDesc.mOverrideShaderName       = "MeshNanosuitShader";
    popDesc.mTextureOverrideNames     = {"glass_dif", "glass_ddn"};
    popDesc.mIsEnabledTextureOverride = true;
    popDesc.mIsEnabledShaderOverride  = true;
    if (auto matPtr = manInfo.PopulateMaterialInformation("Glass", popDesc); !matPtr.has_value()) { return; }
    else { populatedMaterialList.try_emplace("Glass", matPtr.value()); }
  }
  {
    popDesc.mTextureOverrideNames     = {"leg_dif", "leg_showroom_ddn", "leg_showroom_spec"};
    if (auto matPtr = manInfo.PopulateMaterialInformation("Leg", popDesc); !matPtr.has_value()) { return; }
    else { populatedMaterialList.try_emplace("Leg", matPtr.value()); }
  }
  {
    popDesc.mTextureOverrideNames     = {"hand_dif", "hand_showroom_ddn", "hand_showroom_spec"};
    if (auto matPtr = manInfo.PopulateMaterialInformation("Hand", popDesc); !matPtr.has_value()) { return; }
    else { populatedMaterialList.try_emplace("Hand", matPtr.value()); }
  }
  {
    popDesc.mTextureOverrideNames     = {"arm_dif", "arm_showroom_ddn", "arm_showroom_spec"};
    if (auto matPtr = manInfo.PopulateMaterialInformation("Arm", popDesc); !matPtr.has_value()) { return; }
    else { populatedMaterialList.try_emplace("Arm", matPtr.value()); }
  }
  {
    popDesc.mTextureOverrideNames     = {"helmet_dif", "helmet_showroom_ddn", "helmet_showroom_spec"};
    if (auto matPtr = manInfo.PopulateMaterialInformation("Helmet", popDesc); !matPtr.has_value()) { return; }
    else { populatedMaterialList.try_emplace("Helmet", matPtr.value()); }
  }
  {
    popDesc.mTextureOverrideNames     = {"body_dif", "body_showroom_ddn", "body_showroom_spec"};
    if (auto matPtr = manInfo.PopulateMaterialInformation("Body", popDesc); !matPtr.has_value()) { return; }
    else { populatedMaterialList.try_emplace("Body", matPtr.value()); }
  }

  dy::PDyRendererConsturctionDescriptor rendererDesc;
  {
    rendererDesc.mModelName     = "TestModel";
    rendererDesc.mMaterialNames = {
      populatedMaterialList["Glass"],
      populatedMaterialList["Leg"],
      populatedMaterialList["Hand"],
      populatedMaterialList["Glass"],
      populatedMaterialList["Arm"],
      populatedMaterialList["Helmet"],
      populatedMaterialList["Body"]
    };
  }
  MDY_CALL_ASSERT_SUCCESS(gRenderer.pfInitialize(rendererDesc));
#endif

#ifdef false
  {
    shaderDesc.mShaderFragments.clear();
    shaderDesc.mShaderName = "MeshShader";
    {
      dy::PDyShaderFragmentInformation vertexShaderInfo;
      vertexShaderInfo.mShaderType = dy::EDyShaderFragmentType::Vertex;
      vertexShaderInfo.mShaderPath = "./glMeshVert.vert";
      shaderDesc.mShaderFragments.emplace_back(vertexShaderInfo);
    }
    {
      dy::PDyShaderFragmentInformation fragmentShaderInfo;
      fragmentShaderInfo.mShaderType = dy::EDyShaderFragmentType::Pixel;
      fragmentShaderInfo.mShaderPath = "./glMeshFrag.frag";
      shaderDesc.mShaderFragments.emplace_back(fragmentShaderInfo);
    }
  }
  MDY_CALL_ASSERT_SUCCESS(manInfo.CreateShaderInformation(shaderDesc));

  std::vector<std::string> materialNameList = {};
  dy::PDyMaterialPopulateDescriptor popDesc;
  {
    popDesc.mOverrideShaderName       = "MeshShader";
    popDesc.mIsEnabledShaderOverride  = true;
    popDesc.mIsEnabledMaterialCustomNameOverride = true;

    popDesc.mMaterialOverrideName     = "MaterialDennum1";
    if (auto matPtr = manInfo.PopulateMaterialInformation("Material__2385", popDesc); !matPtr.has_value()) { return; }
    else { materialNameList.emplace_back(matPtr.value()); }
  }
  {
    popDesc.mMaterialOverrideName     = "MaterialCap";
    if (auto matPtr = manInfo.PopulateMaterialInformation("Material__2412", popDesc); !matPtr.has_value()) { return; }
    else { materialNameList.emplace_back(matPtr.value()); }
  }
  {
    popDesc.mMaterialOverrideName     = "MaterialFace";
    if (auto matPtr = manInfo.PopulateMaterialInformation("Material__2386", popDesc); !matPtr.has_value()) { return; }
    else { materialNameList.emplace_back(matPtr.value()); }
  }
  {
    popDesc.mMaterialOverrideName     = "MaterialDennum3";
    if (auto matPtr = manInfo.PopulateMaterialInformation("Material__2410", popDesc); !matPtr.has_value()) { return; }
    else { materialNameList.emplace_back(matPtr.value()); }
  }
  {
    popDesc.mMaterialOverrideName     = "MaterialDennum2";
    if (auto matPtr = manInfo.PopulateMaterialInformation("Material__2387", popDesc); !matPtr.has_value()) { return; }
    else { materialNameList.emplace_back(matPtr.value()); }
  }
  {
    popDesc.mMaterialOverrideName     = "MaterialShoes";
    if (auto matPtr = manInfo.PopulateMaterialInformation("Material__2413", popDesc); !matPtr.has_value()) { return; }
    else { materialNameList.emplace_back(matPtr.value()); }
  }
#endif
#ifdef false
  {
    dy::PDyMaterialPopulateDescriptor popDesc;
    popDesc.mIsEnabledShaderOverride  = true;
    popDesc.mOverrideShaderName       = "TestDeferredShader";
    const auto spawnedMatStr = manInfo.PopulateMaterialInformation("asdf1:Beta_HighLimbsGeoSG2", popDesc);

    dy::PDyRendererConsturctionDescriptor rendererDesc;
    {
      rendererDesc.mModelName     = "Boxing";
      rendererDesc.mMaterialNames = { spawnedMatStr.value(), spawnedMatStr.value() };
    }
    MDY_CALL_ASSERT_SUCCESS(gRenderer.pfInitialize(rendererDesc));
  }

  dy::PDyRendererConsturctionDescriptor rendererDesc;
  {
    rendererDesc.mModelName     = "TestModel";
    rendererDesc.mMaterialNames = std::vector<std::string>(394, "TestMat");
  }
  MDY_CALL_ASSERT_SUCCESS(gRenderer.pfInitialize(rendererDesc));
}

///
/// @brief
///
void DyTestSoundFmod()
{
  dy::PDySoundConstructionDescriptor desc;
  desc.mSoundName = "1";
  desc.mSoundPath = "./TestResource/_02Effect.mp3";

  auto& manInfo = dy::MDyIOData::GetInstance();
  auto& resInfo = dy::MDyIOResource::GetInstance();
  MDY_CALL_ASSERT_SUCCESS(manInfo.CreateSoundInformation(desc));
  MDY_CALL_ASSERT_SUCCESS(resInfo.CreateSoundResource(desc.mSoundName));
}
#endif

} /// unnamed namespace

