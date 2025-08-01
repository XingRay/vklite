//
// Created by leixing on 2025/6/8.
//

#ifdef VKLITE_ANDROID
#include "HardwareBufferDeviceMemoryBuilder.h"

namespace vklite {

    HardwareBufferDeviceMemoryBuilder::HardwareBufferDeviceMemoryBuilder() {
        mMemoryDedicatedAllocateInfo.setPNext(&mHardwareBufferInfo);
        mDeviceMemoryBuilder.next(&mMemoryDedicatedAllocateInfo);
    }

    HardwareBufferDeviceMemoryBuilder::~HardwareBufferDeviceMemoryBuilder() = default;

    HardwareBufferDeviceMemoryBuilder &HardwareBufferDeviceMemoryBuilder::device(vk::Device device) {
        mDevice = device;
        mDeviceMemoryBuilder.device(device);
        return *this;
    }

    HardwareBufferDeviceMemoryBuilder &HardwareBufferDeviceMemoryBuilder::hardwareBuffer(AHardwareBuffer *hardwareBuffer) {
        mHardwareBufferInfo
                .setBuffer(hardwareBuffer);
        return *this;
    }

    HardwareBufferDeviceMemoryBuilder &HardwareBufferDeviceMemoryBuilder::image(vk::Image image) {
        mMemoryDedicatedAllocateInfo.setImage(image);
        return *this;
    }

    HardwareBufferDeviceMemoryBuilder &HardwareBufferDeviceMemoryBuilder::allocationSize(vk::DeviceSize allocationSize) {
        mDeviceMemoryBuilder.allocationSize(allocationSize);
        return *this;
    }

    HardwareBufferDeviceMemoryBuilder &HardwareBufferDeviceMemoryBuilder::memoryTypeIndex(uint32_t memoryTypeIndex) {
        mDeviceMemoryBuilder.memoryTypeIndex(memoryTypeIndex);
        return *this;
    }

    DeviceMemory HardwareBufferDeviceMemoryBuilder::build() {
        return mDeviceMemoryBuilder.build();
    }

    std::unique_ptr<DeviceMemory> HardwareBufferDeviceMemoryBuilder::buildUnique() {
        return std::make_unique<DeviceMemory>(build());
    }

} // vklite
#endif
