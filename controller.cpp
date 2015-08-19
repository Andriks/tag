#include "controller.h"

#include <cmath>
#include <algorithm>
#include <QDebug>


AbstractModel::TagCell::TagCell(const QString &name, const QString &opacity) :
    name_(name),
    opacity_(opacity)
{
}

QString AbstractModel::TagCell::name() const
{
    return name_;
}

QString AbstractModel::TagCell::opacity() const
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
        data_list_.push_back(TagCell(QString("%1").arg(i+1), (i == free_cell_ ? "0" : "1")));
    }
}


int AbstractModel::rowCount(const QModelIndex & parent) const {
    return data_list_.size();
}


QVariant AbstractModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() > data_list_.size())
        return QVariant();

    const TagCell &el = data_list_[index.row()];

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
    for (int i = 0; i < edge_size_ * 500; i++) {
        // getting points near free cell (which able to move to free cell correct)
        QPoint p_to = getPointFromIndex(free_cell_);
        std::vector<QPoint> cells_to_move = cellsToMove(p_to);

        // random choose one of points
        int inx = std::rand() % cells_to_move.size();
        QPoint p_from = cells_to_move[inx];

        // moving (with saving tag in correct state)
        moveFromTo(p_from, p_to);
        free_cell_ = getIndexFromPoint(p_from);
    }
}

bool AbstractModel::inRange(int val)
{
    if (val < 0)
        return false;

    if (val >= edge_size_)
        return false;

    return true;
}

std::vector<QPoint> AbstractModel::cellsToMove(QPoint p)
{
    std::vector<QPoint> res;

    if (inRange(p.y() - 1))
        res.push_back(QPoint(p.x(), p.y() - 1));

    if (inRange(p.y() + 1))
        res.push_back(QPoint(p.x(), p.y() + 1));

    if (inRange(p.x() - 1))
        res.push_back(QPoint(p.x() - 1, p.y()));

    if (inRange(p.x() + 1))
        res.push_back(QPoint(p.x() + 1, p.y()));

    return res;
}


void AbstractModel::moveCell(const int index)
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


QPoint AbstractModel::getPointFromIndex(const int p)
{
    int x = std::ceil(p / edge_size_);
    int y = p - (x * edge_size_);

    return QPoint(x, y);
}

int AbstractModel::getIndexFromPoint(const QPoint p)
{
    int i = p.y() + (p.x() * edge_size_);
    return i;
}


bool AbstractModel::ableToMove(const int from, const int to)
{
    QPoint p_from = getPointFromIndex(from);
    QPoint p_to = getPointFromIndex(to);

    std::vector<QPoint> cells_to_move = cellsToMove(p_from);

    std::vector<QPoint>::iterator it = std::find(cells_to_move.begin(), cells_to_move.end(), p_to);

    if (it != cells_to_move.end())
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


void AbstractModel::moveFromTo(const int from, const int to)
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

void AbstractModel::moveFromTo(const QPoint from, const QPoint to)
{
    int i_from = getIndexFromPoint(from);
    int i_to = getIndexFromPoint(to);

    moveFromTo(i_from, i_to);
}

