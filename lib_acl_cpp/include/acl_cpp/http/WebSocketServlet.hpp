#pragma once
#include "../acl_cpp_define.hpp"
#include "HttpServlet.hpp"

namespace acl
{

class websocket;
class session;

class WebSocketServlet: public HttpServlet
{
public:
	WebSocketServlet(void);

	/**
	 * ���캯��
	 * @param stream {socket_stream*} ���� acl_master ��������ܿ�����
	 *  ����ʱ���ò�������ǿգ����� apache ���� CGI ��ʽ����ʱ���ò���
	 *  ��Ϊ NULL�����⣬�ú����ڲ�����ر������ӣ�Ӧ��Ӧ���д���������
	 *  �Ĺر�������������Է����� acl_master �ܹ����
	 * @param session {session*} ÿһ�� HttpServlet ����һ�� session ����
	 */
	WebSocketServlet(socket_stream* stream, session* session);

	/**
	 * ���캯��
	 * @param stream {socket_stream*} ���� acl_master ��������ܿ�����
	 *  ����ʱ���ò�������ǿգ����� apache ���� CGI ��ʽ����ʱ���ò���
	 *  ��Ϊ NULL�����⣬�ú����ڲ�����ر������ӣ�Ӧ��Ӧ���д���������
	 *  �Ĺر�������������Է����� acl_master �ܹ����
	 * @param memcache_addr {const char*}
	 */
	WebSocketServlet(socket_stream* stream,
		const char* memcache_addr = "127.0.0.1:11211");

	/**
	 * HttpServlet ����ʼ���У����� HTTP ���󣬲��ص����� doXXX �麯����
	 * �ú������Ȼ���� start ���̣�Ȼ����� start �ķ��ؽ��������/��Ӧ
	 * �����Ƿ�Ҫ�󱣳ֳ������������Ƿ���Ҫ��ͻ��˱��ֳ�����.
	 * @return {bool} ���ش����������� false ��ʾ����ʧ�ܻ���ɹ���
	 *  �����ֳ����ӣ�Ӧ�ر�����
	 */

	virtual ~WebSocketServlet(void);

	/**
	 * HttpServlet ����ʼ���У����� HTTP ���󣬲��ص����� doXXX �麯��
	 * @param session {session&} �洢 session ���ݵĶ���
	 * @param stream {socket_stream*} ���� acl_master ��������ܿ�����
	 *  ����ʱ���ò�������ǿգ����� apache ���� CGI ��ʽ����ʱ���ò���
	 *  ��Ϊ NULL�����⣬�ú����ڲ�����ر������ӣ�Ӧ��Ӧ���д���������
	 *  �Ĺر�������������Է����� acl_master �ܹ����
	 * @return {bool} ���ش�����
	 */
	virtual bool doRun(session& session, socket_stream* stream = NULL)
	{
		return HttpServlet::doRun(session, stream);
	}

	/**
	 * HttpServlet ����ʼ���У����� HTTP ���󣬲��ص����� doXXX �麯����
	 * ���ñ�������ζ�Ų��� memcached ���洢 session ����
	 * @param memcached_addr {const char*} memcached ��������ַ����ʽ��IP:PORT
	 * @param stream {socket_stream*} ����ͬ��
	 * @return {bool} ���ش�����
	 */
	virtual bool doRun(const char* memcached_addr, socket_stream* stream)
	{
		return HttpServlet::doRun(memcached_addr, stream);
	}

	virtual bool doRun(void);

	/**
	 * ���Ͷ���������.
	 * @param rw_timeout {const char *} ���͵�����
	 * @return {bool} ���� false.���� true
	 */

	bool send_binary(const char *buf, int len);

	/**
	 * �����ı�����.
	 * @param rw_timeout {const char *} ���͵�����
	 * @return {bool} ���� false.���� true
	 */

	bool send_text(const char *text);

	/**
	 * ����pong ��Ϣ.
	 * @param rw_timeout {const char *} ���͵�����
	 * @return {bool} ���� false.���� true
	 */
	bool send_pong(const char *buffer = NULL);

	/**
	 * ����pong ��Ϣ.
	 * @param rw_timeout {const char *} ���͵�����
	 * @return {bool} ���� false.���� true
	 */
	bool send_ping(const char *buffer = NULL);

	unsigned long long get_max_msg_len(void) const
	{
		return max_msg_len_;
	}

	/**
	 * ���������Ϣ���ȣ���websocket ��Ϣ�������ֵ�����Ͽ�websocket����.
	 * @param unsigned long long{len} �µĳ���
	 */
	void set_max_msg_len(unsigned long long len)
	{
		max_msg_len_ = len;
	}

protected:
	/**
	 * websocket �ر���Ϣ�ص�
	 * @return {void}
	 */
	virtual void on_close(void);

	/**
	 * websocket ping ��Ϣ�ص�.
	 * @param {const char *} buf ��Ϣ����
	 * @param {int} len ��Ϣ���ݳ���
	 * @return {bool} false �Ͽ����ӡ�
	 */
	virtual bool on_ping(const char *buf, unsigned long long  len) = 0;

	/**
	 * websocket pong ��Ϣ�ص�.
	 * @param {const char *} buf ��Ϣ����
	 * @param {int} len ��Ϣ���ݳ���
	 * @return {bool} false �Ͽ����ӡ�
	 */
	virtual bool on_pong(const char *buf, unsigned long long  len) = 0;

	/**
	 * websocket ping ��Ϣ�ص�.
	 * @param data{char *} �ص����ݻ�������
	 * @param len{unsigned long long}�ص����ݻ��������ȡ�
	 * @param text{bool } true Ϊ�ı����ݡ������� ���������ݡ�
	 * @return {bool} false �Ͽ����ӡ�
	 */
	virtual bool on_message(char *data, unsigned long long len, bool text) = 0;

private:
	// @override
	bool doWebsocket(HttpServletRequest&, HttpServletResponse&);

private:

	unsigned long long max_msg_len_;
	websocket *ws_;

	char *recv_buffer_;
	int write_pos_;
	int opcode_;
};

} // namespace acl
