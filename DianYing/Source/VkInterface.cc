#include <precompiled.h>
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

/// Header file
#include <Dy/VkInterface.h>

#include <iostream>
#include <fstream>
#include <set>
#include <sstream>

#include <Dy/Helper/macroes.h>

#undef max
#undef min

namespace dy
{

const char* validationLayerNames[] = {
  "VK_LAYER_LUNARG_standard_validation"
};

const std::vector<const char*> vulkanDeviceExtensions = {
  VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

VkDebugReportCallbackEXT    vulkanMsgCallback;
VkPhysicalDevice            vkPhysicalDevice;
VkPhysicalDeviceProperties  vkPhysicalDeviceProperties;
VkPhysicalDeviceFeatures    vkPhysicalDeviceFeatures;
VkPhysicalDeviceMemoryProperties vkPhysicalDeviceMemoryProperties;
VkDevice        vkLogicalDevice;
VkInstance      vkInstance    = nullptr;
VkSurfaceKHR    vkSurface     = nullptr;
VkSwapchainKHR  vkSwapChain   = nullptr;
VkQueue         vkPresentQueue;
VkQueue         vkGraphicsQueue;
VkFormat        vkSwapChainImageFormat;
VkExtent2D      vkSwapChainExtent;
std::vector<VkImage>          vkSwapChainImages;
std::vector<VkImageView>      vkSwapChainImageViews;
std::vector<VkFramebuffer>    vkSwapChainFrameBuffers;
std::vector<VkCommandBuffer>  vkCommandBuffers;
std::vector<VkSemaphore>      vkImageAvailableSemaphores;
std::vector<VkSemaphore>      vkRenderFinishedSemaphores;
std::vector<VkFence>          vkSynchronizationFences;
VkShaderModule  vkVertShaderModule;
VkShaderModule  vkFragShaderModule;
VkRenderPass    vkRenderPass;
VkPipelineLayout vkPipelineLayout;
VkPipeline      vkPipeline;
VkCommandPool   vkCommandPool;

///
/// @brief
///
void DyVkRenderLoop()
{
#if defined(_WIN32)
  MSG   win32Message;
  bool  win32QuitMessageReceived = false;
  while (!win32QuitMessageReceived) {
    while (PeekMessage(&win32Message, nullptr, 0, 0, PM_REMOVE)) {
      TranslateMessage(&win32Message);
      DispatchMessage(&win32Message);

      if (win32Message.message == WM_QUIT) {
        win32QuitMessageReceived = true;
        bDyWindowCloseSign = true;
        break;
      }
    }

    if (!IsIconic(windowHandle)) {
      DyVkRenderFrame();
    }

    vkDeviceWaitIdle(vkLogicalDevice);
  }
#elif defined(__linux__)

#endif
}

///
/// @brief
///
std::string DyVkGetErrorString(VkResult result_id) {
  switch (result_id) {
    MDY_CASE_RETURN(VK_NOT_READY);
    MDY_CASE_RETURN(VK_TIMEOUT);
    MDY_CASE_RETURN(VK_EVENT_SET);
    MDY_CASE_RETURN(VK_EVENT_RESET);
    MDY_CASE_RETURN(VK_INCOMPLETE);
    MDY_CASE_RETURN(VK_ERROR_OUT_OF_HOST_MEMORY);
    MDY_CASE_RETURN(VK_ERROR_OUT_OF_DEVICE_MEMORY);
    MDY_CASE_RETURN(VK_ERROR_INITIALIZATION_FAILED);
    MDY_CASE_RETURN(VK_ERROR_DEVICE_LOST);
    MDY_CASE_RETURN(VK_ERROR_MEMORY_MAP_FAILED);
    MDY_CASE_RETURN(VK_ERROR_LAYER_NOT_PRESENT);
    MDY_CASE_RETURN(VK_ERROR_EXTENSION_NOT_PRESENT);
    MDY_CASE_RETURN(VK_ERROR_FEATURE_NOT_PRESENT);
    MDY_CASE_RETURN(VK_ERROR_INCOMPATIBLE_DRIVER);
    MDY_CASE_RETURN(VK_ERROR_TOO_MANY_OBJECTS);
    MDY_CASE_RETURN(VK_ERROR_FORMAT_NOT_SUPPORTED);
    MDY_CASE_RETURN(VK_ERROR_FRAGMENTED_POOL);
    MDY_CASE_RETURN(VK_ERROR_SURFACE_LOST_KHR);
    MDY_CASE_RETURN(VK_ERROR_NATIVE_WINDOW_IN_USE_KHR);
    MDY_CASE_RETURN(VK_SUBOPTIMAL_KHR);
    MDY_CASE_RETURN(VK_ERROR_OUT_OF_DATE_KHR);
    MDY_CASE_RETURN(VK_ERROR_INCOMPATIBLE_DISPLAY_KHR);
    MDY_CASE_RETURN(VK_ERROR_VALIDATION_FAILED_EXT);
  default: return MDY_U8("UNKNOWN_ERROR occured");
  }
}

///
/// @brief vulkan validation layer event message callback prototype.
/// This function must not be called independently without binding vulkan validation system.
///
VKAPI_ATTR VkBool32 VKAPI_PTR pVulkanMessageCallback (
    VkDebugReportFlagsEXT      flags,
    VkDebugReportObjectTypeEXT objectType,
    uint64_t                   object,
    size_t                     location,
    int32_t                    messageCode,
    const char*                pLayerPrefix,
    const char*                pMessage,
    void*                      pUserData) {
  std::string logPrefix = "";

  if (MDY_BITMASK_FLAG_TRUE(flags, VK_DEBUG_REPORT_INFORMATION_BIT_EXT)) {
    logPrefix += "INFORMATION:";
  }
  if (MDY_BITMASK_FLAG_TRUE(flags, VK_DEBUG_REPORT_WARNING_BIT_EXT)) {
    logPrefix += "WARNING:";
  }
  if (MDY_BITMASK_FLAG_TRUE(flags, VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)) {
    logPrefix += "PERFORMANCE:";
  }
  if (MDY_BITMASK_FLAG_TRUE(flags, VK_DEBUG_REPORT_ERROR_BIT_EXT)) {
    logPrefix += "ERROR:";
  }
  if (MDY_BITMASK_FLAG_TRUE(flags, VK_DEBUG_REPORT_DEBUG_BIT_EXT)) {
    logPrefix += "DEBUG:";
  }

  // Display message to default output (console / logcat, etc)
  std::stringstream debugLogMessage;
  debugLogMessage << logPrefix << " [" << pLayerPrefix << "] Code " << messageCode << " : " << pMessage;

#if defined(_WIN32)
  if (MDY_BITMASK_FLAG_TRUE(flags, VK_DEBUG_REPORT_ERROR_BIT_EXT)) {
    std::cerr << debugLogMessage.str() << "\n";
  }
  else {
    std::cerr << debugLogMessage.str() << "\n";
  }
#endif
  return VK_FALSE;
}

///
/// @brief
///
[[nodiscard]] EDySuccess DyVkCheckValidationLayerSupport(const char* (*validationLayers), int32_t layerCount) {
  uint32_t availableLayerCount = 0;
  vkEnumerateInstanceLayerProperties(&availableLayerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers{availableLayerCount};
  vkEnumerateInstanceLayerProperties(&availableLayerCount, availableLayers.data());

  for (int32_t i = 0; i < layerCount; ++i ) {
    const char* layerName = validationLayers[i];
    bool isLayerFound = false;

    for (const auto& availableLayerProperties : availableLayers) {
      if (strcmp(layerName, availableLayerProperties.layerName) == 0) {
        isLayerFound = true;
        break;
      }
    }

    if (!isLayerFound) {
      return DY_FAILURE;
    }
  }

  return DY_SUCCESS;
}

#if defined(_WIN32)
void DyVkInitialize(HWND windowHandle, HINSTANCE hInstance)
#endif
{
  const auto resultPair = dy::DyVkCreateVulkanInstance();
  vkInstance = resultPair.first;
  if (resultPair.second != VK_SUCCESS) {
    std::cout << MDY_U8("Failed to create Vulkan instance.\n");
    std::cout << dy::DyVkGetErrorString(resultPair.second) << '\n';
    fflush(stdout);
    exit(1);
  }
  MDY_CHECK_EXECUTE(MDY_VULKAN_FLAG_VALIDATION_SET, dy::DyVkInitializeDebugCallback(vkInstance));
  MDY_CALL_ASSERT_SUCCESS(dy::DyVkCreateSurface(windowHandle, hInstance));
  MDY_CALL_ASSERT_SUCCESS(dy::DyVkFindPhysicsDevices(vkInstance, &vkPhysicalDevice));
  MDY_CALL_ASSERT_SUCCESS(dy::DyVkCreateLogicalDevice());
  MDY_CALL_ASSERT_SUCCESS(dy::DyVkGetDeviceQueues());
  MDY_CALL_ASSERT_SUCCESS(dy::DyVkCreateSwapChain());
  MDY_CALL_ASSERT_SUCCESS(dy::DyVkCreateImageViews());
  MDY_CALL_ASSERT_SUCCESS(dy::DyVkCreateRenderPass());
  MDY_CALL_ASSERT_SUCCESS(dy::DyVkCreateGraphicsPipeLine());
  MDY_CALL_ASSERT_SUCCESS(dy::DyVkCreateFrameBuffers());
  MDY_CALL_ASSERT_SUCCESS(dy::DyVkCreateCommandPool());
  MDY_CALL_ASSERT_SUCCESS(dy::DyVkCreateCommandBuffers());
  MDY_CALL_ASSERT_SUCCESS(dy::DyVkCreateSemaphores());
  MDY_CALL_ASSERT_SUCCESS(dy::DyVkCreateFences());
}

///
/// @brief
///
std::pair<VkInstance, VkResult> DyVkCreateVulkanInstance() {
  VkApplicationInfo appInfo = {};
  appInfo.sType               = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName    = u8"DianYing v0.0.1";
  appInfo.applicationVersion  = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName         = u8"DianYing v0.0.1";
  appInfo.engineVersion       = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion          = VK_API_VERSION_1_0;

  VkInstanceCreateInfo instanceCreateInfo = {};
  instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instanceCreateInfo.pNext = nullptr;
  instanceCreateInfo.flags = 0;
  instanceCreateInfo.pApplicationInfo = &appInfo;

  std::vector<const char*> instanceExtensions = { VK_KHR_SURFACE_EXTENSION_NAME };
  instanceExtensions.emplace_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

  if (!instanceExtensions.empty()) {
    if constexpr (MDY_VULKAN_FLAG_VALIDATION_SET) {
      instanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    }
    instanceCreateInfo.enabledExtensionCount = static_cast<dy::TUint32>(instanceExtensions.size());
    instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();
  }

  if constexpr (MDY_VULKAN_FLAG_VALIDATION_SET) {
    if (DyVkCheckValidationLayerSupport(validationLayerNames, 1) == DY_FAILURE) {
      std::cout << MDY_U8("Failed to create validation layer of vulkan. Not supported.\n");
      OutputDebugString(L"Failed to create validation layer of vulkan. Not supported.\n");
      fflush(stdout);
      exit(1);
    }

    instanceCreateInfo.enabledLayerCount = 1;
    instanceCreateInfo.ppEnabledLayerNames = validationLayerNames;
  }

  VkInstance instance;
  return {instance, vkCreateInstance(&instanceCreateInfo, nullptr, &instance)};
}

///
/// @brief
///
void DyVkInitializeDebugCallback(VkInstance instance) {
  const VkDebugReportFlagsEXT debugReportFlags =
      VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
  auto CreateDebugReportCallback = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(
      vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT"));
  assert(CreateDebugReportCallback != nullptr);
#ifdef false
  auto DestroyDebugReportCallback = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(
      vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT"));
  auto DebugBreakCallback = reinterpret_cast<PFN_vkDebugReportMessageEXT>(
      vkGetInstanceProcAddr(instance, "vkDebugReportMessageEXT"));
#endif

  VkDebugReportCallbackCreateInfoEXT debugCreateInfo = {};
  debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
  debugCreateInfo.pfnCallback = static_cast<PFN_vkDebugReportCallbackEXT>(pVulkanMessageCallback);
  debugCreateInfo.flags = debugReportFlags;

  auto err = CreateDebugReportCallback(instance, &debugCreateInfo, nullptr, &vulkanMsgCallback);
  err = err;
  assert(!err);
}

///
/// @brief
///
[[nodiscard]] DVkSwapChainSupportDetails
DyVkFindProperSwapChainSupport(VkPhysicalDevice physicalDevice) {
  assert(vkSurface != nullptr);
  DVkSwapChainSupportDetails details;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, vkSurface, &details.capabilities);

  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, vkSurface, &formatCount, nullptr);
  if (formatCount > 0) {
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice,
                                         vkSurface,
                                         &formatCount,
                                         details.formats.data());
  }

  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, vkSurface, &presentModeCount, nullptr);
  if (presentModeCount > 0) {
    details.presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice,
                                              vkSurface,
                                              &presentModeCount,
                                              details.presentModes.data());
  }

