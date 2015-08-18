#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QAbstractItemModel>
#include <QPoint>


/////////////////////////////////////////////////////////////////////////////////////

class AbstractModel : public QAbstractListModel
{
    Q_OBJECT

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

public slots:
    void moveCell(int);
    void randomize();

private:
    // for game logic
    QPoint getPoint(int);
    bool ableToMove(int, int);
    bool gameComplited();
    void moveFromTo(int, int);

private:
    QObject *root_;
    QList<TagElement> data_list_;
    int free_cell_;
};

#endif // CONTROLLER_H
