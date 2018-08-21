#ifndef GUARD_DY_VK_INTERFACE_H
#define GUARD_DY_VK_INTERFACE_H
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

#include <cstdint>

#include <string>
#include <vector>

#include <Dy/Helper/enum_flags.h>

#include <vulkan/vulkan.h>

namespace dy
{

//!
//! Vulkan API
//!

constexpr const char* vertexFilePath          = "./vert.spv";
constexpr const char* fragementFilePath       = "./frag.spv";
constexpr int32_t     kDyVkMaxFramesInFlight  = 2;

///
/// @struct DVkQueueFamilyIndices
/// @brief Struct for storing vulkan queue family index of specified vulkan physics device.
///
struct DVkQueueFamilyIndices final {
  /// Used to write pixels to swap chain.
  int32_t graphicsFamily = MDY_NOT_INITIALIZED;
  /// Used to present pixels to monitor or output devices.
  int32_t presentFamily  = MDY_NOT_INITIALIZED;

  bool IsCompleted() const noexcept {
    return graphicsFamily >= 0 && presentFamily >= 0;
  }
};

///
/// @struct DVkSwapChainSupportDetails
/// @brief
///
struct DVkSwapChainSupportDetails final {
  VkSurfaceCapabilitiesKHR        capabilities = {};
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR>   presentModes;
};

// @TODO temporal
#if defined(_WIN32)
void DyVkInitialize(HWND windowHandle, HINSTANCE hInstance);
#endif

std::pair<VkInstance, VkResult> DyVkCreateVulkanInstance();
std::string               DyVkGetErrorString(VkResult result_id);

void                      DyVkInitializeDebugCallback(VkInstance instance);
bool                      DyVkCheckPhysicalDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*>& requiredExtensions);
EDySuccess                DyVkCheckValidationLayerSupport(const char* (*validationLayers), int32_t layerCount);

DVkSwapChainSupportDetails  DyVkFindProperSwapChainSupport(VkPhysicalDevice physicalDevice);
DVkQueueFamilyIndices       DyVkFindProperQueueFamilies(VkPhysicalDevice vkDevice) ;

EDySuccess                DyVkCreateSwapChain();
VkSurfaceFormatKHR        DyVkChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
VkPresentModeKHR          DyVkChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
VkExtent2D                DyVkChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

EDySuccess                DyVkCreateImageViews();
EDySuccess                DyVkCreateRenderPass();
EDySuccess                DyVkCreateShaderModule(const std::vector<char>& shaderCode, VkShaderModule& shaderModule);
EDySuccess                DyVkCreateGraphicsPipeLine();
EDySuccess                DyVkCreateFrameBuffers();
EDySuccess                DyVkCreateCommandPool();
EDySuccess                DyVkCreateCommandBuffers();
EDySuccess                DyVkCreateSemaphores();
EDySuccess                DyVkCreateFences();

void                      DyVkCleanupResources();
EDySuccess                DyVkRecreateSwapChain();

void                      DyVkRenderLoop();
void                      DyVkRenderFrame();

} /// ::dy namespace

#endif /// GUARD_DY_VK_INTERFACE_H