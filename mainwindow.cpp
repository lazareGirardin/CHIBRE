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

#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent/*, Qt::FramelessWindowHint*/), ui(new Ui::MainWindow) {
  ui->setupUi(this);

    game = new ChibreCalculator;

    // game page
    ui->stackedWidget->setCurrentIndex(0);

    QColor color1 = QColor(23, 72, 232, 100);
    QColor color0 = QColor(0, 0, 255, 127);


    ui->progressBar_team1->setMaximum(1000);
    ui->progressBar_team2->setMaximum(1000);
    ui->progressBar_team1->setValue(0);
    ui->progressBar_team2->setValue(0);

    QString team1_name = QInputDialog::getText(parent,"Team 1","Enter first team name");
    QString team2_name = QInputDialog::getText(parent,"Team 2","Enter first team name");

    //ask for team name here!
    //team1_name = "Team 1";
    //team2_name = "Team 2";

    ui->label_team1->setText(team1_name);
    ui->label_team2->setText(team2_name);

    ui->annoncesgroup1->setTitle(team1_name);
    ui->annoncegroup2->setTitle(team2_name);

    series = new QPieSeries();
    series->setHoleSize(0.35);
    series->append("Annonces", 5);
    series->append("Game points", 5);

    QPieSlice *slice = series->slices().at(0);
    slice->setExploded();
    //slice->setLabelVisible();
    slice->setPen(QPen(QColor(0, 0, 0, 255),1));
    slice->setBrush(color0);

    series->slices().at(1)->setPen(QPen(QColor(0, 0, 0, 255),1));
    series->slices().at(1)->setBrush(color1);

    chart = new QChart();
    chart->addSeries(series);
    //chart->setTitle("Repartition des points");
    //chart->legend()->hide();

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    series2 = new QPieSeries();
    series2->setHoleSize(0.35);
    series2->append("Annonces", 5);
    series2->append("Game points", 5);

    QPieSlice *slice2 = series2->slices().at(0);
    slice2->setExploded();
    //slice2->setLabelVisible();
    slice2->setPen(QPen(QColor(0, 0, 0, 255),1));
    slice2->setBrush(color0);

    series2->slices().at(1)->setPen(QPen(QColor(0, 0, 0, 255),1));
    series2->slices().at(1)->setBrush(color1);

    chart2 = new QChart();
    chart2->addSeries(series2);
    //chart2->setTitle("Repartition des points");
    //chart2->legend()->hide();

    chartView2 = new QChartView(chart2);
    chartView->setRenderHint(QPainter::Antialiasing);

    chart->setBackgroundBrush(QColor(30, 30, 30, 127));
    chart2->setBackgroundBrush(QColor(30, 30, 30, 127));

    QVBoxLayout *lay = new QVBoxLayout(ui->frame_pie1);
    lay->addWidget(chartView);
    QVBoxLayout *lay2 = new QVBoxLayout(ui->frame_pie2);
    lay2->addWidget(chartView2);

    lay->addWidget(ui->annonces_points1);
    lay2->addWidget(ui->annonces_points2);

    tour = 0;
    ui->setScore_button->setEnabled(false);

    chart->setAcceptHoverEvents(true);
    chart2->setAcceptHoverEvents(true);

    connect(slice, &QPieSlice::hovered, this, &MainWindow::on_hover_slice);
    connect(slice2, &QPieSlice::hovered, this, &MainWindow::on_hover_slice);


}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::on_points_team1_returnPressed()
{
    if (ui->setScore_button->isEnabled())
        on_setScore_button_clicked();
}

void MainWindow::on_points_team2_returnPressed()
{
    if (ui->setScore_button->isEnabled())
        on_setScore_button_clicked();
}

void MainWindow::on_points_team1_textChanged(const QString &arg1)
{
    ui->setScore_button->setEnabled(true);
    if (arg1.isEmpty())
        ui->points_team2->setText("");
    else
    {
        int score_team2 = game->otherScore(arg1.toInt());

        if (score_team2 != -1)
            ui->points_team2->setText(QString::number(score_team2));
        else
            ui->setScore_button->setEnabled(false);
    }
}

void MainWindow::on_points_team2_textChanged(const QString &arg1)
{
    ui->setScore_button->setEnabled(true);
    if (arg1.isEmpty())
        ui->points_team1->setText("");
    else
    {
        int score_team1 = game->otherScore(arg1.toInt());

        if (score_team1 != -1)
            ui->points_team1->setText(QString::number(score_team1));
        else
            ui->setScore_button->setEnabled(false);
    }
}

