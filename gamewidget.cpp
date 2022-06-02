#include "gamewidget.h"
#include <QGridLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <vector>
#include <cmath>
#include <QMessageBox>


GameWidget::GameWidget(HighScore* highscore, QWidget *parent)
  : QWidget(parent),
    m_checkButton("Проверить!", this),
    m_table(0, 2, this),
    m_highscore(highscore) {

  setWindowTitle("Быки и коровы");

  QGridLayout *layout = new QGridLayout(this);
  this->setLayout(layout);

  auto startButton = new QPushButton("Новая игра", this);
  auto scoresButton = new QPushButton("Рекорды", this);

  m_numberInput.setValidator(new QIntValidator(this));

  m_table.setHorizontalHeaderLabels(QStringList({"Число", "Результат"}));
  m_table.horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  m_table.setEnabled(false);

  layout->addWidget(startButton, 0, 0);
  layout->addWidget(&m_statusMessage, 0, 1);
  layout->addWidget(scoresButton, 0, 2);

  layout->addWidget(new QLabel("введи число", this), 1, 0);
  layout->addWidget(&m_numberInput, 1, 1);
  layout->addWidget(&m_checkButton, 1, 2);

  layout->addWidget(&m_table, 2, 0, 1, 3);

  connect(startButton, SIGNAL(clicked()), SLOT(startGame()));
  connect(&m_checkButton, SIGNAL(clicked()), SLOT(check()));
  connect(scoresButton, SIGNAL(clicked()), SLOT(scores()));

  stopGame();

  srand(time(NULL));
}

void GameWidget::startGame() {
  m_statusMessage.setText("Игра запущена...");
  m_numberInput.setEnabled(true);
  m_checkButton.setEnabled(true);
  m_table.setEnabled(true);

  m_numberInput.clear();
  while (m_table.rowCount() > 0) {
    m_table.removeRow(0);
  }

  int l = 0;
  bool h = 0;
  std::vector<int> vec(4,-1);
  while(l < vec.size())
  {
      int k = rand()%10;
      bool h = 1;
      for(int i = 0; i < vec.size() ; i++)
      {
          if(vec[i]==k)
          {
              h=0;
          }
      }
      if(h == 1)
      {
          vec[l]=k;
          l++;
      }
  }

  QString strvalue;

  for(int i = 0; i < vec.size(); i++)
  {
      strvalue += QString::number(vec[i]) ;
  }

  //m_statusMessage.setText(strvalue);


  m_RandNumber = strvalue;
}

void GameWidget::scores() {
  bool visibility = m_highscore->isVisible();
  m_highscore->setVisible(!visibility);
}

void GameWidget::stopGame() {
  m_statusMessage.setText("Игра не начата");
  m_numberInput.setEnabled(false);
  m_checkButton.setEnabled(false);
  m_table.setEnabled(false);
}

void GameWidget::check() {
  QString value = m_numberInput.text();

  int ivalue = value.toInt();

//проверка на количество введенных цифр
  if (value.size() < 4 || value.size() > 4) {
    m_statusMessage.setText("Введите число");
    QMessageBox::about(this,"Уууупс... :(","Введённое число должно содержать 4 цифры");

    return;
  }

// проверка на различие введенных цифр
  int ivaluecop = ivalue;
  std::vector<int> vect(4);
  for(int i = vect.size()-1; i >=0; i--)
  {
      vect[i] = ivaluecop / pow(10 , i);
      ivaluecop -= vect[i] * pow(10 , i);
  }
  bool k = 0;
  for (int i = 0; i < vect.size() ; i++)
    {
        for (int j = 0; j < vect.size() ; j++)
        {
            if (i != j && vect[i] == vect[j])
            {
                k = 1;
                break;
            }
        }
    }
  if (k == 1)
  {
      QMessageBox::about(this,"Уууупс... :(","Введённое число должно содержать различные цифры");
      return;
  }

  if (value == m_RandNumber) {
    int score = m_table.rowCount();
    if (false == m_highscore->isTop(score))  {
      m_statusMessage.setText("Вы выиграли, но не попали в рейтинг!");
      QMessageBox::about(this,"Уууупс... :(","Вы выиграли, но не попали в рейтинг!");

    }
    else {
      bool bOk;
      QString name = QInputDialog::getText(0, "Input", "Name:", QLineEdit::Normal, "", &bOk);
      if (bOk) {
        // нажато Ok, а не Cancel
        m_highscore->add(score, name);
                this->stopGame();

      }
    }


    return;
  }

  m_statusMessage.setText("Не угадали, попробуйте еще...");

  m_table.insertRow(m_table.rowCount());
  m_table.setItem(m_table.rowCount()-1, 0, new QTableWidgetItem(value));

  int nBulls, nBows;
  calcBows(value, m_RandNumber, nBows, nBulls);

  QString result = "Быков: " + QString::number(nBulls) + "; Коров: " + QString::number(nBows);

  m_table.setItem(m_table.rowCount()-1, 1, new QTableWidgetItem(result));
}

void GameWidget::calcBows(QString a, QString b, int& nBows, int& nBulls) {
  nBulls = 0;
  nBows = 0;

  for (int i = 0; i < a.size(); ++i) {
    if (a[i] == b[i]) {
      nBulls++;
      b[i] = ' ';
      a[i] = ' ';
    }
  }

  for (int i = 0; i < a.size(); ++i) {
    if (a[i] == ' ')
      continue;

    int countLeft = countBetween(a[i], a, 0, i);
    int countInRandNumber = b.count(a[i]);

    if (countInRandNumber != 0 && countLeft <= countInRandNumber)
      nBows++;
  }
}

int GameWidget::countBetween(QChar what, QString where, int from, int to) {
  int count = 0;

  for (int i = from; i < to; ++i) {
    if (where[i] == what)
      count++;
  }

  return count;
}

