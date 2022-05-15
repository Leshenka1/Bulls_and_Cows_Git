#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <map>
#include <QTableWidget>

class HighScore : public QWidget {
  Q_OBJECT
public:
  explicit HighScore(QString filename, QWidget *parent = nullptr);
  void add(int score, QString name);
  bool isTop(int score);
  void refill();
  QTableWidget m_scoreTable;

private:
  void save();
private:
  QString m_filename;
  std::multimap<int, QString, std::less<int>> m_highScore;
};

#endif // HIGHSCORE_H
