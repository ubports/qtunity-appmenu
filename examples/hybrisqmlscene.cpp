// Copyright © 2012 Canonical Ltd
// FIXME(loicm) Add copyright notice here.

#include "hybrisqmlscene.h"
#include <QtQuick>
#include <cstdio>
#include <ctime>
#include <csignal>

extern "C" void init_hybris();

Scene::Scene(QObject* parent)
    : QObject(parent)
    , frames_(0)
    , sum_(0.0)
    , min_(std::numeric_limits<double>::max())
    , max_(0.0) {
}

void Scene::beforeRendering() {
  clock_gettime(CLOCK_MONOTONIC, &t1_);
}

void Scene::afterRendering() {
  clock_gettime(CLOCK_MONOTONIC, &t2_);
  double time = ((t2_.tv_sec * 1000000000 + t2_.tv_nsec) -
                 (t1_.tv_sec * 1000000000 + t1_.tv_nsec)) / 1000000.0;
  if (time < min_)
    min_ = time;
  if (time > max_)
    max_ = time;
  sum_ += time;
  frames_++;
}

void Scene::getStats(int* frames, double* min, double* max, double* average) {
  *frames = frames_;
  *min = min_;
  *max = max_;
  *average = sum_ / frames_;
}

static void usage() {
  fprintf(stdout, "Usage: hybris-qmlscene <filename>\n");
}

static void logger(QtMsgType type, const char* msg) {
  Q_UNUSED(type);
  Q_UNUSED(msg);
}

static void signalHandler(int signal) {
  switch (signal) {
    case SIGINT:
    case SIGTERM: {
      QGuiApplication::quit();
      break;
    }
    default: {
      break;
    }
  }
}

int main(int argc, char* argv[]) {
  QQuickView* view;
  int exit_code;
  QUrl url;

  for (int i = 1; i < argc; ++i) {
    if (QFileInfo(QFile::decodeName(argv[i])).exists()) {
      url = QUrl::fromLocalFile(argv[i]);
    } else {
      const QString arg = QString::fromLatin1(argv[i]).toLower();
      if (arg == QLatin1String("--help") || arg == QLatin1String("-help") ||
          arg == QLatin1String("--h") || arg == QLatin1String("-h")) {
        usage();
        return 0;
      }
    }
  }
  if (url.isEmpty()) {
    usage();
    return 1;
  }

  // Swallow all the messages to avoid cluttering the standard output.
  qInstallMsgHandler(logger);

  // Ensure the libs are loaded and threading is all setup.
  init_hybris();

  signal(SIGINT, signalHandler);
  signal(SIGTERM, signalHandler);

  QGuiApplication app(argc, argv);
  app.setApplicationName("HybrisQmlScene");
  app.setOrganizationName("Canonical");
  app.setOrganizationDomain("canonical.com");

  view = new QQuickView();
  view->setColor(Qt::black);
  view->setWindowTitle("Hybris QML Scene");
  view->setResizeMode(QQuickView::SizeRootObjectToView);
  view->setSource(url);
  view->show();

  Scene scene;
  QObject::connect(view, SIGNAL(beforeRendering()), &scene, SLOT(beforeRendering()));
  QObject::connect(view, SIGNAL(afterRendering()), &scene, SLOT(afterRendering()));

  exit_code = app.exec();
  delete view;

  int frames;
  double min, max, average;
  scene.getStats(&frames, &min, &max, &average);
  fprintf(stdout, "Over %d frames\nmin: %.2lf ms\nmax: %.2lf ms\navg: %.2lf ms\n",
          frames, min, max, average);

  return exit_code;
}