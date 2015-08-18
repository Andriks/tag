#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QPoint>
#include <QQmlApplicationEngine>
#include <QAbstractItemModel>
#include <QStringList>



/////////////////////////////////////////////////////////////////////////////////////

class AbstractModel : public QAbstractListModel
{
    Q_OBJECT

    class TagElement
    {
    public:
        TagElement(const QString &name, const QString &color, const QString &opacity);

        QString name() const;
        QString color() const;
        QString opacity() const;

    private:
        QString name_;
        QString color_;
        QString opacity_;
    };

public:
    enum AnimalRoles {
        NameRole = Qt::UserRole + 1,
        ColorRole,
        OpacityRole,
    };

    AbstractModel(QObject *parent = 0);

    QPoint getPoint(int);
    bool ableToMove(int, int);
    bool gameComplited();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;

public slots:
    void moveCell(int);
    void randomize();

private:
    void moveFromTo(int, int);

private:
    QObject *root_;
    QList<TagElement> data_list_;
    int free_cell_;
};

#endif // CONTROLLER_H
