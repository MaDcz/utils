#pragma once

#include <QWidget>

#include <assert.h>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>

namespace mad { namespace utils { namespace gui {

/**
 * @brief Holds individual widget parts created by a specific widget factory.
 *
 * The parts are accessible either in the order in which they should occur in the ui
 * or through an enum that defines the individual parts.
 */
template <typename WidgetFactoryT>
class WidgetPartsCollection
{
public:
  WidgetPartsCollection() {}

  WidgetPartsCollection(const WidgetPartsCollection&) = delete;
  WidgetPartsCollection& operator=(const WidgetPartsCollection&) = delete;

  WidgetPartsCollection(WidgetPartsCollection&&) = default;
  WidgetPartsCollection& operator=(WidgetPartsCollection&&) = default;

  /**
   * Adds a new part into the collection.
   */
  void add(typename WidgetFactoryT::Part partFlag, std::unique_ptr<QWidget>&& part);

  /**
   * Returns the number of parts in the collection.
   */
  size_t size() const;

  /**
   * Releases all the parts at once.
   */
  std::vector<std::unique_ptr<QWidget>> release();

  /**
   * Releases a specific part from the collection.
   */
  std::unique_ptr<QWidget> release(typename WidgetFactoryT::Part partFlag);

private:
  std::vector<std::unique_ptr<QWidget>> m_parts;
  std::map<typename WidgetFactoryT::Part, QWidget*> m_partsByFlags;
};

template <typename WidgetFactoryT>
inline void WidgetPartsCollection<WidgetFactoryT>::add(typename WidgetFactoryT::Part partFlag,
    std::unique_ptr<QWidget>&& part)
{
  assert(part);

  if (m_partsByFlags.count(partFlag))
    throw std::logic_error("The part is already present in the collection");

  m_partsByFlags[partFlag] = part.get();
  m_parts.push_back(std::move(part));
}

template <typename WidgetFactoryT>
inline size_t WidgetPartsCollection<WidgetFactoryT>::size() const
{
  return m_parts.size();
}

template <typename WidgetFactoryT>
inline std::vector<std::unique_ptr<QWidget>> WidgetPartsCollection<WidgetFactoryT>::release()
{
  m_partsByFlags.clear();
  return std::move(m_parts);
}

template <typename WidgetFactoryT>
inline std::unique_ptr<QWidget> WidgetPartsCollection<WidgetFactoryT>::release(
    typename WidgetFactoryT::Part partFlag)
{
  auto it = m_partsByFlags.find(partFlag);
  if (it == m_partsByFlags.end())
    return nullptr;

  auto partsIt = m_parts.begin();
  for (; partsIt != m_parts.end(); ++partsIt)
    if (partsIt->get() == it->second)
      break;

  assert(partsIt != m_parts.end());
  auto part = std::move(*partsIt);
  m_parts.erase(partsIt);
  m_partsByFlags.erase(it);
  assert(m_parts.size() == m_partsByFlags.size());
  return std::move(part);
}

}}} // namespace mad::utils::gui
