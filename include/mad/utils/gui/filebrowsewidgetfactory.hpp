#pragma once

#include "../flags.hpp"
#include "widgetfactory.hpp"

#include <QCoreApplication>

class QWidget;

namespace mad { namespace utils { namespace gui {

/**
 * @brief Factory for creating file browse widget usually composed of an edit box and a browse button.
 */
class FileBrowseWidgetFactory : public WidgetFactory<FileBrowseWidgetFactory>
{
  Q_DECLARE_TR_FUNCTIONS(FileBrowseWidgetFactory)

public:
  /**
   * @brief Individual parts of the file browse widget.
   */
  enum Part
  {
    EditBoxPart       = 1 << 0,
    BrowseButtonPart  = 1 << 1,
    AllParts          = EditBoxPart | BrowseButtonPart
  };

  using Parts = Flags<Part>;

public:
  explicit FileBrowseWidgetFactory(const Parts& partFlags = EditBoxPart | BrowseButtonPart);

  /**
   * @brief Returns part flags.
   */
  const Parts& parts() const;

  /**
   * @brief Sets a particular flag(s) on, or off.
   */
  void setPart(Part part, bool on = true);

  /**
   * @brief Sets a part flags to the given state.
   */
  void setParts(const Parts& parts);

  std::unique_ptr<QWidget> build(QWidget* parent = nullptr) const override;

  WidgetPartsCollection<FileBrowseWidgetFactory> buildParts(QWidget* parent = nullptr) const override;

private:
  Parts m_partFlags;
};

}}} // namespace mad::utils::gui
