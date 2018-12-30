#include "chibrecalculator.h"

ChibreCalculator::ChibreCalculator(QObject *parent) : QObject(parent)
{
    scoreTeam_1 = 0;
    scoreTeam_2 = 0;
}

int ChibreCalculator::otherScore(int nb_points)
{
    int result = 0;
    if (nb_points == 0)
        result = TOTAL_POINTS + MATCH;
    else if (nb_points <= TOTAL_POINTS)
        result = TOTAL_POINTS - nb_points;
    else if (nb_points == TOTAL_POINTS + MATCH)
        result = 0;
    else
        result = -1;

    return result;
}

void ChibreCalculator::modifyScore(int new_points, bool team)
{
    // first team has bool 0
    if (team == TEAM_1)
        scoreTeam_1 = new_points;
    else if (team == TEAM_2)
        scoreTeam_2 = new_points;
    return;
}

int ChibreCalculator::getScores(bool team)
{
    int value = -1;
    if (team == TEAM_1)
        value = scoreTeam_1;
    else if (team == TEAM_2)
        value = scoreTeam_2;
    return value;
}

void ChibreCalculator::setScores(int score1, int score2, QVector<QVector<int>> annonces)
{
    // keep a history of the scores without annonces
    std::array<int, 2> scores = {score1, score2};
    score_history.append(scores);

    QVector<int> a_team1;
    QVector<int> a_team2;

    //if (annonces.size() == 2)
    //{
    //    annonces_history_team1.append(annonces[TEAM_1]);
    //    annonces_history_team2.append(annonces[TEAM_2]);
    //}
    //else
    //    qDebug() << "error in annonces size given to the chibre calculator";

    QVector<int> a_points = {0, 20, 50, 100, 100, 150, 200, 20};

    int winning_team = -1;
    if(annonces[TEAM_1][0])
        winning_team = TEAM_1;
    else if (annonces[TEAM_2][0])
        winning_team = TEAM_2;

    a_team1.append(annonces[TEAM_1][0]);
    a_team2.append(annonces[TEAM_2][0]);

    // add all annonces from the team winning them
    if(winning_team != -1)
    {
        for (int i = 1; i < 7; i++)
        {
            a_team1.append(annonces[TEAM_1][i]*a_points[i]);
            a_team2.append(annonces[TEAM_2][i]*a_points[i]);
           if(annonces[winning_team][i])
           {
               winning_team == TEAM_1 ? scoreTeam_1 += a_points[i] : scoreTeam_2 += a_points[i];
           }
        }
    }
    // count the Stock if any
    scoreTeam_1 += annonces[TEAM_1].last() * a_points.last();
    scoreTeam_2 += annonces[TEAM_2].last() * a_points.last();

    a_team1.append(annonces[TEAM_1].last() * a_points.last());
    a_team2.append(annonces[TEAM_2].last() * a_points.last());

    qDebug() << "annonces team1 : " << a_team1;
    qDebug() << "annonces team2 : " << a_team2;

    annonces_history_team1.append(a_team1);
    annonces_history_team2.append(a_team2);

    scoreTeam_1 += score1;
    scoreTeam_2 += score2;
}

QVector<QVector<int>> ChibreCalculator::getScoreRepartition()
{
    QVector<QVector<int>> result;
    QVector<int> temp;
    int sum_annonces_counted = 0;
    int sum_annonces_notcounted = 0;

    // ---------------- TEAM 1 ----------------------------------------
    for (const auto &elem : annonces_history_team1)
    {
        // has won the annonce
        if(elem.at(0)== 1)
        {
            // count the sum of the vector, remove 1 because of the first element beeing a bool indicating if the annonced counted
            sum_annonces_counted = std::accumulate(elem.begin(), elem.end(), sum_annonces_counted) -1;
        }
        else
        {
            // if there was a Stock
            sum_annonces_counted += elem.last();
            //qDebug() << "they lost the annonce, last element is: " << elem.last();
            sum_annonces_notcounted = std::accumulate(elem.begin(), elem.end(), sum_annonces_notcounted) - elem.last();
            //qDebug() << "and " << sum_annonces_notcounted << " was not counted";
        }
    }
    //qDebug() << "counted for team 1: " << sum_annonces_counted;
    //qDebug() << "not counted for team 1" << sum_annonces_notcounted;

    temp << sum_annonces_counted << sum_annonces_notcounted;
    result.append(temp);

    // ---------------- TEAM 2 ----------------------------------------
    sum_annonces_counted = 0;
    sum_annonces_notcounted = 0;
    temp.clear();

    for (const auto &elem : annonces_history_team2)
    {
        // has won the annonce
        if(elem.at(0)== 1)
        {
            // count the sum of the vector, remove 1 because of the first element beeing a bool indicating if the annonced counted
            sum_annonces_counted = std::accumulate(elem.begin(), elem.end(), sum_annonces_counted) -1;
        }
        else
        {
            // if there was a Stock
            sum_annonces_counted += elem.last();
            //qDebug() << "they lost the annonce, last element is: " << elem.last();
            sum_annonces_notcounted = std::accumulate(elem.begin(), elem.end(), sum_annonces_notcounted) - elem.last();
            //qDebug() << "and " << sum_annonces_notcounted << " was not counted";
        }
    }

    temp << sum_annonces_counted << sum_annonces_notcounted;
    result.append(temp);

    return result;
}