void MainWindow::on_setScore_button_clicked()
{
    //retrieve the annonces checkboxes values
    const auto l_checkboxes = ui->a_value1->findChildren<QCheckBox *>();
    int i = 0;
    for (auto&& singlebox : l_checkboxes)
    {
        //retrieve checkbox status
        annonces_team1[i] = singlebox->isChecked();
        i++;
        //reset checkboxes
        singlebox->setChecked(false);
    }
    const auto l_checkboxes2 = ui->a_value2->findChildren<QCheckBox *>();
    i = 0;
    for (auto&& singlebox : l_checkboxes2)
    {
        annonces_team2[i] = singlebox->isChecked();
        i++;
        //reset checkboxes
        singlebox->setChecked(false);
    }
    // -------------------- COEFS -----------------------------------------
    const auto l_spinbox = ui->a_coef1->findChildren<QSpinBox *>();
    i = 0;
    for (auto&& spinbox : l_spinbox)
    {
        annonces_coef[i] = spinbox->value();
        i++;
        spinbox->setValue(1);
    }
    annonces_coef[4] = 1;
    annonces_coef[5] = 1;
    annonces_coef[6] = 1;

    const auto l_spinbox2 = ui->a_coef2->findChildren<QSpinBox *>();
    i = 0;
    for (auto&& spinbox : l_spinbox2)
    {
        annonces_coef2[i] = spinbox->value();
        i++;
        spinbox->setValue(1);
    }
    annonces_coef2[4] = 1;
    annonces_coef2[5] = 1;
    annonces_coef2[6] = 1;

    QVector<QVector<int>> results = check_annonces(annonces_team1, annonces_team2, annonces_coef, annonces_coef2);
    // add current score to the total ADD CONDITION TO ENSURE GOOD POINTS
    game->setScores(ui->points_team1->text().toInt(), ui->points_team2->text().toInt(),
                    results);

    // reset current score display
    ui->points_team1->setText("");
    ui->points_team2->setText("");

    //update score count
    int score1 = game->getScores(TEAM_1);
    int score2 = game->getScores(TEAM_2);

    // reached 1000 already?
    if (score1 >= 1000 || score2 >= 1000)
        game_completed();


    ui->scoreTeam1_label->setText(QString::number(score1));
    ui->scoreTeam2_label->setText(QString::number(score2));
    ui->progressBar_team1->setValue(score1);
    ui->progressBar_team2->setValue(score2);

    QVector<QVector<int>> repartition = game->getScoreRepartition();
    update_pieCharts(repartition);

    tour++;
    ui->tour_label->setText(QString::number(tour));

    ui->setScore_button->setEnabled(false);
}

//Pre-appened a TRUE before annonces if they counted
QVector<QVector<int>> MainWindow::check_annonces(int* a_team1, int* a_team2, int* a_coef1, int* a_coef2)
{
    QVector<QVector<int>> results;
    QVector<int> t_1;
    QVector<int> t_2;

    t_1 << 0;
    t_2 << 0;

    for (int i=5; i>=0; i--)
    {
        if (a_team1[i] && a_team2[i])
        {
            qDebug() << "egalite";
            QMessageBox msgBox;
            msgBox.setText(tr("Qui a l'annonce la plus haute?"));
            QAbstractButton* pButtonYes = msgBox.addButton(tr("team 1"), QMessageBox::YesRole);
            const QString temp = team1_name;
            //pButtonYes->setText(temp);
            QAbstractButton* pButtonNo = msgBox.addButton(tr("team 2"), QMessageBox::NoRole);
            //pButtonNo->setText(team2_name);

            msgBox.exec();
            /*
            QMessageBox msgBox;
            QAbstractButton* pButtonYes = msgBox.addButton(tr("team1_name"), QMessageBox::YesRole);
            QAbstractButton* pButtonNo = msgBox.addButton(tr("Nope"), QMessageBox::NoRole);
            msgBox.setButtonText(0, trUtf8("Sim"));
            msgBox.setButtonText(1, trUtf8("NÃ£o"));
            msgBox.exec();
            */

            if (msgBox.clickedButton()==pButtonYes) {
                t_1[0] = 1;
                t_2[0] = 0;
                break;
            }
            else
            {
                t_1[0] = 0;
                t_2[0] = 1;
                break;
            }
        }
        else if (a_team1[i])
        {
            t_1[0] = 1;
            t_2[0] = 0;
            qDebug() << "team one has the annonces";
            break;
        }
        else if (a_team2[i])
        {
            t_1[0] = 0;
            t_2[0] = 1;
            qDebug() << "team 2 has the annonces";
            break;
        }
    }

    for(int i=0; i<7; i++)
    {
        t_1 << a_team1[i]*a_coef1[i];
        t_2 << a_team2[i]*a_coef2[i];
    }
    //qDebug() << "t1 : " << t_1;
    //qDebug() << "t2 : " << t_2;

    results.append(t_1);
    results.append(t_2);

    return results;

}

