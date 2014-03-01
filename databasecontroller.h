#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include <QObject>
#include <QtSql>
#include <QDebug>
#include <QStringList>

class DatabaseController : public QObject {
    Q_OBJECT
public:
    explicit DatabaseController(QObject *parent = 0);
    void buildTables();
    void readVals(QString what, QString where);
    void readVals(QString what, QString where, QString modifier);
    void addVals(QString where, QString str1, QString str2, QString str3, int int1);
    void deleteVals(QString where, QString modifier);
    void deleteTable();

    virtual bool isOpen() { m_state; }

private:
    QSqlDatabase db;
    bool m_state;

};

#endif // DATABASECONTROLLER_H
