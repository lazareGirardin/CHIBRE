#ifndef POINTHANDLER_H
#define POINTHANDLER_H

#include <QObject>

class PointHandler : public QObject
{
    Q_OBJECT
    Q_ENUMS(Annonces)
public:
    explicit PointHandler(QObject *parent = nullptr);

    enum class Annonces { CARTES3_20, CARTES4_50, CARTES4_100,
                    CARTES5_100, NEL_150, BUURG_200, STOCK_20 };
signals:

public slots:
};

#endif // POINTHANDLER_H
