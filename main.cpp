#include <QApplication>
#include "gamewidget.h"
#include "highscore.h"
#include <iostream>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  try {
    HighScore highscore("F:\\OiMP\\Bulls_and_Cows_1\\highscore.txt");

    GameWidget game(&highscore);
    game.show();

    return app.exec();
  }
  catch (...) {
    return -1;
  }
}
