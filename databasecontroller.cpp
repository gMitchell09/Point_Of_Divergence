/* Author: Jesse R. Hairston
 * Use Case: #18: Save Settings
 */

#include "databasecontroller.h"

DatabaseController* DatabaseController::m_singleton = 0;

DatabaseController::DatabaseController() {
    this->buildTables();
}

void DatabaseController::buildTables() {
    //for non-lab machines, use "pavelow.eng.uah.edu"
    // No.  For ALL machines use pavelow.eng.uah.edu.
    //  The lab machines are a special case.

    /* DB Params:
     * Server flavor: sqlite
     * host name: pavelow
     * port number: 3303
     * database name: PoD_database.db
     */
    db = QSqlDatabase::addDatabase( "QMYSQL" );
    db.setHostName("pavelow.eng.uah.edu");
    db.setPort(3303);
    db.setUserName("team3");
//    db.setPassword("password");
    db.setDatabaseName("PoD_database");

    if (!db.open()) {
        m_state = false;
        qDebug() << db.lastError() << "Error: unable to connect";
    }
    else {
        QSqlQuery build;

        build.exec("SELECT * FROM save_table");
        if (!build.isActive()) {
            qDebug() << "Save Table doesn't exist ... creating";

            build.exec( QString("CREATE TABLE save_table"
                                "(name TEXT,"
                                "clocks INTEGER,"
                                "score INTEGER,"
                                "coins INTEGER,"
                                "checkpoint INTEGER,"
                                "powerups INTEGER,"
                                "gametime INTEGER,"
                                "items INTEGER)"));
        } else {
            qDebug() << "save_table already exists... contains: ";
            while(build.next()) {
                qDebug() << build.value(0).toString();
            }
        }
        build.exec("SELECT * FROM options_table");
        if (!build.isActive()) {
            build.exec( QString("CREATE TABLE options_table"
                                "(name TEXT,"
                                "sound BOOLEAN"
                                "music BOOLEAN)"));
            m_state = true;
            qDebug() << "options database built";
        } else {
            qDebug() << "options_table already exists... contains: ";
            while(build.next()) {
                qDebug() << build.value(0).toString();
            }
        }
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

void DatabaseController::addVals(QString where, QString str1, QString str2, QString str3, QString int1) {
    QSqlQuery test("SELECT * FROM " + where + " WHERE name='" + str1 + "'");
    if (test.size() <= 0) {
        QSqlQuery qr("INSERT INTO " + where + " (" + str1 + ", " + str2 +
                ") VALUES ('" + str3 + "', " + int1 + ")");
    } else {
        QSqlQuery qr("UPDATE (" + str1 + ", " + str2 + ") FROM " + where + " SET "+str2+"='"+int1+"'");
    }
}

/*
 INSERT INTO table (name1, name2, name3, name4) VALUES (val1, val2, val3)
*/

void DatabaseController::addSave(std::vector<QString> names, std::vector<QString> values, QString table) {
    QString strNames, strValues;


    for (int i=0; i < names.size(); i++) {
        strNames += names.at(i);
        if (i != names.size() - 1) {
            strNames += ", ";
        }
    }

    for (int i=0; i < values.size(); i++) {
        strValues += "'" + values.at(i) + "'";
        if (i != values.size() - 1) {
            strValues += ", ";
        }
    }

    assert(names.at(0) == "name");
    QSqlQuery test("SELECT * FROM "+ table +" WHERE name='" + values.at(0) + "'");
    if (test.size() <= 0) {
        QSqlQuery qr("INSERT INTO "+table+" ("+strNames+") VALUES (" + strValues + ")");
    } else {
        QSqlQuery qr("UPDATE (" + strNames + ") FROM "+table+" SET " + strValues + ")");
    }

    if(db.isOpenError()) {
        qDebug() << "FUCK";
    }
}

void DatabaseController::addOption(QString saveName, QString optionName, QString optionValue) {
    QSqlQuery test("SELECT * FROM options_table WHERE name=" + saveName);
    if (test.size() <= 0) {
        qDebug() << "Name doesn't exist in DB... adding: ";
        QSqlQuery qr("INSERT INTO option_table (name, value) VALUES ('"+optionName+"', '"+optionValue+"')");
    } else {
        qDebug() << "Name exists, updating";
        QSqlQuery qr("UPDATE name, value FROM options_table WHERE name="+optionName+" SET value=" + optionValue);
    }
}

void DatabaseController::deleteVals(QString where, QString modifier) {
    QSqlQuery qr("DELETE FROM " + where + " WHERE " + modifier);
}

void DatabaseController::deleteTable() {
    //QSqlQuery qdt("DELETE FROM save_table WHERE score > 0");
    //qDebug() << "table deleted";
}



