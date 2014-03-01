/* Author: Jesse R. Hairston
 * Use Case: #18: Save Settings
 */

#include "databasecontroller.h"

DatabaseController::DatabaseController(QObject *parent) :
    QObject(parent) {
}

void DatabaseController::buildTables() {
    db = QSqlDatabase::addDatabase( "QSQLITE" );  //QMYSQL
    //for non-lab machines, use "pavelow.eng.uah.edu"
    db.setHostName("pavelow");              //host name
    db.setPort(3303);                       //port number
//    db.setUserName("team3");
//    db.setPassword("hroluba");
    db.setDatabaseName("PoD_database.db");   //database name

    db.open();
//    db.removeDatabase("PoD_database.db");
//    db.close();
    if (!db.isOpen()) {
        m_state = false;
        qDebug() << db.lastError() << "Error: unable to connect";
    }
    else {
        QSqlQuery build;
        build.exec( QString("CREATE TABLE save_table"
                            "(name TEXT,"
                            "clocks INTEGER,"
                            "score INTEGER,"
                            "coins INTEGER,"
                            "checkpoint INTEGER,"
                            "powerups INTEGER,"
                            "gametime INTEGER,"
                            "items INTEGER)"));
        m_state = true;
        qDebug() << "database built";
    }
}

void DatabaseController::readVals(QString what, QString where, QString modifier) {
    QSqlQuery qr("SELECT" + what + " FROM " + where + " WHERE " + modifier);

    if (!qr.isActive()) qDebug() << qr.lastError() << "error: unable to complete query";
    while(qr.next())
        qDebug() << "Name: " << qr.value(0).toString()
                 << "| Score: " << qr.value(2).toInt();
}

void DatabaseController::readVals(QString what, QString where) {
    QSqlQuery qr("SELECT " + what + " FROM " + where);

    if (!qr.isActive()) qDebug() << qr.lastError() << "error: unable to complete query";
    while(qr.next())
        qDebug() << "Name: " << qr.value(0).toString()
                 << "| Score: " << qr.value(2).toInt();
}

void DatabaseController::addVals(QString where, QString str1, QString str2, QString str3, int int1) {
    QSqlQuery qr("INSERT INTO " + where + " (" + str1 + ", " + str2 +
            ") VALUES ('" + str3 + "', " + QString::number(int1) + ")");
}

void DatabaseController::deleteVals(QString where, QString modifier) {
    QSqlQuery qr("DELETE FROM " + where + " WHERE " + modifier);
}

void DatabaseController::deleteTable() {
    QSqlQuery qdt("DELETE FROM save_table WHERE score > 0");
    qDebug() << "table deleted";
}



