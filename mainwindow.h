/*
###############################################################################
#                                                                             #
# The MIT License                                                             #
#                                                                             #
# Copyright (C) 2017 by Juergen Skrotzky (JorgenVikingGod@gmail.com)          #
#               >> https://github.com/Jorgen-VikingGod                        #
#                                                                             #
# Sources: https://github.com/Jorgen-VikingGod/Qt-Frameless-Window-DarkStyle  #
#                                                                             #
###############################################################################
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QVector>
#include <QMessageBox>
#include <QtCharts>
#include "chibrecalculator.h"

#define TEAM_1      0
#define TEAM_2      1
#define TOUR        2
#define A_TEAM1_C   3
#define A_TEAM2_C   4
#define A_TEAM1_NC  5
#define A_TEAM2_NC  6
#define TS_TEAM1    7
#define TS_TEAM2    8

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();


private slots:
    void on_points_team1_returnPressed();

    void on_points_team1_textChanged(const QString &arg1);

    void on_points_team2_textChanged(const QString &arg1);

    void on_setScore_button_clicked();

    void update_pieCharts(QVector<QVector<int>> repartition);

    void on_modify_button_clicked();

    void on_points_team2_returnPressed();

    void on_newGame_button_clicked();

    void on_hover_slice(bool state);

private:
  Ui::MainWindow *ui;
  ChibreCalculator *game;

  QVector<QVector<int>> check_annonces(int* a_team1, int* a_team2, int* a_coef1, int* a_coef2);
  void game_completed();

  void create_graph_team1(QVector<QVector<double>>);
  void create_graph_team2(QVector<QVector<double>>);

  QString team1_name;
  QString team2_name;

  int annonces_team1[7];
  int annonces_team2[7];

  int annonces_coef[7];
  int annonces_coef2[7];

  // pie charts handles
  QPieSeries *series;
  QPieSeries *series2;
  QChart *chart;
  QChart *chart2;
  QChartView *chartView;
  QChartView *chartView2;

  int tour;
};


#endif // MAINWINDOW_H
