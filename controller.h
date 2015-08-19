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
    class TagElement
    {
    public:
        TagElement(const QString &, const QString &);

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
    void moveCell(int);
    void randomize();

signals:
    void edgeSizeChanged();

private:
    // for game logic
    QPoint getPoint(int);
    bool ableToMove(int, int);
    bool gameComplited();
    void moveFromTo(int, int);

private:
    QObject *root_;
//    QList<TagElement> data_list_;
    std::vector<TagElement> data_list_;
    int free_cell_;
    int edge_size_;


};

#endif // CONTROLLER_H
