// Copyright © 2012 Canonical Ltd
// FIXME(loicm) Add copyright notice here.

#ifndef QHYBRISWINDOW_H
#define QHYBRISWINDOW_H

#include "qhybrisintegration.h"
#include "qhybrisscreen.h"
#include <qpa/qplatformwindow.h>
#include <QtWidgets/QWidget>

class QHybrisWindow : public QPlatformWindow {
 public:
  QHybrisWindow(QWindow* w);

  void setGeometry(const QRect&);
  WId winId() const;

  // FIXME(loicm) Add opacity and stacking support.
  // void setOpacity(qreal level);
  // void raise();
  // void lower();

 private:
  WId m_winId;
};

#endif  // QHYBRISWINDOW_H
