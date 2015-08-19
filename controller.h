#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QAbstractItemModel>
#include <QPoint>
#include <vector>


/////////////////////////////////////////////////////////////////////////////////////

class AbstractModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int edgeSize READ getEdgeSize WRITE setEdgeSize NOTIFY edgeSizeChanged)

    ///////////////////////////////////////////////////////
    class TagCell
    {
    public:
        TagCell(const QString &, const QString &);

        QString name() const;
        QString opacity() const;

    private:
        QString name_;
        QString opacity_;
    };

    ///////////////////////////////////////////////////////
    enum TagElemRoles {
        NameRole = Qt::UserRole + 1,
        OpacityRole
    };

public:
    AbstractModel(QObject *parent = 0);

    // for QML Engine
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;

    int getEdgeSize() const;
    void setEdgeSize(const int);

public slots:
    void moveCell(const int);
    void randomize();

signals:
    void edgeSizeChanged();

private:
    // for game logic
    bool inRange(int);
    std::vector<QPoint> cellsToMove(QPoint);

    QPoint getPointFromIndex(const int);
    int getIndexFromPoint(const QPoint);

    bool ableToMove(const int, const int);
    bool gameComplited();

    void moveFromTo(const int, const int);
    void moveFromTo(const QPoint, const QPoint);

private:
    QObject *root_;
    std::vector<TagCell> data_list_;
    int free_cell_;
    int edge_size_;


};

#endif // CONTROLLER_H
