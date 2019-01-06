#include <QDebug>
#include <QVector>
#include <QObject>

#define TOTAL_POINTS 157
#define MATCH 100

#define TEAM_1 0
#define TEAM_2 1
#define TOUR   2

#define NOT_COUNTED 0
#define COUNTED 1

#ifndef CHIBRECALCULATOR_H
#define CHIBRECALCULATOR_H

class ChibreCalculator : public QObject
{
    Q_OBJECT
    Q_ENUMS(Annonces)

public:
    explicit ChibreCalculator(QObject *parent = nullptr);

    enum class Annonces { CARTES3_20, CARTES4_50, CARTES4_100,
                            CARTES5_100, NEL_150, BUURG_200, STOCK_20 };

    void modifyScore(int nb_points, bool team);
    int otherScore(int nb_points);
    int getScores(bool team);
    void setScores(int score1, int score2, QVector<QVector<int>> annonces);
    QVector<QVector<int>> getScoreRepartition();
    QVector<QVector<double>> getResultsStats();

private:
    int scoreTeam_1;
    int scoreTeam_2;
    QVector<std::array<int, 2>> score_history;
    QVector<std::array<int, 2>> total_score_history;
    QVector<QVector<int>> annonces_history_team1;
    QVector<QVector<int>> annonces_history_team2;

    QVector<int> a_values = {0, 20, 50, 100, 100, 150, 200, 20};
};


#endif // CHIBRECALCULATOR_H