  return details;
}

///
/// @brief
///
[[nodiscard]] DVkQueueFamilyIndices
DyVkFindProperQueueFamilies(VkPhysicalDevice vkDevice) {
  DVkQueueFamilyIndices indices;

  // Get the number of supporting queue families of vkDevice. ( physical )
  uint32_t vulkanQueueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(vkDevice, &vulkanQueueFamilyCount, nullptr);
  std::vector<VkQueueFamilyProperties> vulkanQueueFamilies(vulkanQueueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(vkDevice, &vulkanQueueFamilyCount, vulkanQueueFamilies.data());

  int32_t i = 0;
  for (const auto& queueFamily : vulkanQueueFamilies) {
    if (queueFamily.queueCount > 0 &&
        MDY_BITMASK_FLAG_TRUE(queueFamily.queueFlags, VK_QUEUE_GRAPHICS_BIT)) {
      indices.graphicsFamily = i;
      if (indices.IsCompleted()) break;
    }

    VkBool32 presentSupports = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(vkDevice, i, vkSurface, &presentSupports);
    if (queueFamily.queueCount > 0 && presentSupports) {
      indices.presentFamily = i;
      if (indices.IsCompleted()) break;
    }

    ++i;
  }
  return indices;
}

///
/// @brief
///
[[nodiscard]] VkSurfaceFormatKHR
DyVkChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
  if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
    return {VK_FORMAT_B8G8R8A8_SNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
  }

  for (const auto& availableSurfaceFormat : availableFormats) {
    if (availableSurfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM &&
        availableSurfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return availableSurfaceFormat;
    }
  }

  return availableFormats[0];
}

