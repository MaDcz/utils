#pragma once

#include <QImage>

#include <string>

namespace mad { namespace imageacquisition {

class Device
{
public:
  virtual ~Device() {}

  virtual const std::string& id() const = 0;

  virtual QImage acquireImage() = 0;
};

}} // mad::imageacquisition
