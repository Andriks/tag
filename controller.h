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
    QPoint getPoint(int);
    bool able_to_move(int, int);
    void move_cell();


private:
    QObject *root_;
    size_t free_cell_;
    const size_t size_of_tag_;

};

#endif // CONTROLLER_H