///
/// @brief
///
[[nodiscard]] VkPresentModeKHR
DyVkChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
  for (const auto& availablePresentMode : availablePresentModes) {
    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return availablePresentMode;
    }
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}

[[nodiscard]] VkExtent2D
DyVkChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
  if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
    return capabilities.currentExtent;
  }
  VkExtent2D actualExtent;

#if defined(_WIN32)
  RECT area;
  GetClientRect(windowHandle, &area);

  using TType = decltype(VkExtent2D::width);
  actualExtent = VkExtent2D{static_cast<TType>(area.right),
                            static_cast<TType>(area.bottom)};
#else
  // Do other platform way.
  assert(false);
#endif

  // Clamp extent size.
  actualExtent.width = std::clamp(actualExtent.width,
                                  capabilities.minImageExtent.width,
                                  capabilities.maxImageExtent.width);
  actualExtent.height = std::clamp(actualExtent.height,
                                   capabilities.minImageExtent.height,
                                   capabilities.maxImageExtent.height);
  return actualExtent;
}

///
/// @brief
///
bool DyVkCheckPhysicalDeviceExtensionSupport(VkPhysicalDevice device,
                                             const std::vector<const char*>& requiredExtensions) {
  // Get enumerate device extension properties from physics device.
  uint32_t extensionCount = 0;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

  std::set<std::string> requiredExtensionProperties(MDY_BIND_BEGIN_END(requiredExtensions));
  for (const auto& extension : availableExtensions) {
    requiredExtensionProperties.erase(extension.extensionName);
  }

  return requiredExtensionProperties.empty();
}

///
/// @brief
///
bool DyVkIsPhysicsDeviceSuitable(VkPhysicalDevice vkDevice) {
  const auto indices = DyVkFindProperQueueFamilies(vkDevice);
  const bool is_supports = DyVkCheckPhysicalDeviceExtensionSupport(vkDevice, vulkanDeviceExtensions);
  const auto swapchainSupportInfo = DyVkFindProperSwapChainSupport(vkDevice);

  return indices.IsCompleted() && is_supports &&
         !(swapchainSupportInfo.formats.empty() || swapchainSupportInfo.presentModes.empty());
}

///
/// @brief
///
[[nodiscard]] EDySuccess
DyVkFindPhysicsDevices(VkInstance instance, VkPhysicalDevice* physicalDevice) {
  uint32_t vulkanSupportGpuCount = 0;
  // Get the number of physical vulkan supporting devices
  {
    const VkResult err = vkEnumeratePhysicalDevices(instance, &vulkanSupportGpuCount, nullptr);
    assert(err == VK_SUCCESS);
    assert(vulkanSupportGpuCount > 0);
  }

  // Enumerate devices.
  std::vector<VkPhysicalDevice> vulkanPhysicalDevices(vulkanSupportGpuCount);
  const VkResult err = vkEnumeratePhysicalDevices(instance,
                                                  &vulkanSupportGpuCount,
                                                  vulkanPhysicalDevices.data());
  if (err) {
    std::cout << "Failed to enumerate physical devices : " << DyVkGetErrorString(err) << '\n';
    return DY_FAILURE;
  }

  // Select GPU.
  for (const auto& device : vulkanPhysicalDevices) {
    if (DyVkIsPhysicsDeviceSuitable(device)) {
      // Get physical device's supporting feature and properties.
      (*physicalDevice) = device;
      vkGetPhysicalDeviceProperties((*physicalDevice), &vkPhysicalDeviceProperties);
      vkGetPhysicalDeviceFeatures((*physicalDevice), &vkPhysicalDeviceFeatures);
      vkGetPhysicalDeviceMemoryProperties((*physicalDevice), &vkPhysicalDeviceMemoryProperties);
      break;
    }
  }

  if ((*physicalDevice) == VK_NULL_HANDLE) {
    std::cout << "Failed to find a suitable physical GPU.\n";
    return DY_FAILURE;
  }
  else {
    return DY_SUCCESS;
  }
}

