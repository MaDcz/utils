#include "gui/features/zoomfeature.hpp"

#include "gui/events/mousewheelevent.hpp"

#include <QIcon>
#include <QIntValidator>
#include <QLineEdit>
#include <QToolBar>
#include <QToolButton>

namespace mad { namespace utils { namespace gui {

//
// ZoomFeature::State
//

class ZoomFeature::State : public Feature::State,
                           public virtual MouseWheelEventProcessor
{
public:
  explicit State(ZoomFeature& zoomFeature);

  using MouseWheelEventProcessor::process;

  void process(const MouseWheelEvent& event) override;

private:
  ZoomFeature& m_zoomFeature;
};

ZoomFeature::State::State(ZoomFeature& zoomFeature)
  : Feature::State(zoomFeature),
    m_zoomFeature(zoomFeature)
{
}

void ZoomFeature::State::process(const MouseWheelEvent& event)
{
  if (event.modifiers != InputEvent::CtrlModifier)
    return;

  m_zoomFeature.increaseZoom(event.stepsDelta);
}

//
// ZoomFeature::Action
//

ZoomFeature::Action::Action(ZoomFeature& zoomFeature, QObject* parent)
  : QWidgetAction(parent),
    m_zoomFeature(zoomFeature)
{
}

ZoomFeature::Action::~Action()
{
}

QWidget* ZoomFeature::Action::createWidget(QWidget* parent)
{
  auto themeName = QIcon::themeName();
  auto themeSearchPaths = QIcon::themeSearchPaths();

  auto widget = new QToolBar(parent);

  // TODO Provide icons for plus and minus buttons.
  auto zoomPlus = new QToolButton;
  zoomPlus->setToolButtonStyle(Qt::ToolButtonFollowStyle);
  zoomPlus->setText("+");
  widget->addWidget(zoomPlus);

  // TODO Check that it's okay to have a single validator for multiple line edits.
  if (!m_zoomEditsValidator)
  {
    m_zoomEditsValidator = new QIntValidator(this);
    m_zoomEditsValidator->setRange(
        int(m_zoomFeature.settings().min), int(m_zoomFeature.settings().max));
  }

  // FIXME The only way how to commit the entered value is to focus out somehow, hitting enter
  //       will close the dialog.
  auto zoomEdit = new QLineEdit;
  zoomEdit->setValidator(m_zoomEditsValidator);
  zoomEdit->setFixedWidth(50);
  zoomEdit->setAlignment(Qt::AlignRight);
  zoomEdit->setText(QString::number(m_zoomFeature.zoom()));
  widget->addWidget(zoomEdit);
  m_zoomEdits.emplace_back(zoomEdit);

  auto zoomMinus = new QToolButton;
  zoomMinus->setText("-");
  widget->addWidget(zoomMinus);

  connect(zoomPlus, &QToolButton::clicked, this, &Action::zoomPlusClicked);
  connect(zoomMinus, &QToolButton::clicked, this, &Action::zoomMinusClicked);
  connect(&m_zoomFeature.sigs(), &Signals::zoomChanged, this, &Action::zoomChanged);
  connect(&m_zoomFeature.sigs(), &Signals::settingsChanged, this, &Action::zoomSettingsChanged);
  connect(zoomEdit, &QLineEdit::editingFinished, this, &Action::zoomEditingFinished);

  return widget;
}

void ZoomFeature::Action::zoomPlusClicked()
{
  m_zoomFeature.increaseZoom();
}

void ZoomFeature::Action::zoomMinusClicked()
{
  m_zoomFeature.decreaseZoom();
}

void ZoomFeature::Action::zoomChanged(unsigned int newZoom)
{
  auto it = m_zoomEdits.begin();
  while (it != m_zoomEdits.end())
  {
    if (it->isNull())
      it = m_zoomEdits.erase(it);
    else
      (*(it++))->setText(QString::number(newZoom));
  }
}

void ZoomFeature::Action::zoomSettingsChanged(const Settings& newSettings)
{
  if (m_zoomEditsValidator)
    m_zoomEditsValidator->setRange(int(newSettings.min), int(newSettings.max));
}

// FIXME The editingFinished() signal isn't emitted if the value entered is invalid. We need
//       to reset the line edit text in that case so it will display current zoom.
void ZoomFeature::Action::zoomEditingFinished()
{
  bool ok = false;
  auto senderLineEdit = qobject_cast<QLineEdit*>(sender());
  assert(senderLineEdit);
  auto zoom = senderLineEdit->text().toUInt(&ok);
  if (ok)
    m_zoomFeature.setZoom(zoom);
  else
    senderLineEdit->setText(QString::number(m_zoomFeature.zoom()));
}

//
// ZoomFeature
//

ZoomFeature::ZoomFeature()
  : Feature(std::make_unique<State>(*this))
{
}

ZoomFeature::~ZoomFeature()
{
}

const ZoomFeature::Signals& ZoomFeature::sigs() const
{
  if (!m_sigs)
    m_sigs.reset(new Signals);

  return *m_sigs;
}

unsigned int ZoomFeature::zoom() const
{
  return m_zoom;
}

void ZoomFeature::setZoom(unsigned int zoom, const std::optional<QPoint>& pos)
{
  assert(m_settings.min <= m_settings.max);
  if (zoom < m_settings.min)
    zoom = m_settings.min;
  else if (zoom > m_settings.max)
    zoom = m_settings.max;

  if (zoom == m_zoom)
    return;

  auto oldZoom = m_zoom;
  m_zoom = zoom;

  emit sigs().zoomChanged(m_zoom, pos, oldZoom);
}

void ZoomFeature::increaseZoom(int stepsCount, const std::optional<QPoint>& pos)
{
  auto newZoom = int(m_zoom) + stepsCount * int(m_settings.step);
  if (newZoom < 0)
    newZoom = 0;

  using uint = unsigned int;
  setZoom(uint(newZoom), pos);
}

void ZoomFeature::decreaseZoom(int stepsCount, const std::optional<QPoint>& pos)
{
  auto newZoom = int(m_zoom) - stepsCount * int(m_settings.step);
  if (newZoom < 0)
    newZoom = 0;

  using uint = unsigned int;
  setZoom(uint(newZoom), pos);
}

unsigned int ZoomFeature::minimalZoom() const
{
  return m_settings.min;
}

void ZoomFeature::setMinimalZoom(unsigned int min)
{
  assert(min <= m_settings.max);

  m_settings.min = min;

  if (m_zoom < m_settings.min)
    setZoom(m_settings.min);
}

unsigned int ZoomFeature::maximalZoom() const
{
  return m_settings.max;
}

void ZoomFeature::setMaximalZoom(unsigned int max)
{
  assert(max >= m_settings.min);

  m_settings.max = max;

  if (m_zoom > m_settings.max)
    setZoom(m_settings.max);
}

unsigned int ZoomFeature::zoomStep() const
{
  return m_settings.step;
}

void ZoomFeature::setZoomStep(unsigned int step)
{
  m_settings.step = step;
}

const ZoomFeature::Settings& ZoomFeature::settings() const
{
  return m_settings;
}

void ZoomFeature::setSettings(const Settings& settings)
{
  assert(settings.min <= settings.max);

  m_settings = settings;

  if (m_zoom < m_settings.min)
    setZoom(m_settings.min);
  else if (m_zoom > m_settings.max)
    setZoom(m_settings.max);
}

QAction* ZoomFeature::action()
{
  if (!m_action)
    m_action.reset(new Action(*this));

  return m_action.get();
}

}}} // namespace mad::utils::gui
