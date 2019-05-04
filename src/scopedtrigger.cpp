#include "scopedtrigger.hpp"

namespace mad { namespace utils {

ScopedTrigger::ScopedTrigger(const std::function<void()>& func)
  : m_func(func)
{
}

ScopedTrigger::~ScopedTrigger()
{
  if (m_func)
  {
    m_func();
    m_func = {};
  }
}

}} // namespace mad::utils
