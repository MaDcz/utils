#include "gui/features/zoomfeature.hpp"

#include "gui/events/mousewheelevent.hpp"

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

  auto newZoom = int(m_zoomFeature.m_zoom) + event.stepsDelta * int(m_zoomFeature.m_zoomStep);
  if (newZoom < 0)
    newZoom = 0;

  using uint = unsigned int;
  m_zoomFeature.setZoom(uint(newZoom), event.pos);
}

//
// ZoomFeature
//

ZoomFeature::ZoomFeature()
  : Feature(std::make_unique<State>(*this))
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
  assert(m_minZoom <= m_maxZoom);
  if (zoom < m_minZoom)
    zoom = m_minZoom;
  else if (zoom > m_maxZoom)
    zoom = m_maxZoom;

  if (zoom == m_zoom)
    return;

  auto oldZoom = m_zoom;
  m_zoom = zoom;

  emit sigs().zoomChanged(oldZoom, m_zoom, pos);
}

unsigned int ZoomFeature::zoomStep() const
{
  return m_zoomStep;
}

void ZoomFeature::setZoomStep(unsigned int zoomStep)
{
  m_zoomStep = zoomStep;
}

unsigned int ZoomFeature::minimalZoom() const
{
  return m_minZoom;
}

void ZoomFeature::setMinimalZoom(unsigned int minZoom)
{
  assert(minZoom <= m_maxZoom);

  m_minZoom = minZoom;

  if (m_zoom < m_minZoom)
    setZoom(m_minZoom);
}

unsigned int ZoomFeature::maximalZoom() const
{
  return m_maxZoom;
}

void ZoomFeature::setMaximalZoom(unsigned int maxZoom)
{
  assert(maxZoom >= m_minZoom);

  m_maxZoom = maxZoom;

  if (m_zoom > m_maxZoom)
    setZoom(m_maxZoom);
}

}}} // namespace mad::utils::gui