void MainWindow::update_pieCharts(QVector<QVector<int>> repartition)
{
    series->slices().at(0)->setValue(repartition[TEAM_1][0]);
    series2->slices().at(0)->setValue(repartition[TEAM_2][0]);

    int score_1 = game->getScores(TEAM_1) - repartition[TEAM_1][0];
    int score_2 = game->getScores(TEAM_2) - repartition[TEAM_2][0];

    series->slices().at(1)->setValue(score_1);
    series2->slices().at(1)->setValue(score_2);
}

void MainWindow::game_completed()
{
    //victory page
    ui->stackedWidget->setCurrentIndex(1);

    QVector<QVector<double>> score_history = game->getResultsStats();

    create_graph_team1(score_history);
    create_graph_team2(score_history);
    double mean_team1_1=0;
    double mean_team1_2=0;
    double mean_team2_1=0;
    double mean_team2_2=0;
    int i=0;
    int count_1 = 0;
    int count_2 = 0;
    for(const auto &elem: score_history[TOUR])
    {
        if(i%2==0)
        {
            mean_team1_1+=score_history[TEAM_1][i];
            mean_team2_1+=score_history[TEAM_2][i];
            count_1++;
        }
        else
        {
           mean_team1_2+=score_history[TEAM_1][i];
           mean_team2_2+=score_history[TEAM_2][i];
           count_2++;
        }
        i++;
    }
    mean_team1_1 = mean_team1_1/count_1;
    mean_team1_2 = mean_team1_2/count_2;
    mean_team2_1 = mean_team2_1/count_1;
    mean_team2_2 = mean_team2_2/count_2;

    QString m_1;
    QString m_2;

    if (mean_team1_1 > mean_team1_2)
    {
        m_1 = "Mean Atout: " + QString::number(mean_team1_1) + ", (" + QString::number(count_1) + " fois)";
        m_2 = "Mean not Atout: " + QString::number(mean_team1_2) + ", (" + QString::number(count_2) + " fois)";

    }
    else
    {
        m_1 = "Mean Atout: " + QString::number(mean_team1_2) + ", (" + QString::number(count_2) + " fois)";
        m_2 = "Mean not Atout: " + QString::number(mean_team1_1) + ", (" + QString::number(count_1) + " fois)";
    }
    ui->label_mean_atout->setText(m_1);
    ui->label_mean_notatout->setText(m_2);

    if (mean_team2_1 > mean_team2_2)
    {
        m_1 = "Mean Atout: " + QString::number(mean_team2_1) + ", (" + QString::number(count_1) + " fois)";
        m_2 = "Mean not Atout: " + QString::number(mean_team2_2) + ", (" + QString::number(count_2) + " fois)";

    }
    else
    {
        m_1 = "Mean Atout: " + QString::number(mean_team2_2) + ", (" + QString::number(count_2) + " fois)";
        m_2 = "Mean not Atout: " + QString::number(mean_team2_1) + ", (" + QString::number(count_1) + " fois)";
    }

    ui->label_atout_2->setText(m_1);
    ui->label_notatout_2->setText(m_2);

    QVector<QVector<int>> repartition = game->getScoreRepartition();
    int a_1 = repartition[TEAM_1][0];
    int a_2 = repartition[TEAM_2][0];
    QString v = "Annonces: " + QString::number(a_1);
    QString v_2 = "Annonces: " + QString::number(a_2);
    ui->label_annonces->setText(v);
    ui->label_annonces_2->setText(v_2);

}

void MainWindow::on_modify_button_clicked()
{
    QDialog dialog(this);
    QFormLayout form(&dialog);

    form.addRow(new QLabel("What are the new scores?"));
    QLineEdit *new_score_team1 = new QLineEdit(&dialog);
    QLineEdit *new_score_team2 = new QLineEdit(&dialog);

    form.addRow("Team 1", new_score_team1);
    form.addRow("Team 2", new_score_team2);

    // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Show the dialog as modal
    if (dialog.exec() == QDialog::Accepted) {
        game->modifyScore(new_score_team1->text().toInt(), TEAM_1);
        game->modifyScore(new_score_team2->text().toInt(), TEAM_2);
    }

    // Display new changes
    // reset current score display
    ui->points_team1->setText("");
    ui->points_team2->setText("");

    //update score count
    int score1 = game->getScores(TEAM_1);
    int score2 = game->getScores(TEAM_2);

    // reached 1000 already?
    if (score1 >= 1000 || score2 >= 1000)
        game_completed();

    ui->scoreTeam1_label->setText(QString::number(score1));
    ui->scoreTeam2_label->setText(QString::number(score2));
    ui->progressBar_team1->setValue(score1);
    ui->progressBar_team2->setValue(score2);

    QVector<QVector<int>> repartition = game->getScoreRepartition();
    update_pieCharts(repartition);
}

