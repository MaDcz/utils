#pragma once

#include <functional>

namespace mad { namespace utils {

class ScopedTrigger
{
public:
  explicit ScopedTrigger(const std::function<void()>& func);

  ScopedTrigger(const ScopedTrigger&) = delete;
  ScopedTrigger& operator=(const ScopedTrigger&) = delete;

  ScopedTrigger(ScopedTrigger&&) = default;
  ScopedTrigger& operator=(ScopedTrigger&&) = default;

  virtual ~ScopedTrigger();

private:
  std::function<void()> m_func;
};

}} // namespace mad::utils
