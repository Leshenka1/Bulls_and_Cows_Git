#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include "highscore.h"

class GameWidget : public QWidget {
  Q_OBJECT
public:
  explicit GameWidget(HighScore* highscore, QWidget *parent = nullptr);

private slots:
  void startGame();
  void stopGame();
  void check();
  void calcBows(QString a, QString b, int& bows, int& bulls);
  void scores();
private:
  //! count in [from, to)
  int countBetween(QChar what, QString where, int from, int to);
private:
  QLineEdit m_numberInput;
  QLabel m_statusMessage;
  QPushButton m_checkButton;
  QTableWidget m_table;
  QString m_RandNumber;
  HighScore *m_highscore;
};

#endif // GAMEWIDGET_H
