#include "controller.h"

#include <cmath>
#include <QDebug>


AbstractModel::TagElement::TagElement(const QString &name, const QString &opacity) :
    name_(name),
    opacity_(opacity)
{
}

QString AbstractModel::TagElement::name() const
{
    return name_;
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
        data_list_.append(TagElement(QString("%1").arg(i+1), (i == free_cell_ ? "0" : "1")));
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
        if (data_list_[i].name() == "16") {
            free_cell_ = i;
            break;
        }
    }
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


void AbstractModel::moveFromTo(int from, int to)
{
    if (from == to)
        return;

    if (std::abs(from - to) > 1) {
        int min = std::min(from, to);
        int max = std::max(from, to);

        beginMoveRows(QModelIndex(), max, max, QModelIndex(), min);
        endMoveRows();

        beginMoveRows(QModelIndex(), min+1, min+1, QModelIndex(), max+1);
        endMoveRows();

        data_list_.move(max, min);
        data_list_.move(min+1, max);
    } else {
        int add_nun = 0;

        if (from < to)
            add_nun = 1;

        beginMoveRows(QModelIndex(), from, from, QModelIndex(), to + add_nun);
        endMoveRows();

        data_list_.move(from, to);
    }
}