///
/// @brief
///
[[nodiscard]] EDySuccess DyVkCreateSwapChain() {
  assert(vkPhysicalDevice != nullptr);
  assert(vkSurface != nullptr);

  // Get informations for creating swap chain object.
  const auto swapchainSupport = DyVkFindProperSwapChainSupport(vkPhysicalDevice);
  auto surfaceFormat    = DyVkChooseSwapSurfaceFormat(swapchainSupport.formats);
  auto presentMode      = DyVkChooseSwapPresentMode(swapchainSupport.presentModes);
  auto extent2D         = DyVkChooseSwapExtent(swapchainSupport.capabilities);

  // Increase swap chain FIFO queue length by 1, when only capabilities permits.
  uint32_t imageCount = swapchainSupport.capabilities.minImageCount + 1;
  if (swapchainSupport.capabilities.maxImageCount > 0 &&
      imageCount > swapchainSupport.capabilities.maxImageCount) {
    imageCount = swapchainSupport.capabilities.maxImageCount;
  }

  // Create swap chain object.
  VkSwapchainCreateInfoKHR swapCreateInfo = {};
  swapCreateInfo.sType    = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  swapCreateInfo.surface          = vkSurface;
  swapCreateInfo.minImageCount    = imageCount;
  swapCreateInfo.imageFormat      = surfaceFormat.format;
  swapCreateInfo.imageColorSpace  = surfaceFormat.colorSpace;
  swapCreateInfo.imageExtent      = extent2D;
  swapCreateInfo.imageArrayLayers = 1;
  swapCreateInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  const auto indices = DyVkFindProperQueueFamilies(vkPhysicalDevice);
  std::array<uint32_t, 2> queueFamilyIndices {
      static_cast<uint32_t>(indices.graphicsFamily),
      static_cast<uint32_t>(indices.presentFamily)};

  if (indices.graphicsFamily != indices.presentFamily) {
    swapCreateInfo.imageSharingMode       = VK_SHARING_MODE_CONCURRENT;
    swapCreateInfo.queueFamilyIndexCount  = 2;
    swapCreateInfo.pQueueFamilyIndices    = queueFamilyIndices.data();
  }
  else {
    swapCreateInfo.imageSharingMode       = VK_SHARING_MODE_EXCLUSIVE;
    swapCreateInfo.queueFamilyIndexCount  = 0;
    swapCreateInfo.pQueueFamilyIndices    = nullptr;
  }

  swapCreateInfo.preTransform   = swapchainSupport.capabilities.currentTransform;
  swapCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  swapCreateInfo.presentMode    = presentMode;
  swapCreateInfo.clipped        = VK_TRUE;
  swapCreateInfo.oldSwapchain   = VK_NULL_HANDLE;

  if (const auto err = vkCreateSwapchainKHR(vkLogicalDevice, &swapCreateInfo, nullptr, &vkSwapChain);
      err != VK_SUCCESS) {
    return DY_FAILURE;
  }

  vkGetSwapchainImagesKHR(vkLogicalDevice, vkSwapChain, &imageCount, nullptr);
  vkSwapChainImages.resize(imageCount);
  vkGetSwapchainImagesKHR(vkLogicalDevice, vkSwapChain, &imageCount, vkSwapChainImages.data());

  vkSwapChainImageFormat = surfaceFormat.format;
  vkSwapChainExtent      = extent2D;
  return DY_SUCCESS;
}

///
/// @brief
///
[[nodiscard]] EDySuccess DyVkCreateImageViews() {
  vkSwapChainImageViews.resize(vkSwapChainImages.size());

  const auto swapChainImageViewsSize = static_cast<int32_t>(vkSwapChainImageViews.size());
  for (int32_t i = 0; i < swapChainImageViewsSize; ++i) {
    VkImageViewCreateInfo imageViewCreateInfo = {};
    imageViewCreateInfo.sType     = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewCreateInfo.image     = vkSwapChainImages[i];
    imageViewCreateInfo.viewType  = VK_IMAGE_VIEW_TYPE_2D;
    imageViewCreateInfo.format    = vkSwapChainImageFormat;

    imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    imageViewCreateInfo.subresourceRange.aspectMask   = VK_IMAGE_ASPECT_COLOR_BIT;
    imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
    imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
    imageViewCreateInfo.subresourceRange.levelCount = 0;
    imageViewCreateInfo.subresourceRange.layerCount = 0;

    const auto errCode = vkCreateImageView(
        vkLogicalDevice,
        &imageViewCreateInfo,
        nullptr,
        &vkSwapChainImageViews[i]);
    if (errCode != VK_SUCCESS) {
      return DY_FAILURE;
    }
  }

  return DY_SUCCESS;
}

///
/// @brief
///
#if defined(_WIN32)
[[nodiscard]] EDySuccess DyVkCreateSurface(HWND hWnd, HINSTANCE hInstance)
#else

#endif
{
#if defined(_WIN32)
  VkWin32SurfaceCreateInfoKHR win32CreateInfo;
  win32CreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
  win32CreateInfo.hwnd  = windowHandle;
  win32CreateInfo.hinstance = hInstance;

  auto CreateWin32SurfaceKHR = reinterpret_cast<PFN_vkCreateWin32SurfaceKHR>(
      vkGetInstanceProcAddr(vkInstance, "vkCreateWin32SurfaceKHR"));
  if (!CreateWin32SurfaceKHR ||
      CreateWin32SurfaceKHR(vkInstance, &win32CreateInfo, nullptr, &vkSurface) != VK_SUCCESS) {
    return DY_FAILURE;
  }
  else {
    return DY_SUCCESS;
  }
#endif
}

