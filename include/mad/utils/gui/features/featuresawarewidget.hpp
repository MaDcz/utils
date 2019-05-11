#pragma once

#include "../events/mouseevent.hpp"
#include "../events/mousewheelevent.hpp"
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
class FeaturesAwareWidget : public BaseT
{
public:
  explicit FeaturesAwareWidget(QWidget* parent = nullptr)
    : BaseT(parent)
  {
    setMouseTracking(true);
  }

  void addFeature(std::unique_ptr<Feature>&& feature)
  {
    assert(feature);
    m_features.emplace_back(std::move(feature));
  }

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
    for (auto&& feature : m_features)
    {
      assert(feature);
      event.accept(feature->currentState());
    }

    repaint();
  }

  void mouseMoveEvent(QMouseEvent* ev) override
  {
    BaseT::mouseMoveEvent(ev);

    if (m_features.empty())
      return;

    assert(ev);

    MouseEvent uiEvent(MouseEvent::Move, ev->pos());
    for (auto&& feature : m_features)
    {
      assert(feature);
      uiEvent.accept(feature->currentState());
    }

    repaint();
  }

  void leaveEvent(QEvent* ev) override
  {
    BaseT::leaveEvent(ev);

    if (m_features.empty())
      return;

    MouseEvent event(MouseEvent::Leave, mapFromGlobal(QCursor::pos()));
    for (auto&& feature : m_features)
    {
      assert(feature);
      event.accept(feature->currentState());
    }

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
    for (auto&& feature : m_features)
    {
      assert(feature);
      event.accept(feature->currentState());
    }

    repaint();
  }

  void wheelEvent(QWheelEvent* ev) override
  {
    BaseT::wheelEvent(ev);

    if (m_features.empty())
      return;

    assert(ev);

    MouseWheelEvent::ScrollPhase scrollPhase = MouseWheelEvent::ScrollBegin;
    auto qScrollPhase = ev->phase();
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

      for (auto&& feature : m_features)
      {
        assert(feature);
        event.accept(feature->currentState());
      }

      repaint();
    }
  }

protected:
  std::vector<std::unique_ptr<Feature>> m_features;
};

}}} // namespace mad::utils::gui
