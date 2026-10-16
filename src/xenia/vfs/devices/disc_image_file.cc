/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2013 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include "xenia/vfs/devices/disc_image_file.h"

#include "xenia/base/logging.h"
#include "xenia/vfs/devices/disc_image_entry.h"
#include "xenia/vfs/devices/disc_image_device.h"
namespace xe {
namespace vfs {

DiscImageFile::DiscImageFile(uint32_t file_access, DiscImageEntry* entry)
    : File(file_access, entry), entry_(entry) {}

DiscImageFile::~DiscImageFile() = default;

void DiscImageFile::Destroy() { delete this; }

X_STATUS DiscImageFile::ReadSync(std::span<uint8_t> buffer, size_t byte_offset,
                                 size_t* out_bytes_read) {
  if (byte_offset >= entry_->size()) {
    return X_STATUS_END_OF_FILE;
  }

  const size_t real_length =
      std::min(buffer.size(), entry_->data_size() - byte_offset);
  if (!real_length) {
    if (out_bytes_read) {
      *out_bytes_read = 0;
    }
    return X_STATUS_SUCCESS;
  }

  const size_t real_offset = entry_->data_offset() + byte_offset;
#if XE_PLATFORM_WINRT
  // UWP opens disc images as FileHandle streams rather than memory mappings.
  // Reading from entry_->mmap() here always returns EOF on Xbox, even when
  // the ISO filesystem and default.xex were successfully discovered.
  auto* device = static_cast<DiscImageDevice*>(entry_->device());
  size_t bytes_read = 0;
  if (!device->ReadImageData(real_offset, buffer.data(), real_length,
                             &bytes_read) ||
      bytes_read != real_length) {
    XELOGE("XISO: failed reading '{}' at offset {} ({} of {} bytes)",
           entry_->name(), real_offset, bytes_read, real_length);
    if (out_bytes_read) {
      *out_bytes_read = bytes_read;
    }
    return X_STATUS_UNSUCCESSFUL;
  }
  if (out_bytes_read) {
    *out_bytes_read = bytes_read;
  }
#else
  auto* mmap = entry_->mmap();
  if (!mmap) {
    return X_STATUS_END_OF_FILE;
  }
  std::memcpy(buffer.data(), mmap->data() + real_offset, real_length);
  if (out_bytes_read) {
    *out_bytes_read = real_length;
  }
#endif
  return X_STATUS_SUCCESS;
}

}  // namespace vfs
}  // namespace xe