///
/// @brief
///
[[nodiscard]] EDySuccess DyVkCreateRenderPass() {
  // format and samples should match the format of swap chain and msaa information.
  VkAttachmentDescription colorAttachment = {};
  colorAttachment.format  = vkSwapChainImageFormat;
  colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  colorAttachment.loadOp  = VK_ATTACHMENT_LOAD_OP_CLEAR;
  colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachment.stencilLoadOp   = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachment.stencilStoreOp  = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachment.initialLayout   = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachment.finalLayout     = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference colorAttachmentRef = {};
  colorAttachmentRef.attachment   = 0;
  colorAttachmentRef.layout       = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  VkSubpassDescription subpass    = {};
  subpass.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount    = 1;
  subpass.pColorAttachments       = &colorAttachmentRef;

  // Make render pass.
  VkRenderPassCreateInfo renderPassInfo = {};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount  = 1;
  renderPassInfo.pAttachments     = &colorAttachment;
  renderPassInfo.subpassCount     = 1;
  renderPassInfo.pSubpasses       = &subpass;

  if (const auto err = vkCreateRenderPass(vkLogicalDevice, &renderPassInfo, nullptr, &vkRenderPass);
      err != VK_SUCCESS) {
    std::cout << DyVkGetErrorString(err) << '\n';
    return DY_FAILURE;
  }
  return DY_SUCCESS;
}

///
/// @brief
///
[[nodiscard]] EDySuccess DyVkCreateShaderModule(
    const std::vector<char>& shaderCode,
    VkShaderModule& shaderModule) {
  VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
  shaderModuleCreateInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  shaderModuleCreateInfo.codeSize = shaderCode.size();
  shaderModuleCreateInfo.pCode    = reinterpret_cast<const uint32_t*>(shaderCode.data());

  if (vkCreateShaderModule(vkLogicalDevice, &shaderModuleCreateInfo, nullptr, &shaderModule)
      != VK_SUCCESS) {
    return DY_FAILURE;
  }
  else {
    return DY_SUCCESS;
  }
}

///
/// @brief
///
[[nodiscard]] EDySuccess DyVkCreateGraphicsPipeLine() {
  auto vertShaderCodeBuffer = DyReadBinaryFile(vertexFilePath);
  auto fragShaderCodeBuffer = DyReadBinaryFile(fragementFilePath);

  MDY_CALL_ASSERT_SUCCESS(DyVkCreateShaderModule(vertShaderCodeBuffer.value(), vkVertShaderModule));
  MDY_CALL_ASSERT_SUCCESS(DyVkCreateShaderModule(fragShaderCodeBuffer.value(), vkFragShaderModule));

  std::vector<VkPipelineShaderStageCreateInfo> shaderStageList;

  VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
  vertShaderStageInfo.sType   = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vertShaderStageInfo.stage   = VK_SHADER_STAGE_VERTEX_BIT;
  vertShaderStageInfo.module  = vkVertShaderModule;
  vertShaderStageInfo.pName   = "main";
  vertShaderStageInfo.pSpecializationInfo = nullptr;
  shaderStageList.emplace_back(vertShaderStageInfo);

  VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
  fragShaderStageInfo.sType   = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  fragShaderStageInfo.stage   = VK_SHADER_STAGE_FRAGMENT_BIT;
  fragShaderStageInfo.module  = vkFragShaderModule;
  fragShaderStageInfo.pName   = "main";
  fragShaderStageInfo.pSpecializationInfo = nullptr;
  shaderStageList.emplace_back(fragShaderStageInfo);

  VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
  vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertexInputInfo.vertexBindingDescriptionCount   = 0;
  vertexInputInfo.pVertexBindingDescriptions      = nullptr;
  vertexInputInfo.vertexAttributeDescriptionCount = 0;
  vertexInputInfo.pVertexAttributeDescriptions    = nullptr;

  VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
  inputAssembly.sType     = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  inputAssembly.topology  = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  inputAssembly.primitiveRestartEnable = VK_FALSE;

  // Make viewport
  VkViewport viewport = {};
  viewport.x          = 0.0f;
  viewport.y          = 0.0f;
  viewport.width      = static_cast<float>(vkSwapChainExtent.width);
  viewport.height     = static_cast<float>(vkSwapChainExtent.height);
  viewport.minDepth   = 0.0f;
  viewport.maxDepth   = 1.0f;

  VkRect2D scissorRectangle = {};
  scissorRectangle.offset = {0, 0};
  scissorRectangle.extent = vkSwapChainExtent;

  VkPipelineViewportStateCreateInfo viewportStateInfo = {};
  viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewportStateInfo.viewportCount = 1;
  viewportStateInfo.pViewports    = &viewport;
  viewportStateInfo.scissorCount  = 1;
  viewportStateInfo.pScissors     = &scissorRectangle;

  // Make Rasterizer information
  VkPipelineRasterizationStateCreateInfo rasterizationInfo = {};
  rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizationInfo.depthClampEnable        = VK_FALSE;
  rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
  rasterizationInfo.polygonMode             = VK_POLYGON_MODE_FILL;
  rasterizationInfo.lineWidth               = 1.0f;
  rasterizationInfo.cullMode                = VK_CULL_MODE_BACK_BIT;
  rasterizationInfo.frontFace               = VK_FRONT_FACE_CLOCKWISE; // (IMPORTANT!!)
  rasterizationInfo.depthBiasEnable         = VK_FALSE;
  rasterizationInfo.depthBiasConstantFactor = 0.0f;

  // Make MSAA information (disabled now)
  VkPipelineMultisampleStateCreateInfo multiSamplingInfo = {};
  multiSamplingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multiSamplingInfo.sampleShadingEnable   = VK_FALSE;
  multiSamplingInfo.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT;
  multiSamplingInfo.minSampleShading      = 1.0f;
  multiSamplingInfo.pSampleMask           = nullptr;

  // Make Depth and Stencil testing information (disabled now)
  VkPipelineDepthStencilStateCreateInfo depthStencilInfo = {};
  depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;

  // Make blending information
  VkPipelineColorBlendAttachmentState colorBlendingAttachment = {};
  colorBlendingAttachment.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT |
      VK_COLOR_COMPONENT_G_BIT |
      VK_COLOR_COMPONENT_G_BIT |
      VK_COLOR_COMPONENT_A_BIT;
  colorBlendingAttachment.blendEnable         = VK_FALSE;
  colorBlendingAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
  colorBlendingAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  colorBlendingAttachment.alphaBlendOp        = VK_BLEND_OP_ADD;
  colorBlendingAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
  colorBlendingAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
  colorBlendingAttachment.colorBlendOp        = VK_BLEND_OP_ADD;

  VkPipelineColorBlendStateCreateInfo colorBlendingInfo = {};
  colorBlendingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  colorBlendingInfo.logicOpEnable   = VK_FALSE;
  colorBlendingInfo.logicOp         = VK_LOGIC_OP_COPY;
  colorBlendingInfo.attachmentCount = 1;
  colorBlendingInfo.pAttachments    = &colorBlendingAttachment;

  VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  if (vkCreatePipelineLayout(vkLogicalDevice, &pipelineLayoutInfo, nullptr, &vkPipelineLayout)
      != VK_SUCCESS) {
    return DY_FAILURE;
  }

  VkGraphicsPipelineCreateInfo pipelineGraphicsInfo = {};
  pipelineGraphicsInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineGraphicsInfo.stageCount         = 2;
  pipelineGraphicsInfo.pStages            = shaderStageList.data();
  pipelineGraphicsInfo.pVertexInputState  = &vertexInputInfo;
  pipelineGraphicsInfo.pInputAssemblyState= &inputAssembly;
  pipelineGraphicsInfo.pViewportState     = &viewportStateInfo;
  pipelineGraphicsInfo.pRasterizationState= &rasterizationInfo;
  pipelineGraphicsInfo.pMultisampleState  = &multiSamplingInfo;
  pipelineGraphicsInfo.pColorBlendState   = &colorBlendingInfo;
  pipelineGraphicsInfo.pDepthStencilState = nullptr;
  pipelineGraphicsInfo.pDynamicState      = nullptr;

  pipelineGraphicsInfo.layout             = vkPipelineLayout;
  pipelineGraphicsInfo.renderPass         = vkRenderPass;
  pipelineGraphicsInfo.subpass            = 0;                  /// (index of subpass)

  if (vkCreateGraphicsPipelines(
      vkLogicalDevice,                    /// vk Logical device
      VK_NULL_HANDLE,                     /// (optional) Pipeline cache
      1,                                  /// GraphicsPipelineInfo count
      &pipelineGraphicsInfo,              /// Pointer of GraphicsPipelineInfo list.
      nullptr,                            /// (optional) Allocators
      &vkPipeline) != VK_SUCCESS) {       /// vk Pipeline handle pointer.
    return DY_FAILURE;
  }

  vkDestroyShaderModule(vkLogicalDevice, vkVertShaderModule, nullptr);
  vkDestroyShaderModule(vkLogicalDevice, vkFragShaderModule, nullptr);
  return DY_SUCCESS;
}

