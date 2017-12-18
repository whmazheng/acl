#pragma once
#include <memory>
#include "acl_cpp/db/db_mysql.hpp"
#include "lib_acl_cpp1z/reflection/reflection.hpp"
namespace acl
{
	namespace lz
	{
		
		class db_mysql : public acl::db_mysql
		{
		public:
			struct date
			{
				time_t value;
			};
		public:
			/**
			* ���캯����ʽһ
			* @param dbaddr {const char*} ���ݿ������ַ������Ϊ TCP �׽ӿڻ��� UNIX
			*  ƽ̨�µ����׽ӿڣ���ʽ�磺127.0.0.1:3306���� /tmp/mysql.sock
			* @param dbname {const char*} ���ݿ����ƣ��� NULL
			* @param dbuser {const char*} �������ݿ�ʱ���û���
			* @param dbpass {const char*} �������ݿ�ʱ���û�����
			* @param dbflags {unsigned long} ���� MYSQL ʱ�ı�־λ
			* @param auto_commit {bool} �������ݿ�����޸�ʱ�Ƿ��Զ��ύ����
			* @param conn_timeout {int} �������ݿ�ĳ�ʱʱ�䣨�룩
			* @param rw_timeout {int} �������ݿ����ʱ�ĳ�ʱʱ�䣨�룩
			* @param charset {const char*} �������ݿ�ʱ�ı����ַ�����gbk, utf8, ...��
			*/

			db_mysql(const char* dbaddr, const char* dbname,
				const char* dbuser, const char* dbpass,
				unsigned long dbflags = 0, bool auto_commit = true,
				int conn_timeout = 60, int rw_timeout = 60,
				const char* charset = "utf8")
				:acl::db_mysql(
					dbaddr, 
					dbname, 
					dbuser, 
					dbpass, 
					dbflags, 
					auto_commit, 
					conn_timeout, 
					rw_timeout, 
					charset)
			{

			}

			/**
			* ���캯����ʽ����ʹ�ò��������������й���
			* @param conf {const mysql_conf&} mysql ���ݿ��������������
			*/
			db_mysql(const mysql_conf& conf)
				:acl::db_mysql(conf)
			{

			}

			template<typename T, typename... Args>
			std::enable_if_t<(sizeof...(Args) == 0), std::pair<bool, std::vector<T>>> 
				select(const char *sql)
			{
				std::vector<T> v;

				if (!sql_select(sql))
					return{ false, {} };
				for (size_t i = 0; i < length(); i++)
				{
					T t;
					const acl::db_row* row = (*this)[i];
					for_each(t, [&](auto& item, size_t I, bool is_last)
					{
						const char * row_name = get_name<T>(I);
						const char * value = (*row)[row_name];
						if (!value)
							return;
						assign(item, value);
					});
					v.push_back(t);
				}
				return{ true, v };
			}

			template<typename T, typename... Args>
			std::enable_if_t<(sizeof...(Args) == 0), bool>
				select(const char *sql, std::string &json_str)
			{
				if (!sql_select(sql))
					return false;
				json_str.push_back('[');

				for (size_t i = 0; i < length(); i++)
				{
					T t;
					json_str.push_back('{');
					const acl::db_row* row = (*this)[i];
					for_each(t, [&](auto& item, size_t I, bool is_last)
					{
						const char * row_name = get_name<T>(I);
						const char * value = (*row)[row_name];
						json_str.append(row_name);
						json_str.append(":");
						append_x(item, json_str);
						if (value)
							json_str.append(value);
						append_x(item, json_str);
						if (!is_last)
							json_str.push_back(',');

					});
					json_str.push_back('}');
					json_str.push_back(',');
				}
				if (json_str.back() == ',')
					json_str.pop_back();
				json_str.push_back(']');
				return true;
			}
			template<typename T>
			bool insert(T &obj)
			{
				std::shared_ptr<acl::query> query = make_insert_query(obj);
				if (!exec_update(*query.get()))
				{
					std::cout << query->to_string().c_str() << std::endl;
					return false;
				}
				return true;
			}
		private:
			template<typename T>
			std::shared_ptr<acl::query> make_insert_query(T &obj)
			{
				std::string sql_template("insert into ");
				sql_template.append(get_name<T>());
				sql_template.push_back('(');
				for_each(obj, [&sql_template](auto &item, size_t I, bool is_last) {
					sql_template.append(get_name<T>(I));
					if (!is_last)
						sql_template.push_back(',');
				});
				sql_template.append(")values(");

				for_each(obj, [&sql_template](auto &item, size_t I, bool is_last) {

					sql_template.push_back(':');
					sql_template.append(get_name<T>(I));
					if (!is_last)
						sql_template.push_back(',');
				});
				sql_template.push_back(')');

				std::shared_ptr<acl::query> query(new acl::query);
				query->create_sql(sql_template.c_str());

				for_each(obj, [&](auto &item, size_t I, bool is_last) {
					query->set_parameter(get_name<T>(I), item);
				});

				return query;
			}
			template<typename T>
			std::enable_if_t<std::is_arithmetic<T>::value>
			append_x(T &, std::string &str)
			{
				return;
			}
			template<typename T>
			std::enable_if_t<!std::is_arithmetic<T>::value>
			append_x(T &, std::string &str)
			{
				str.push_back('"');
			}
			void assign(int &item, const char *value)
			{
				item = std::strtol(value, 0, 10);
			}
			void assign(unsigned int &item, const char *value)
			{
				item = std::strtoul(value, 0, 10);
			}
			void assign(float &item, const char *value)
			{
				item = std::strtof(value, 0);
			}

			void assign(double &item, const char *value)
			{
				item = std::strtod(value, 0);
			}

			void assign(acl::string &item, const char *value)
			{
				item.append(value);
			}

			void assign(std::string &item, const char *value)
			{
				item.append(value);
			}
		};
	}
}
