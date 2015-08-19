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
    edge_size_(4)
{
    int dim_size = edge_size_ * edge_size_;
    free_cell_ = dim_size - 1;

    for (int i = 0; i < dim_size; i++) {
        data_list_.push_back(TagElement(QString("%1").arg(i+1), (i == free_cell_ ? "0" : "1")));
    }
}


int AbstractModel::rowCount(const QModelIndex & parent) const {
    return data_list_.size();
}

QVariant AbstractModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() > data_list_.size())
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

int AbstractModel::getEdgeSize() const
{
    return edge_size_;
}

void AbstractModel::setEdgeSize(const int val)
{
    edge_size_ = val;
}


void AbstractModel::randomize()
{
    int dim_size = edge_size_ * edge_size_;

    for (int i = 0; i < edge_size_ * 500; i++) {
        int x = std::rand() % dim_size;
        int y = std::rand() % dim_size;

        if (ableToMove(x, y))
            moveFromTo(x, y);
    }

    for (int i = 0; i < dim_size; i++) {
        if (data_list_[i].name() == QString("%1").arg(dim_size)) {
            free_cell_ = i;
            break;
        }
    }
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
    int x = std::ceil(p / edge_size_);
    int y = p - (x * edge_size_);

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
    int dim_size = edge_size_ * edge_size_;

    for (int i = 0; i < dim_size; i++) {
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
    } else {
        int add_num = 0;

        if (from < to)
            add_num = 1;

        beginMoveRows(QModelIndex(), from, from, QModelIndex(), to + add_num);
        endMoveRows();

    }

    std::swap(data_list_[from], data_list_[to]);
}