///
/// @brief
///
[[nodiscard]] EDySuccess DyVkCreateFrameBuffers() {
  const auto frameBufferListSize = static_cast<int32_t>(vkSwapChainImageViews.size());
  vkSwapChainFrameBuffers.resize(frameBufferListSize);

  for (int32_t i = 0; i < frameBufferListSize; ++i) {
    VkImageView attachments[] = { vkSwapChainImageViews[i] };
    VkFramebufferCreateInfo frameBufferInfo = {};
    frameBufferInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    frameBufferInfo.renderPass      = vkRenderPass;
    frameBufferInfo.attachmentCount = 1;
    frameBufferInfo.pAttachments    = attachments;
    frameBufferInfo.width           = vkSwapChainExtent.width;
    frameBufferInfo.height          = vkSwapChainExtent.height;
    frameBufferInfo.layers          = 1;

    if (vkCreateFramebuffer(vkLogicalDevice, &frameBufferInfo, nullptr, &vkSwapChainFrameBuffers[i])
        != VK_SUCCESS) {
      return DY_FAILURE;
    }
  }

  return DY_SUCCESS;
}

///
/// @brief
///
EDySuccess DyVkCreateCommandPool() {
  const auto queueFamilyIndices = DyVkFindProperQueueFamilies(vkPhysicalDevice);

  VkCommandPoolCreateInfo commandPoolInfo;
  commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  commandPoolInfo.queueFamilyIndex  = queueFamilyIndices.graphicsFamily;  /// Write queue index.
  commandPoolInfo.flags             = 0;                                  /// (Optional)

  if (const auto err = vkCreateCommandPool(
          vkLogicalDevice,
          &commandPoolInfo,
          nullptr,
          &vkCommandPool);
      err != VK_SUCCESS) {
    std::cout << DyVkGetErrorString(err) << '\n';
    return DY_FAILURE;
  }
  else {
    return DY_SUCCESS;
  }
}

