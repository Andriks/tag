#include "controller.h"

#include <cstdlib>
#include <cmath>

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickItem>



Controller::Controller(QObject *parent) :
    QObject(parent),
    root_(parent),
    size_of_tag_(15),
    free_cell_(15)
{

}


void Controller::randomize()
{
    QObject *model = root_->findChild<QObject*>("dataModel");
    for (int i = 0; i < 500; i++) {
        int x = std::rand() % 16;
        int y = std::rand() % 16;
        int min = std::min(x, y);
        int max = std::max(x, y);

        QMetaObject::invokeMethod(model, "move", Q_ARG(int, min), Q_ARG(int, max), Q_ARG(int, 1));

        if (min == free_cell_) {
            free_cell_ = max;
            continue;
        }

        if (free_cell_ > min && free_cell_ <= max) {
            free_cell_--;
            continue;
        }


    }
//    for (int i = 0; i < 15; i++) {
//        // all cells except empty have theirs number as text
//        if (model.get(i).text)
//            free_cell_ = i;
//    }


}

QPoint Controller::getPoint(int p)
{
    int x = std::ceil(p / 4);
    int y = p - (x * 4);

    return QPoint(x, y);
}


bool Controller::able_to_move(int from, int to)
{
        QPoint p_from = getPoint(from);
        QPoint p_to = getPoint(to);


        if (QPoint(p_from.x(), p_from.y() - 1) == p_to)
            return true;

        if (QPoint(p_from.x(), p_from.y() + 1) == p_to)
            return true;

        if (QPoint(p_from.x() - 1, p_from.y()) == p_to)
            return true;

        if (QPoint(p_from.x() + 1, p_from.y()) == p_to)
            return true;


    return false;
}

void Controller::move_cell()
{
    QObject *model = root_->findChild<QObject*>("dataModel");
    QObject *view = root_->findChild<QObject*>("view");

    int index = view->property("currentIndex").toInt();
//    int index = model->property("index").toInt();

    if (able_to_move(index, free_cell_)) {
        int old_index = index;
        QMetaObject::invokeMethod(model, "move", Q_ARG(int, old_index), Q_ARG(int, free_cell_), Q_ARG(int, 1));

        if (old_index < free_cell_)
            QMetaObject::invokeMethod(model, "move", Q_ARG(int, free_cell_-1), Q_ARG(int, old_index), Q_ARG(int, 1));
        else
            QMetaObject::invokeMethod(model, "move", Q_ARG(int, free_cell_+1), Q_ARG(int, old_index), Q_ARG(int, 1));

        free_cell_ = old_index;

//        if (game_complited(view)) {
//            messageDialog.show("Congratulation, you win!!");
//        }
    }

}
