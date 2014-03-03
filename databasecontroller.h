#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include <QtSql>
#include <QDebug>
#include <QStringList>

#include <map>
#include <assert.h>

class DatabaseController {
public:
    static DatabaseController* Instance() {
        if (!m_singleton) {
            m_singleton = new DatabaseController;
        }
        return m_singleton;
    }

    void buildTables();
    void readVals(QString what, QString where);
    void readVals(QString what, QString where, QString modifier);
    void addVals(QString where, QString str1, QString str2, QString str3, QString int1);
    void addSave(std::vector<QString> names, std::vector<QString> values, QString table = QString("save_table"));
    void addOption(QString saveName, QString optionName, QString optionValue);
    void deleteVals(QString where, QString modifier);
    void deleteTable();

    virtual bool isOpen() { return m_state; }

private:
    DatabaseController();
    static DatabaseController *m_singleton;
    QSqlDatabase db;
    bool m_state;
};

#endif // DATABASECONTROLLER_H