///
/// @brief
///
void DyVkRenderFrame() {
  // Acquiring an image from the swap chain.
  static uint32_t currentFrame = 0;

  // Waiting fence.
  if (const auto err = vkWaitForFences(
          vkLogicalDevice,
          1,
          &vkSynchronizationFences[currentFrame],
          VK_TRUE,
          std::numeric_limits<uint64_t>::max());
      err != VK_SUCCESS) {
    std::cout << DyVkGetErrorString(err) << '\n';
    return;
  }

  // Write
  uint32_t imageIndex;
  if (const auto err = vkAcquireNextImageKHR(
          vkLogicalDevice,
          vkSwapChain,
          std::numeric_limits<uint64_t>::max(),
          vkImageAvailableSemaphores[currentFrame],
          VK_NULL_HANDLE,
          &imageIndex);
      err != VK_SUCCESS) {
    if (MDY_BITMASK_FLAG_TRUE(err, VK_ERROR_OUT_OF_DATE_KHR)) {
      if (bDyWindowCloseSign) return;
      MDY_CALL_ASSERT_SUCCESS(DyVkRecreateSwapChain());
      return;
    }
    else {
      std::cout << DyVkGetErrorString(err) << '\n';
      return;
    }
  };

  if (const auto err = vkResetFences(
          vkLogicalDevice,
          1,
          &vkSynchronizationFences[currentFrame]);
      err != VK_SUCCESS) {
    std::cout << DyVkGetErrorString(err) << '\n';
    return;
  }

  // Submit acquired image into the command buffer.
  VkPipelineStageFlags waitStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

  VkSubmitInfo submitInfo = {};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.waitSemaphoreCount   = 1;
  submitInfo.pWaitDstStageMask    = &waitStageMask;
  submitInfo.pWaitSemaphores      = &vkImageAvailableSemaphores[currentFrame];
  submitInfo.commandBufferCount   = 1;
  submitInfo.pCommandBuffers      = &vkCommandBuffers[imageIndex];
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores    = &vkRenderFinishedSemaphores[currentFrame];

  if (const auto err = vkQueueSubmit(
          vkGraphicsQueue,
          1,
          &submitInfo,
          vkSynchronizationFences[currentFrame]);
      err != VK_SUCCESS && err != VK_SUBOPTIMAL_KHR) {
    std::cout << DyVkGetErrorString(err) << '\n';
    return;
  }

  VkPresentInfoKHR presentInfo = {};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.waitSemaphoreCount  = 1;
  presentInfo.pWaitSemaphores     = &vkRenderFinishedSemaphores[currentFrame];
  presentInfo.swapchainCount      = 1;
  presentInfo.pSwapchains         = &vkSwapChain;
  presentInfo.pImageIndices       = &imageIndex;
  presentInfo.pResults            = nullptr;

  if (const auto err = vkQueuePresentKHR(vkGraphicsQueue, &presentInfo);
      err != VK_SUCCESS) {
    if (MDY_BITMASK_FLAG_TRUE(err, VK_ERROR_OUT_OF_DATE_KHR | VK_SUBOPTIMAL_KHR)) {
      if (bDyWindowCloseSign) return;
      MDY_CALL_ASSERT_SUCCESS(DyVkRecreateSwapChain());
    }
    else {
      std::cout << DyVkGetErrorString(err) << '\n';
      return;
    }
  }

  currentFrame = (currentFrame + 1) % kDyVkMaxFramesInFlight;
}

///
/// @brief
///
[[nodiscard]] EDySuccess DyVkCreateLogicalDevice() {
  const auto indices = DyVkFindProperQueueFamilies(vkPhysicalDevice);

  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
  std::set<int> uniqueQueueFamilies = {indices.graphicsFamily, indices.presentFamily};
  for (int queueFamilyIndex : uniqueQueueFamilies) {
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType             = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex  = queueFamilyIndex;
    queueCreateInfo.queueCount        = 1;
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities  = &queuePriority;
    queueCreateInfos.emplace_back(queueCreateInfo);
  }

  // Specifying used device features
  VkPhysicalDeviceFeatures deviceFeatures = {};

  // Create the logical devices
  VkDeviceCreateInfo createInfo       = {};
  createInfo.sType                    = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.queueCreateInfoCount     = static_cast<uint32_t>(queueCreateInfos.size());
  createInfo.pQueueCreateInfos        = queueCreateInfos.data();
  createInfo.pEnabledFeatures         = &deviceFeatures;
  createInfo.enabledExtensionCount    = static_cast<uint32_t>(vulkanDeviceExtensions.size());
  createInfo.ppEnabledExtensionNames  = vulkanDeviceExtensions.data();
  MDY_CHECK_EXECUTE(MDY_VULKAN_FLAG_VALIDATION_SET, {
    createInfo.enabledLayerCount      = static_cast<uint32_t>(1);
    createInfo.ppEnabledLayerNames    = validationLayerNames;
  });

  if (const auto result = vkCreateDevice(vkPhysicalDevice, &createInfo, nullptr, &vkLogicalDevice);
      result != VK_SUCCESS) {
    std::cout << "Failed to create logical device.\n";
    std::string errorString = DyVkGetErrorString(result);
    std::cout << errorString << '\n';
    OutputDebugString(std::wstring{errorString.begin(), errorString.end()}.c_str());
    return DY_FAILURE;
  }

  return DY_SUCCESS;
}

///
/// @brief
///
[[nodiscard]] EDySuccess DyVkGetDeviceQueues() {
  // Specifying the queues to be created
  const auto indices = DyVkFindProperQueueFamilies(vkPhysicalDevice);

  // Retrieve queue handle.
  vkGetDeviceQueue(vkLogicalDevice, indices.presentFamily, 0, &vkPresentQueue);
  // Get a graphics queue from the logical device explicitly.
  // VkQueue will be destroyed implicitly by being destroyed bound VkDevice instance.
  vkGetDeviceQueue(vkLogicalDevice, indices.graphicsFamily, 0, &vkGraphicsQueue);

  return DY_SUCCESS;
}


