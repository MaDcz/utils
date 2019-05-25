#pragma once

#include <assert.h>

namespace mad { namespace utils {

/**
 * @brief Type safe flags-like enum handling.
 */
template <typename EnumT>
class Flags
{
public:
  Flags();

  Flags(EnumT flags);

  Flags& operator=(EnumT flags);

  // TODO Underlying type.
  Flags(int flags);

  Flags& operator=(int flags);

  /**
   * @brief Switches the given flag(s) on/off.
   */
  void set(EnumT flag, bool on = true);

  /**
   * @brief Switches the given flag(s) off.
   */
  void unset(EnumT flag);

  /**
   * @brief Tests if a given flag (or any of the given flags) is set.
   */
  bool isSet(EnumT flag) const;

  /**
   * @brief Switches all flags off.
   */
  void clear();

  /**
   * @brief Switches the given flag(s) off.
   *
   * This is an alias for \c unset().
   */
  void clear(EnumT flag);

  /**
   * @brief Returns \c true no flag is set, \c flase otherwise.
   */
  bool empty() const;

  /**
   * @brief Returns \c true if any of the flags is set, \c flase otherwise.
   */
  explicit operator bool() const;

private:
  EnumT m_flags = {};
};

template <typename EnumT>
inline Flags<EnumT>::Flags()
{
  assert(!m_flags);
}

template <typename EnumT>
inline Flags<EnumT>::Flags(EnumT flags)
  : m_flags(flags)
{
}

template <typename EnumT>
inline Flags<EnumT>& Flags<EnumT>::operator=(EnumT flags)
{
  m_flags = flags;
}

template <typename EnumT>
inline Flags<EnumT>::Flags(int flags)
  : m_flags(EnumT(flags))
{
}

template <typename EnumT>
inline Flags<EnumT>& Flags<EnumT>::operator=(int flags)
{
  m_flags = EnumT(flags);
}

template <typename EnumT>
inline void Flags<EnumT>::set(EnumT flag, bool on)
{
  if (on)
    m_flags = EnumT(m_flags | flag);
  else
    m_flags = EnumT(m_flags & ~flag);
}

template <typename EnumT>
inline void Flags<EnumT>::unset(EnumT flag)
{
  m_flags = EnumT(m_flags & ~flag);
}

template <typename EnumT>
inline bool Flags<EnumT>::isSet(EnumT flag) const
{
  return m_flags & flag;
}

template <typename EnumT>
inline void Flags<EnumT>::clear()
{
  m_flags = {};
}

template <typename EnumT>
inline void Flags<EnumT>::clear(EnumT flag)
{
  unset(flag);
}

template <typename EnumT>
inline bool Flags<EnumT>::empty() const
{
  return !bool(m_flags);
}

template <typename EnumT>
inline Flags<EnumT>::operator bool() const
{
  return bool(m_flags);
}

}} // namespace mad::utils
