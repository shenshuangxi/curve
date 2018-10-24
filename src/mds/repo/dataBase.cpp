/*
 * Project: curve
 * Created Date: Mon Sep 10 2018
 * Author: lixiaocui
 * Copyright (c) 2018 netease
 */

#include <glog/logging.h>
#include "src/mds/repo/dataBase.h"

namespace curve {
namespace repo {
DataBase::DataBase(const std::string &user,
                   const std::string &url,
                   const std::string &password) {
    this->url_ = url;
    this->user_ = user;
    this->password_ = password;
}

DataBase::~DataBase() {
    delete statement_;
    delete conn_;
}

int DataBase::connectDB() {
    try {
        sql::Driver *driver;
        driver = get_driver_instance();
        conn_ = driver->connect(url_, user_, password_);
        statement_ = conn_->createStatement();
        return OperationOK;
    } catch (sql::SQLException &e) {
        LOG(ERROR) << "connect fail, "
                   << "error code: " << e.getErrorCode() << ", "
                   << "error message: " << e.what();
        return SqlException;
    } catch (std::runtime_error &e) {
        LOG(ERROR) << "[dataBase.cpp] connect db get runtime_error, "
                   << "error message: " << e.what();
        return RuntimeExecption;
    }
}

int DataBase::Exec(const std::string &sql) {
    try {
        statement_->execute(sql::SQLString(sql));
        return OperationOK;
    } catch (sql::SQLException &e) {
        LOG(ERROR) << "exec sql: " << sql << "get sqlException, "
                   << "error code: " << e.getErrorCode() << ", "
                   << "error message: " << e.what();
        return SqlException;
    } catch (std::runtime_error &e) {
        LOG(ERROR) << "[dataBase.cpp] exec sql: " << sql
                   << "get runtime_error, "
                   << "error message: " << e.what();
        return RuntimeExecption;
    }
}

// retrun value: rows affected
int DataBase::ExecUpdate(const std::string &sql) {
    try {
        statement_->executeUpdate(sql);
        return OperationOK;
    } catch (sql::SQLException &e) {
        LOG(ERROR) << "execUpdate sql: " << sql << "get sqlException, "
                   << "error code: " << e.getErrorCode() << ", "
                   << "error message: " << e.what();
        return SqlException;
    } catch (std::runtime_error &e) {
        LOG(ERROR) << "execUpdate sql: " << sql << "get runtime_error, "
                   << "error message: " << e.what();
        return RuntimeExecption;
    }
}

// retrun queryResult
int DataBase::QueryRows(const std::string &sql, sql::ResultSet **res) {
    assert(res != nullptr);

    try {
        *res = statement_->executeQuery(sql);
        return OperationOK;
    } catch (sql::SQLException &e) {
        LOG(ERROR) << "queryRows sql: " << sql << "get sqlException, "
                   << "error code: " << e.getErrorCode() << ", "
                   << "error message: " << e.what();
        return SqlException;
    } catch (std::runtime_error &e) {
        LOG(ERROR) << "queryRows sql: " << sql << "get runtime_error, "
                   << "error message: " << e.what();
        return RuntimeExecption;
    }
}

int DataBase::SetAutoCommit(const bool &autoAommit) {
    try {
        conn_->setAutoCommit(autoAommit);
        return OperationOK;
    } catch (sql::SQLException &e) {
        LOG(ERROR) << "set AutoCommit " << autoAommit << " get sqlException, "
                   << "error code: " << e.getErrorCode() << ", "
                   << "error message: " << e.what();
        return SqlException;
    } catch (std::runtime_error &e) {
        LOG(ERROR) << "set AutoCommit " << autoAommit << " get runtime_error, "
                   << "error message: " << e.what();
        return RuntimeExecption;
    }
}

int DataBase::Commit() {
    try {
        conn_->commit();
        return OperationOK;
    } catch (sql::SQLException &e) {
        LOG(ERROR) << "commit operations get sqlException, "
                   << "error code: " << e.getErrorCode() << ", "
                   << "error message: " << e.what();
        return SqlException;
    } catch (std::runtime_error &e) {
        LOG(ERROR) << "commit operations get runtime_error, "
                   << "error message: " << e.what();
        return RuntimeExecption;
    }
}

int DataBase::RollBack() {
    try {
        conn_->rollback();
        return OperationOK;
    } catch (sql::SQLException &e) {
        LOG(ERROR) << "rollback get sqlException, "
                   << "error code: " << e.getErrorCode() << ", "
                   << "error message: " << e.what();
        return SqlException;
    } catch (std::runtime_error &e) {
        LOG(ERROR) << "rollback get runtime_error, "
                   << "error message: " << e.what();
        return RuntimeExecption;
    }
}
}  // namespace repo
}  // namespace curve
