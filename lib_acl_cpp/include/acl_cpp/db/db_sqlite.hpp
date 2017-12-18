#pragma once
#include "../acl_cpp_define.hpp"
#include "../stdlib/string.hpp"
#include "../db/db_handle.hpp"

typedef struct sqlite3 sqlite3;

namespace acl {

class charset_conv;

class ACL_CPP_API db_sqlite : public db_handle
{
public:
	/**
	 * ���캯��
	 * @param charset {const char*} �����ַ���(gbk, utf-8, ...)
	 */
	db_sqlite(const char* dbfile, const char* charset = "utf-8");
	~db_sqlite(void);

	/**
	 * ���ص�ǰ�� sqlite �İ汾��Ϣ
	 */
	const char* version(void) const;

	/**
	 * �����ݿ�򿪺�ͨ���˺��������ݿ�Ĳ�������������ã�
	 * �������õ�������Ҫ�ϸ���ѭ sqlite ���������ѡ��Ҫ��
	 * @param pragma {const char*} ����ѡ�����ݣ���ʽΪ��
	 *  PRAGMA xxx=xxx
	 *  �磺PRAGMA synchronous = NORMAL
	 * @return {bool} �������ݿ��Ƿ�ɹ�
	 */
	bool set_conf(const char* pragma);

	/**
	 * �����ݿ�򿪵��ô˺�������������������ѡ��
	 * @param pragma {const char*} ����ѡ�����ݣ���ʽΪ��
	 *  PRAGMA xxx
	 *  �磺PRAGMA synchronous
	 * @param out {string&} �������ֵ�ǿ���洢���
	 * @return {const char*} Ϊ����˵�������ò����ڻ����ݿ�δ��
	 */
	const char* get_conf(const char* pragma, string& out);

	/**
	 * �����ݿ�򿪵�������������ݿ����������ѡ��
	 * @param pragma {const char*} ָ��������ѡ�����ò���Ϊ�գ�
	 *  ��������е�����ѡ���ʽΪ��PRAGMA xxx���磺PRAGMA synchronous
	 */
	void show_conf(const char* pragma = NULL);

	/**
	 * �����ݿ�򿪺����е�Ӱ��ļ�¼����
	 * @return {int} Ӱ���������-1 ��ʾ����
	 */
	int affect_total_count(void) const;

	/**
	 * ֱ�ӻ�� sqlite �ľ����������� NULL ���ʾ sqlite ��û�д�
	 * �����ʱ�ڲ��Զ��ر��� sqlite
	 * @return {sqlite3*}
	 */
	sqlite3* get_conn(void) const
	{
		return db_;
	}

	/********************************************************************/
	/*            ����Ϊ���� db_handle ����ӿ�                         */
	/********************************************************************/

	/**
	 * @override
	 */
	const char* dbtype(void) const;

	/**
	 * @override
	 */
	int get_errno(void) const;

	/**
	 * @override
	 */
	const char* get_error(void) const;

	/**
	 * @override
	 */
	bool dbopen(const char* charset = NULL);

	/**
	 * @override
	 */
	bool is_opened(void) const;

	/**
	 * @override
	 */
	bool close(void);

	/**
	 * @override
	 */
	bool tbl_exists(const char* tbl_name);

	/**
	 * @override
	 */
	bool sql_select(const char* sql, db_rows* result = NULL);

	/**
	 * @override
	 */
	bool sql_update(const char* sql);

	/**
	 * @override
	 */
	int affect_count(void) const;

	/**
	 * @override
	 */
	bool begin_transaction(void);

	/**
	 * @override
	 */
	bool commit(void);

	/**
	 * @override
	 */
	bool set_busy_timeout(int nMillisecs);

private:
	// sqlite ����
	sqlite3* db_;

	// ���ݴ洢�ļ�
	string dbfile_;

	// �ַ���ת����
	charset_conv* conv_;

	// �����ַ���
	string charset_;

	// ����ִ��SQL��ѯ�ĺ���
	bool exec_sql(const char* sql, db_rows* result = NULL);
};

} // namespace acl
