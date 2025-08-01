//
// Created by leixing on 2025/5/30.
//

#include "DeviceMemory.h"

#include <utility>
#include <format>

namespace vklite {

    DeviceMemory::DeviceMemory(const vk::Device &device, const vk::DeviceMemory &deviceMemory, DeviceMemoryMeta &&meta)
            : mDevice(device), mDeviceMemory(deviceMemory), mMeta(std::move(meta)) {}

    DeviceMemory::~DeviceMemory() {
        if (mDevice != nullptr && mDeviceMemory != nullptr) {
            mDevice.free(mDeviceMemory);
            mDevice = nullptr;
            mDeviceMemory = nullptr;
        }
    }

    DeviceMemory::DeviceMemory(DeviceMemory &&other) noexcept
            : mDevice(std::exchange(other.mDevice, nullptr)),
              mDeviceMemory(std::exchange(other.mDeviceMemory, nullptr)),
              mMeta(std::move(other.mMeta)) {}

    DeviceMemory &DeviceMemory::operator=(DeviceMemory &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
            mDeviceMemory = std::exchange(other.mDeviceMemory, nullptr);
            mMeta = std::move(other.mMeta);
        }
        return *this;
    }

    const vk::DeviceMemory &DeviceMemory::getVkDeviceMemory() const {
        return mDeviceMemory;
    }

    void DeviceMemory::updateBuffer(const void *data, vk::DeviceSize size, vk::DeviceSize offset, vk::MemoryMapFlags flags)const{
        if (data == nullptr) {
            throw std::runtime_error("DeviceMemory::updateBuffer: data == nullptr");
            return;
        }
        if (size == 0) {
            throw std::runtime_error("DeviceMemory::updateBuffer: size == 0");
            return;
        }
        if (size + offset > mMeta.getAllocationSize()) {
            throw std::runtime_error(std::format("DeviceMemory::updateBuffer: Data size ({}) exceeds buffer size ({})!", size, (uint32_t) mMeta.getAllocationSize()));
        }

        void *mappedMemoryPointer = mDevice.mapMemory(mDeviceMemory, offset, size, flags);

        if (mappedMemoryPointer == nullptr) {
            throw std::runtime_error("DeviceMemory::updateBuffer: mDevice.mapMemory() => null, buffer memory is not mapped!");
            return;
        }

        memcpy(mappedMemoryPointer, data, size);
        mDevice.unmapMemory(mDeviceMemory);
    }

    void DeviceMemory::updateBuffer(const void *data, vk::DeviceSize size) const{
        updateBuffer(data, size, 0);
    }

} // vklite