///
/// @brief
///
EDySuccess DyVkCreateCommandBuffers() {
  const auto commandBufferSize = static_cast<uint32_t>(vkSwapChainFrameBuffers.size());
  vkCommandBuffers.resize(commandBufferSize);

  VkCommandBufferAllocateInfo commandBufferAllocateInfo;
  commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  commandBufferAllocateInfo.commandPool = vkCommandPool;
  commandBufferAllocateInfo.level       = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  commandBufferAllocateInfo.commandBufferCount = commandBufferSize;

  if (const auto err = vkAllocateCommandBuffers(
          vkLogicalDevice,
          &commandBufferAllocateInfo,
          vkCommandBuffers.data());
      err != VK_SUCCESS) {
    std::cout << DyVkGetErrorString(err) << '\n';
    return DY_FAILURE;
  }

  for (uint32_t i = 0; i < commandBufferSize; ++i) {
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    beginInfo.pInheritanceInfo = nullptr;                           /// (Optional)

    if (const auto err = vkBeginCommandBuffer(vkCommandBuffers[i], &beginInfo);
        err != VK_SUCCESS) {
      std::cout << DyVkGetErrorString(err) << '\n';
      return DY_FAILURE;
    }

    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass   = vkRenderPass;
    renderPassInfo.framebuffer  = vkSwapChainFrameBuffers[i];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = vkSwapChainExtent;

    auto clearColor = VkClearValue{0.125f, 0.125f, 0.25f, 1.f};
    renderPassInfo.clearValueCount  = 1;
    renderPassInfo.pClearValues     = &clearColor;

    vkCmdBeginRenderPass(vkCommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(vkCommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipeline);
    vkCmdDraw(vkCommandBuffers[i], 3, 1, 0, 0);
    vkCmdEndRenderPass(vkCommandBuffers[i]);

    if (const auto err = vkEndCommandBuffer(vkCommandBuffers[i]);
        err != VK_SUCCESS) {
      std::cout << DyVkGetErrorString(err) << '\n';
      return DY_FAILURE;
    }
  }

  return DY_SUCCESS;
}

///
/// @brief
///
[[nodiscard]] EDySuccess DyVkCreateSemaphores() {
  vkImageAvailableSemaphores.resize(kDyVkMaxFramesInFlight);
  vkRenderFinishedSemaphores.resize(kDyVkMaxFramesInFlight);

  VkSemaphoreCreateInfo semaphoreInfo = {};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  for (int32_t i = 0; i < kDyVkMaxFramesInFlight; ++i) {
    if (const auto err = vkCreateSemaphore(
            vkLogicalDevice,
            &semaphoreInfo,
            nullptr,
            &vkImageAvailableSemaphores[i]);
        err != VK_SUCCESS) {
      std::cout << DyVkGetErrorString(err) << '\n';
      return DY_FAILURE;
    }

    if (const auto err = vkCreateSemaphore(
            vkLogicalDevice,
            &semaphoreInfo,
            nullptr,
            &vkRenderFinishedSemaphores[i]);
        err != VK_SUCCESS) {
      std::cout << DyVkGetErrorString(err) << '\n';
      return DY_FAILURE;
    }
  }

  return DY_SUCCESS;
}

///
/// @brief
///
[[nodiscard]] EDySuccess DyVkCreateFences() {
  vkSynchronizationFences.resize(kDyVkMaxFramesInFlight);

  VkFenceCreateInfo fenceCreateInfo = {};
  fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  for (int32_t i = 0; i < kDyVkMaxFramesInFlight; ++i) {
    if (const auto err = vkCreateFence(
            vkLogicalDevice,
            &fenceCreateInfo,
            nullptr,
            &vkSynchronizationFences[i]);
        err != VK_SUCCESS) {
      std::cout << DyVkGetErrorString(err) << '\n';
      return DY_FAILURE;
    }
  }

  return DY_SUCCESS;
}

///
/// @brief
///
void DyVkCleanSwapChainResources() {
  for (auto& frameBuffer : vkSwapChainFrameBuffers) {
    vkDestroyFramebuffer(vkLogicalDevice, frameBuffer, nullptr);
  }
  vkFreeCommandBuffers(
      vkLogicalDevice,
      vkCommandPool,
      static_cast<uint32_t>(vkCommandBuffers.size()),
      vkCommandBuffers.data());
  vkDestroyPipeline(vkLogicalDevice, vkPipeline, nullptr);
  vkDestroyPipelineLayout(vkLogicalDevice, vkPipelineLayout, nullptr);
  vkDestroyRenderPass(vkLogicalDevice, vkRenderPass, nullptr);
  for (auto& imageView : vkSwapChainImageViews) {
    vkDestroyImageView(vkLogicalDevice, imageView, nullptr);
  }
  vkDestroySwapchainKHR(vkLogicalDevice, vkSwapChain, nullptr);
}

///
/// @brief Clean all explicit resources which needs to be destroyed explicitly by application.
/// This function must be called when using Vulkan graphics library.
///
void DyVkCleanupResources() {
  DyVkCleanSwapChainResources();

  for (int32_t i = 0; i < kDyVkMaxFramesInFlight; ++i) {
    vkDestroyFence(vkLogicalDevice, vkSynchronizationFences[i], nullptr);
  }
  for (int32_t i = 0; i < kDyVkMaxFramesInFlight; ++i) {
    vkDestroySemaphore(vkLogicalDevice, vkRenderFinishedSemaphores[i], nullptr);
    vkDestroySemaphore(vkLogicalDevice, vkImageAvailableSemaphores[i], nullptr);
  }
  vkDestroyCommandPool(vkLogicalDevice, vkCommandPool, nullptr);

  vkDestroySurfaceKHR(vkInstance, vkSurface, nullptr);
  vkDestroyDevice(vkLogicalDevice, nullptr);
  vkDestroyInstance(vkInstance, nullptr);
}

///
/// @brief
///
[[nodiscard]] EDySuccess DyVkRecreateSwapChain() {
  // Must wait until everything is done.
  vkDeviceWaitIdle(vkLogicalDevice);

  DyVkCleanSwapChainResources();
  MDY_CALL_ASSERT_SUCCESS(DyVkCreateSwapChain());
  MDY_CALL_ASSERT_SUCCESS(DyVkCreateImageViews());
  MDY_CALL_ASSERT_SUCCESS(DyVkCreateRenderPass());
  MDY_CALL_ASSERT_SUCCESS(DyVkCreateGraphicsPipeLine());
  MDY_CALL_ASSERT_SUCCESS(DyVkCreateFrameBuffers());
  MDY_CALL_ASSERT_SUCCESS(DyVkCreateCommandBuffers());

  return DY_SUCCESS;
}

} /// ::dy namespace