void MainWindow::on_newGame_button_clicked()
{
    game->getResultsStats();
}

void MainWindow::on_hover_slice(bool state)
{
    if(state)
    {
        QVector<QVector<int>> repartition = game->getScoreRepartition();
        int a_1 = repartition[TEAM_1][0];
        int a_2 = repartition[TEAM_2][0];
        QString v = "Annonces: " + QString::number(a_1);
        QString v_2 = "Annonces: " + QString::number(a_2);
        ui->annonces_points1->setText(v);
        ui->annonces_points2->setText(v_2);
    }
    else
    {
        ui->annonces_points1->setText("");
        ui->annonces_points2->setText("");
    }
}

void MainWindow::create_graph_team1(QVector<QVector<double>> score_history)
{
    //QVector<double> zeros(score_history.size(), 0);
    QCPGraph *graph_score_1 = ui->victory_graph->addGraph();
    graph_score_1->setData(score_history[TOUR], score_history[TEAM_1]);
    graph_score_1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 12));
    graph_score_1->setPen(QPen(QColor(120, 120, 120), 3));

    QCPGraph *graph_total_score_1 = ui->victory_graph->addGraph();
    //graph_total_score_1->setLineStyle(QCPGraph::lsLine);
    graph_total_score_1->setData(score_history[TOUR], score_history[TS_TEAM1]);
    graph_total_score_1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 9));
    graph_total_score_1->setPen(QPen(QColor(120, 120, 120), 1));
    graph_total_score_1->setBrush(QBrush(QColor(200, 200, 200, 20)));

    // annonces comptée
    QCPBars *bars1 = new QCPBars(ui->victory_graph->xAxis, ui->victory_graph->yAxis);
    //bars1->setWidth(9/(double)score_history[TOUR].size());
    bars1->setData(score_history[TOUR], score_history[A_TEAM1_C]);
    bars1->setPen(Qt::NoPen);
    bars1->setBrush(QColor(10, 140, 70, 160));

    // annonces non-comptée
    QCPBars *bars2 = new QCPBars(ui->victory_graph->xAxis, ui->victory_graph->yAxis);
    //bars2->setWidth(9/(double)score_history[TOUR].size());
    bars2->setData(score_history[TOUR], score_history[A_TEAM1_NC]);
    bars2->setPen(Qt::NoPen);
    bars2->setBrush(QColor(10, 100, 50, 70));
    bars2->moveAbove(bars1);

    // move bars above graphs and grid below bars:
    ui->victory_graph->addLayer("abovemain", ui->victory_graph->layer("main"), QCustomPlot::limAbove);
    ui->victory_graph->addLayer("belowmain", ui->victory_graph->layer("main"), QCustomPlot::limBelow);
    graph_score_1->setLayer("abovemain");
    ui->victory_graph->xAxis->grid()->setLayer("belowmain");
    ui->victory_graph->yAxis->grid()->setLayer("belowmain");

    ui->victory_graph->xAxis->setLabel("Tour");
    ui->victory_graph->yAxis->setLabel("Points");

    // set some pens, brushes and backgrounds:
    ui->victory_graph->xAxis->setBasePen(QPen(Qt::white, 1));
    ui->victory_graph->yAxis->setBasePen(QPen(Qt::white, 1));
    ui->victory_graph->xAxis->setTickPen(QPen(Qt::white, 1));
    ui->victory_graph->yAxis->setTickPen(QPen(Qt::white, 1));
    ui->victory_graph->xAxis->setSubTickPen(QPen(Qt::white, 1));
    ui->victory_graph->yAxis->setSubTickPen(QPen(Qt::white, 1));
    ui->victory_graph->xAxis->setTickLabelColor(Qt::white);
    ui->victory_graph->yAxis->setTickLabelColor(Qt::white);
    ui->victory_graph->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    ui->victory_graph->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    ui->victory_graph->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    ui->victory_graph->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    ui->victory_graph->xAxis->grid()->setSubGridVisible(true);
    ui->victory_graph->yAxis->grid()->setSubGridVisible(true);
    ui->victory_graph->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    ui->victory_graph->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    ui->victory_graph->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    ui->victory_graph->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    ui->victory_graph->setBackground(plotGradient);
    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(80, 80, 80));
    axisRectGradient.setColorAt(1, QColor(30, 30, 30));
    ui->victory_graph->axisRect()->setBackground(axisRectGradient);

    ui->victory_graph->rescaleAxes();
    ui->victory_graph->replot();
}

