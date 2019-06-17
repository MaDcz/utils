#pragma once

#include <memory>
#include <string>

namespace mad { namespace imageacquisition {
class Device;
}}

namespace mad {

class ImageAcquisition
{
public:
  virtual ~ImageAcquisition() {}

  virtual std::unique_ptr<imageacquisition::Device> configure(const std::string& deviceId = {}) = 0;
};

} // mad
