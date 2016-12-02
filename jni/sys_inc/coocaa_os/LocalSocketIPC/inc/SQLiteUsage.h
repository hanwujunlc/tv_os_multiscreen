#ifndef _SQLITE_USAGE_H_
#define _SQLITE_USAGE_H_

#include "CoocaaDef.h"
#include "sqlite3.h"
#include <string>
#include <thread>

NS_COOCAA_BEGIN

typedef int (*QueryCallback) (void *para, int n_column, char **column_value, char **column_name);  

typedef enum _SQLITE_DATATYPE  
{  
	//����
	SQLITE_DATATYPE_INTEGER = SQLITE_INTEGER,  
	//������
	SQLITE_DATATYPE_FLOAT  = SQLITE_FLOAT,  
	//�ַ���
	SQLITE_DATATYPE_TEXT  = SQLITE_TEXT,  
	//������
	SQLITE_DATATYPE_BLOB = SQLITE_BLOB,  
	//��
	SQLITE_DATATYPE_NULL= SQLITE_NULL,  
}SQLITE_DATATYPE;

class SQLiteUsage;

class SQLiteDataReader
{
public:
	SQLiteDataReader(sqlite3_stmt *pStmt);
	~SQLiteDataReader();

	// ��ȡһ������   
	bool read();  

	// �ر�Reader����ȡ���������   
	void close();  

	// �ܵ�����   
	int columnCount(void);  

	// ��ȡĳ�е�����    
	std::string getName(int nCol);  

	// ��ȡĳ�е���������   
	SQLITE_DATATYPE getDataType(int nCol);  

	// ��ȡĳ�е�ֵ(�ַ���)   
	std::string getStringValue(int nCol);  

	// ��ȡĳ�е�ֵ(����)   
	int getIntValue(int nCol);  

	// ��ȡĳ�е�ֵ(������)   
	long getInt64Value(int nCol);

	// ��ȡĳ�е�ֵ(������)   
	double getFloatValue(int nCol);  

	// ��ȡĳ�е�ֵ(����������)   
	const unsigned char* getBlobValue(int nCol, int &nLen);  

private: 
	sqlite3_stmt *m_pStmt;
};

class SQLiteCommand
{
public:
	SQLiteCommand(SQLiteUsage* pSqlite);
	SQLiteCommand(SQLiteUsage* pSqlite,std::string lpSql);  
	~SQLiteCommand();

	bool setCommandText(std::string lpSql); 
	// �󶨲�����indexΪҪ�󶨲�������ţ���1��ʼ��  
	bool bindParam(int index, std::string szValue);
	bool bindParam(int index, const int nValue);  
	bool bindParam(int index, const double dValue);  
	bool bindParam(int index, const unsigned char* blobValue, int nLen);  
	// ִ������  
	bool execute();
	// �����������ʹ��ʱ����øýӿ������  
	void clear();

private:
	SQLiteUsage *m_pSqlite;  
	sqlite3_stmt *m_pStmt;  
};


class SQLiteUsage
{
public:

	friend class SQLiteCommand;

	SQLiteUsage();
	~SQLiteUsage();

	bool open(std::string dbfile);

	void close();

	//ִ�зǲ�ѯ����
	bool execute(std::string sqlstr);
	bool execute(SQLiteCommand* pCmd);  

	SQLiteDataReader executeWithResult(std::string lpSql);  
	//�ص���ʽ
	bool executeWithResult(std::string lpSql,QueryCallback querycb); 

	//��ʼ���� 
	bool beginTransaction();
	//�ύ���� 
	bool commitTransaction();
	//�ع�����
	bool roolbackTransaction();

	std::string getLastErrorMsg();

private:  
	sqlite3 *m_db;  
};

NS_COOCAA_END

#endif//_SQLITE_USAGE_H_