void MainWindow::create_graph_team2(QVector<QVector<double>> score_history)
{
    QCPGraph *graph_score_1 = ui->victory_graph2->addGraph();
    graph_score_1->setData(score_history[TOUR], score_history[TEAM_2]);
    graph_score_1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 12));
    graph_score_1->setPen(QPen(QColor(120, 120, 120), 3));

    QCPGraph *graph_total_score_1 = ui->victory_graph2->addGraph();
    graph_total_score_1->setData(score_history[TOUR], score_history[TS_TEAM2]);
    graph_total_score_1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 9));
    graph_total_score_1->setPen(QPen(QColor(120, 120, 120), 1));
    graph_total_score_1->setBrush(QBrush(QColor(200, 200, 200, 20)));

    /*QCPGraph *graph2 = ui->victory_graph2->addGraph();
    graph2->setData(score_history[TOUR], score_history[TS_TEAM2]);
    graph2->setPen(Qt::NoPen);
    graph2->setBrush(QColor(200, 200, 200, 20));
    graph2->setChannelFillGraph(graph_score_1);*/

    // annonces comptée
    QCPBars *bars1 = new QCPBars(ui->victory_graph2->xAxis, ui->victory_graph2->yAxis);
    //bars1->setWidth(9/(double)score_history[TOUR].size());
    bars1->setData(score_history[TOUR], score_history[A_TEAM2_C]);
    bars1->setPen(Qt::NoPen);
    bars1->setBrush(QColor(10, 140, 70, 160));

    // annonces non-comptée
    QCPBars *bars2 = new QCPBars(ui->victory_graph2->xAxis, ui->victory_graph2->yAxis);
    //bars2->setWidth(9/(double)score_history[TOUR].size());
    bars2->setData(score_history[TOUR], score_history[A_TEAM2_NC]);
    bars2->setPen(Qt::NoPen);
    bars2->setBrush(QColor(10, 100, 50, 70));
    bars2->moveAbove(bars1);

    // move bars above graphs and grid below bars:
    ui->victory_graph2->addLayer("abovemain", ui->victory_graph2->layer("main"), QCustomPlot::limAbove);
    ui->victory_graph2->addLayer("belowmain", ui->victory_graph2->layer("main"), QCustomPlot::limBelow);
    graph_score_1->setLayer("abovemain");
    ui->victory_graph2->xAxis->grid()->setLayer("belowmain");
    ui->victory_graph2->yAxis->grid()->setLayer("belowmain");

    ui->victory_graph2->xAxis->setLabel("Tour");
    ui->victory_graph2->yAxis->setLabel("Points");

    // set some pens, brushes and backgrounds:
    ui->victory_graph2->xAxis->setBasePen(QPen(Qt::white, 1));
    ui->victory_graph2->yAxis->setBasePen(QPen(Qt::white, 1));
    ui->victory_graph2->xAxis->setTickPen(QPen(Qt::white, 1));
    ui->victory_graph2->yAxis->setTickPen(QPen(Qt::white, 1));
    ui->victory_graph2->xAxis->setSubTickPen(QPen(Qt::white, 1));
    ui->victory_graph2->yAxis->setSubTickPen(QPen(Qt::white, 1));
    ui->victory_graph2->xAxis->setTickLabelColor(Qt::white);
    ui->victory_graph2->yAxis->setTickLabelColor(Qt::white);
    ui->victory_graph2->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    ui->victory_graph2->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    ui->victory_graph2->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    ui->victory_graph2->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    ui->victory_graph2->xAxis->grid()->setSubGridVisible(true);
    ui->victory_graph2->yAxis->grid()->setSubGridVisible(true);
    ui->victory_graph2->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    ui->victory_graph2->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    ui->victory_graph2->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    ui->victory_graph2->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    ui->victory_graph2->setBackground(plotGradient);
    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(80, 80, 80));
    axisRectGradient.setColorAt(1, QColor(30, 30, 30));
    ui->victory_graph2->axisRect()->setBackground(axisRectGradient);

    ui->victory_graph2->rescaleAxes();
    ui->victory_graph2->replot();
}
