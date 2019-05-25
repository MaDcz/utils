#include "ui/layoutarranger.hpp"

#include <QGridLayout>

#include <assert.h>
#include <vector>

namespace mad { namespace utils { namespace ui {

//
// LayoutArranger::Impl
//

class LayoutArranger::Impl
{
public:
  virtual ~Impl() {}

  virtual void place(std::vector<std::unique_ptr<QWidget>>&& parts) const = 0;
};

namespace {

//
// GridLayoutImpl
//

/**
 * Grid layout specific private implementation.
 */
class GridLayoutImpl : public LayoutArranger::Impl
{
public:
  GridLayoutImpl(QGridLayout& gridLayout);

  void place(std::vector<std::unique_ptr<QWidget>>&& parts) const override;

private:
  QGridLayout& m_gridLayout;
};

GridLayoutImpl::GridLayoutImpl(QGridLayout& gridLayout)
  : m_gridLayout(gridLayout)
{
}

void GridLayoutImpl::place(std::vector<std::unique_ptr<QWidget>>&& parts) const
{
  const int row = m_gridLayout.rowCount();
  int col = 0;
  for (size_t i = 0; i < parts.size(); ++i, ++col)
  {
    auto& part = parts[i];
    assert(part);

    int stretch = 1;
    if (i == 0 && int(parts.size()) < m_gridLayout.columnCount())
      // TODO Stretching the first widget to nicely fill the grid.
      //      This will need to be more sophisticated, controlled by the parts provider.
      //      The question is which widget is the main part and will the number of columns
      //      increase afterwards?
      stretch += m_gridLayout.columnCount() - int(parts.size());

    m_gridLayout.addWidget(part.release(), row, col, 1, stretch);

    col += stretch - 1;
  }
}

} // namespace

//
// LayoutArranger
//

LayoutArranger::LayoutArranger(QGridLayout& gridLayout, QWidget* parent)
  : m_impl(new GridLayoutImpl(gridLayout)),
    m_parent(parent)
{
}

LayoutArranger::~LayoutArranger()
{
}

void LayoutArranger::place(std::vector<std::unique_ptr<QWidget>>&& parts) const
{
  m_impl->place(std::move(parts));
}

}}} // namespace mad::utils::ui
