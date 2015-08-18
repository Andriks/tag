#include "controller.h"

#include <cstdlib>
#include <cmath>
#include <QDebug>


AbstractModel::TagElement::TagElement(const QString &name, const QString &color, const QString &opacity) :
    name_(name),
    color_(color),
    opacity_(opacity)
{
}

QString AbstractModel::TagElement::name() const
{
    return name_;
}

QString AbstractModel::TagElement::color() const
{
    return color_;
}

QString AbstractModel::TagElement::opacity() const
{
    return opacity_;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

AbstractModel::AbstractModel(QObject *parent) :
    QAbstractListModel(parent),
    root_(parent),
    free_cell_(15)
{
    for (int i = 0; i < 16; i++) {
        data_list_.append(TagElement(QString("%1").arg(i+1), "lightgreen", (i == free_cell_ ? "0" : "1")));
    }
}


void AbstractModel::randomize()
{
    for (int i = 0; i < 500; i++) {
        int x = std::rand() % 16;
        int y = std::rand() % 16;

        moveFromTo(x, y);
    }
    for (int i = 0; i < 16; i++) {
        if (data_list_[i].name() == "16")
            free_cell_ = i;
    }
}

QPoint AbstractModel::getPoint(int p)
{
    int x = std::ceil(p / 4);
    int y = p - (x * 4);

    return QPoint(x, y);
}

bool AbstractModel::ableToMove(int from, int to)
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

bool AbstractModel::gameComplited()
{
    for (int i = 0; i < 16; i++) {
        if (data_list_[i].name() != QString("%1").arg(i+1))
            return false;
    }

    return true;
}

int AbstractModel::rowCount(const QModelIndex & parent) const {
    return data_list_.count();
}

QVariant AbstractModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() > data_list_.count())
        return QVariant();

    const TagElement &el = data_list_[index.row()];

    switch (role) {
    case NameRole:
        return el.name();
        break;
    case ColorRole:
        return el.color();
        break;
    case OpacityRole:
        return el.opacity();
        break;
    default:
        return QVariant();
        break;
    }
}


QHash<int, QByteArray> AbstractModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[ColorRole] = "color";
    roles[OpacityRole] = "opacity";
    return roles;
}


void AbstractModel::moveCell(int index)
{
    if (ableToMove(index, free_cell_)) {

        moveFromTo(index, free_cell_);

        free_cell_ = index;

        if (gameComplited()) {
            QObject *messageDialog = root_->findChild<QObject*>("messageDialog");
            QMetaObject::invokeMethod(messageDialog, "show", Q_ARG(QVariant, QVariant("Congratulation, you win!!")));
        }
    }
}

void AbstractModel::moveFromTo(int from, int to)
{
    if (from == to)
        return;

    int min = std::min(from, to);
    int max = std::max(from, to);

    if (max-min > 1) {
        beginMoveRows(QModelIndex(), max, max, QModelIndex(), min);
        data_list_.move(max, min);
        endMoveRows();

        beginMoveRows(QModelIndex(), min+1, min+1, QModelIndex(), max+1);
        data_list_.move(min+1, max);
        endMoveRows();
    } else {
        if (to > from) {
            beginMoveRows(QModelIndex(), min, min, QModelIndex(), max+1);
            data_list_.move(min, max);
        } else {
            beginMoveRows(QModelIndex(), max, max, QModelIndex(), min);
            data_list_.move(max, min);
        }

        endMoveRows();
    }
}

