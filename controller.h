#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QPoint>

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = 0);

signals:

public slots:
    void randomize();
    void move_cell();

private:
    bool game_complited();
    QPoint getPoint(int);
    bool able_to_move(int, int);
    void toMe();
private:
    QObject *root_;
    size_t free_cell_;

};

#endif // CONTROLLER_H
