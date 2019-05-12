#pragma once

#include "../events/event.hpp"
#include "../events/mouseevent.hpp"
#include "../events/mousewheelevent.hpp"
#include "../events/resizeevent.hpp"
#include "feature.hpp"

#include <QCursor>
#include <QMouseEvent>
#include <QPainter>
#include <QWheelEvent>
#include <QWidget>

#include <assert.h>
#include <memory>
#include <type_traits>
#include <vector>

namespace mad { namespace utils { namespace gui {

template <typename BaseT, typename = std::enable_if_t<std::is_base_of<QWidget, BaseT>::value>>
class FeaturesAwareQWidget : public BaseT
{
public:
  explicit FeaturesAwareQWidget(QWidget* parent = nullptr)
    : BaseT(parent)
  {
    setMouseTracking(true);
  }

  void addFeature(std::unique_ptr<Feature>&& feature)
  {
    assert(feature);
    m_features.emplace_back(std::move(feature));
  }

  template <typename EventT, typename = std::enable_if_t<std::is_base_of<Event, EventT>::value>>
  void process(const EventT& event)
  {
    for (auto&& feature : m_features)
    {
      assert(feature);
      event.accept(feature->currentState());
    }
  }

protected:
  void paintEvent(QPaintEvent* ev) override
  {
    BaseT::paintEvent(ev);

    if (m_features.empty())
      return;

    QPainter painter(this);
    for (auto&& feature : m_features)
    {
      assert(feature);
      feature->currentState().paint(painter);
    }
  }

  void enterEvent(QEvent* ev) override
  {
    BaseT::enterEvent(ev);

    if (m_features.empty())
      return;

    MouseEvent event(MouseEvent::Enter, mapFromGlobal(QCursor::pos()));
    process(event);
    repaint();
  }

  void mouseMoveEvent(QMouseEvent* ev) override
  {
    BaseT::mouseMoveEvent(ev);

    if (m_features.empty())
      return;

    assert(ev);

    MouseEvent event(MouseEvent::Move, ev->pos());
    process(event);
    repaint();
  }

  void leaveEvent(QEvent* ev) override
  {
    BaseT::leaveEvent(ev);

    if (m_features.empty())
      return;

    MouseEvent event(MouseEvent::Leave, mapFromGlobal(QCursor::pos()));
    process(event);
    repaint();
  }

  void mousePressEvent(QMouseEvent* ev) override
  {
    BaseT::mousePressEvent(ev);

    if (m_features.empty())
      return;

    assert(ev);

    auto eventType = MouseEvent::LeftButtonPressed;
    switch (ev->button())
    {
    case Qt::LeftButton:
      break;

    case Qt::MiddleButton:
      eventType = MouseEvent::MiddleButtonPressed;
      break;

    case Qt::RightButton:
      eventType = MouseEvent::RightButtonPressed;
      break;

    default:
      return;
    }

    MouseEvent event(eventType, mapFromGlobal(QCursor::pos()));
    process(event);
    repaint();
  }

  void wheelEvent(QWheelEvent* ev) override
  {
    BaseT::wheelEvent(ev);

    if (m_features.empty())
      return;

    assert(ev);

    MouseWheelEvent::ScrollPhase scrollPhase = MouseWheelEvent::ScrollBegin;
    switch (ev->phase())
    {
    case Qt::ScrollBegin:
      break;

    case Qt::ScrollUpdate:
    case Qt::NoScrollPhase: // If scroll phase isn't supported just rely on stepsDelta being non-zero.
      scrollPhase = MouseWheelEvent::ScrollUpdate;
      break;

    case Qt::ScrollEnd:
      scrollPhase = MouseWheelEvent::ScrollEnd;
      break;

    default:
      return;
    }

    static const int angleStepSize = 15; // TODO Can we obtain this value somehow or do something else
                                         //      to support finer resolution mouse wheels or maybe touchpad
                                         //      gestures?

    // angleDelta() is in eights of degrees and one step is normally 15 degrees. See QWheelEvent::angleDelta()
    // documentation for details on the stepsDelta computation.
    auto stepsDelta = ev->angleDelta().y() / 8 / angleStepSize;
    if (stepsDelta != 0)
    {
      MouseWheelEvent event(scrollPhase, stepsDelta, mapFromGlobal(QCursor::pos()));

      auto modifiers = ev->modifiers();
      if (modifiers & Qt::ControlModifier)
        event.modifiers = InputEvent::Modifier(event.modifiers | InputEvent::CtrlModifier);
      if (modifiers & Qt::ShiftModifier)
        event.modifiers = InputEvent::Modifier(event.modifiers | InputEvent::ShiftModifier);
      if (modifiers & Qt::AltModifier)
        event.modifiers = InputEvent::Modifier(event.modifiers | InputEvent::AltModifier);

      process(event);
      repaint();
    }
  }

  void resizeEvent(QResizeEvent* ev) override
  {
    BaseT::resizeEvent(ev);

    if (m_features.empty())
      return;

    assert(ev);

    ResizeEvent event(ev->oldSize(), ev->size());
    process(event);
    // Note that the resize event is triggering repainting.
  }

protected:
  std::vector<std::unique_ptr<Feature>> m_features;
};

}}} // namespace mad::utils::gui
