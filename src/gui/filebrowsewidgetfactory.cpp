#include "gui/filebrowsewidgetfactory.hpp"

#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

#include <memory>

namespace mad { namespace utils { namespace gui {

FileBrowseWidgetFactory::FileBrowseWidgetFactory(const Parts& partFlags)
  : m_partFlags(partFlags)
{
}

const FileBrowseWidgetFactory::Parts& FileBrowseWidgetFactory::parts() const
{
  return m_partFlags;
}

void FileBrowseWidgetFactory::setPart(Part part, bool on)
{
  m_partFlags.set(part, on);
}

void FileBrowseWidgetFactory::setParts(const Parts& parts)
{
  m_partFlags = parts;
}

std::unique_ptr<QWidget> FileBrowseWidgetFactory::build(QWidget* parent) const
{
  auto widget = std::make_unique<QWidget>(parent);
  auto widgetLayout = new QHBoxLayout(widget.get());

  auto parts = buildParts(widget.get()).release();
  while (!parts.empty())
  {
    auto it = parts.begin();
    widgetLayout->addWidget(it->release());
    parts.erase(it);
  }

  return widget;
}

WidgetPartsCollection<FileBrowseWidgetFactory> FileBrowseWidgetFactory::buildParts(QWidget* parent) const
{
  WidgetPartsCollection<FileBrowseWidgetFactory> parts;

  if (m_partFlags.isSet(EditBoxPart))
    parts.add(EditBoxPart, std::make_unique<QLineEdit>(parent));
  if (m_partFlags.isSet(BrowseButtonPart))
    parts.add(BrowseButtonPart, std::make_unique<QPushButton>(tr("Browse..."), parent));

  return parts;
}

}}} // namespace mad::utils::gui
