#pragma once
#include "../acl_cpp_define.hpp"
#if defined(_WIN32) || defined(_WIN64)
#include <WinSock2.h>
#endif

namespace acl {

class socket_stream;

/**
 * ����˼����׽ӿ��࣬���տͻ������ӣ��������ͻ��������Ӷ���
 */
class ACL_CPP_API server_socket
{
public:
	/**
	 * ���캯�������ñ����캯����������෽�� open ������ָ�������ַ
	 * @param backlog {int} �����׽ӿڶ��г���
	 * @param block {bool} ������ģʽ���Ƿ�����ģʽ
	 */
	server_socket(int backlog = 128, bool block = true);

	/**
	 * ���캯�������ñ����캯�����ֹ�ٵ��� open ����
	 * @param sstream {ACL_VSTREAM*} �ⲿ�����ļ��������󣬱����ʹ��
	 *  �������ͷţ���Ӧ�����йرոü�������
	 */
	server_socket(ACL_VSTREAM* sstream);

	/**
	 * ���캯�������ñ����캯�����ֹ�ٵ��� open ����
	 * @param fd {ACL_SOCKET} �ⲿ�����ļ�������������ʹ�õ������ͷţ�
	 *  ��Ӧ�����йرոü������
	 */
#if defined(_WIN32) || defined(_WIN64)
	server_socket(SOCKET fd);
#else
	server_socket(int fd);
#endif

	~server_socket();

	/**
	 * ��ʼ������������˵�ַ
	 * @param addr {const char*} ������������ַ����ʽΪ��
	 *  ip:port���� unix �����£������������׽ӿڣ���ʽΪ��
	 *   /path/xxx
	 * @return {bool} �����Ƿ�ɹ�
	 */
	bool open(const char* addr);

	/**
	 * �ر��Ѿ��򿪵ļ����׽ӿ�
	 * @return {bool} �Ƿ������ر�
	 */
	bool close();

	/**
	 * ���տͻ������Ӳ������ͻ���������
	 * @param timeout {int} ������ģʽ�£�����ֵ > 0 ʱ�����ó�ʱ
	 *  ��ʽ���տͻ������ӣ�����ָ��ʱ����δ��ÿͻ������ӣ��򷵻� NULL
	 * @return {socket_stream*} ���ؿձ�ʾ����ʧ��
	 */
	socket_stream* accept(int timeout = 0);

	/**
	 * ��ü����ĵ�ַ
	 * @return {const char*} ����ֵ�ǿ�ָ��
	 */
	const char* get_addr() const
	{
		return addr_;
	}

	/**
	 * ������������������ַ����ñ��������Ի�ü����׽ӿ�
	 * @return {int}
	 */
#if defined(_WIN32) || defined(_WIN64)
	SOCKET sock_handle() const
#else
	int sock_handle() const
#endif
	{
		return fd_;
	}

	/**
	 * ���ü����׽��ֵ��ӳٽ��չ��ܣ������ͻ���������������ʱ�Ž������ӷ���
	 * ��Ӧ�ã�Ŀǰ�ù��ܽ�֧�� Linux
	 * @param timeout {int} ����ͻ��������ڹ涨��ʱ����δ�������ݣ�
	 *  Ҳ�������ӷ��ظ�Ӧ��
	 */
	void set_tcp_defer_accept(int timeout);

private:
	int   backlog_;
	bool  block_;
	bool  unix_sock_;
	char  addr_[64];

#if defined(_WIN32) || defined(_WIN64)
	SOCKET fd_;
	SOCKET fd_local_;
#else
	int   fd_;
	int   fd_local_;
#endif
};

} // namespace acl
