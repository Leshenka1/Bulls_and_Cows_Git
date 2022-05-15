#include "highscore.h"
#include <QHeaderView>
#include <QGridLayout>

HighScore::HighScore(QString filename, QWidget *parent)
  : QWidget(parent), m_filename(filename), m_scoreTable(0, 2, this) {

  setWindowTitle("Рекорды");
  QFile file(m_filename);
  if (true == file.open(QIODevice::ReadOnly)) {
    QTextStream fileStream(&file);

    QString name;
    int score;
    while (true) {
      name = fileStream.readLine();
      if (name.isEmpty())
        break;

      fileStream >> score;
      fileStream.readLine();
      m_highScore.insert(std::make_pair(score, name));
    }
  }

  QGridLayout *layout = new QGridLayout(this);
  setLayout(layout);

  m_scoreTable.setHorizontalHeaderLabels(QStringList({"Попыток", "Имя"}));
  m_scoreTable.horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  m_scoreTable.setEnabled(true);

  m_scoreTable.setEditTriggers(0);
  m_scoreTable.setSelectionMode(QAbstractItemView::SelectionMode(0));

  layout->addWidget(&m_scoreTable);

  refill();
}

void HighScore::add(int score, QString name) {
  if (isTop(score)) {

    m_highScore.insert(std::make_pair(score, name));
    save();
    refill();
  }
}

void HighScore::refill() {
  while (m_scoreTable.rowCount() > 0) {
    m_scoreTable.removeRow(0);
  }

  int i = 0;
  for (auto it = m_highScore.begin(); it != m_highScore.end(); ++it) {
    if (i > 10)
      return;

    auto size = m_scoreTable.rowCount();

    m_scoreTable.insertRow(size);
    m_scoreTable.setItem(size, 0, new QTableWidgetItem(QString::number(it->first)));
    m_scoreTable.setItem(size, 1, new QTableWidgetItem(it->second));

    ++i;
  }
}

void HighScore::save() {
  QFile file(m_filename);
  if (false == file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
    throw std::exception("can't open file");
  }

  QTextStream fileStream(&file);

  for (auto it = m_highScore.begin(); it != m_highScore.end(); ++it) {
    fileStream << it->second << endl << it->first << endl;
  }
}

bool HighScore::isTop(int score) {
  int i = 0;
  for (auto it = m_highScore.begin(); it != m_highScore.end(); ++it) {
    if (i > 10)
      return false;
    if (it->first > score)
      return true;
    ++i;
  }
  return true;
}
