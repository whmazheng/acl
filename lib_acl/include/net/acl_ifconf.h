#ifndef	ACL_IFCONF_INCLUDE_H
#define	ACL_IFCONF_INCLUDE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "../stdlib/acl_define.h"
#include "../stdlib/acl_argv.h"

typedef struct ACL_IFADDR {
	char *name;		/* �ӿ����� */
#if defined(_WIN32) || defined(_WIN64)
	char *desc;		/* �ӿ����� */
#endif
	char  ip[32];		/* ���ַ�����ʾ��IP��ַ */
	unsigned int addr;	/* �����ֽ���� 32 λ IP ��ַ */
} ACL_IFADDR;

typedef struct ACL_IFCONF {
	ACL_IFADDR *addrs;	/* ACL_IFADDR ���� */
	int  length;		/* ACL_IFADDR ���鳤�� */

	/* for acl_iterator */

	/* ȡ������ͷ���� */
	const ACL_IFADDR *(*iter_head)(ACL_ITER*, struct ACL_IFCONF*);
	/* ȡ��������һ������ */
	const ACL_IFADDR *(*iter_next)(ACL_ITER*, struct ACL_IFCONF*);
	/* ȡ������β���� */
	const ACL_IFADDR *(*iter_tail)(ACL_ITER*, struct ACL_IFCONF*);
	/* ȡ��������һ������ */
	const ACL_IFADDR *(*iter_prev)(ACL_ITER*, struct ACL_IFCONF*);
} ACL_IFCONF;

/**
 * ������������������ַ������ӿ�����
 * @return {ACL_IFCONF*}
 */
ACL_API ACL_IFCONF *acl_get_ifaddrs(void);

/**
 * �ͷ��� acl_get_ifaddrs() ���ص� ACL_IFCONF �ڴ�
 * @param ifconf {ACL_IFCONF*}
 */
ACL_API void acl_free_ifaddrs(ACL_IFCONF *ifconf);

/**
 * ɨ�豾���������� IP��������ƥ��ָ��ģʽ�� IP ��ַ���أ�Ŀǰ��֧�� IPV4
 * @param pattern {const char *} ָ����ƥ��ģʽ����ʽΪ��xxx.xxx.xxx.xxx ��
 *  xxx.xxx.xxx.xxx:port���磺192.168.*.*, 192.168.*.8:80��10.*.0.*:81
 * @return {ACL_ARGV *} ���������Ľ��������� pattern ������ж˿ڣ����Զ�
 *  ���˿������ÿ�� IP ���棬�����ض���ǿգ�����������Ӧ���� acl_argv_free
 *  �ͷŸö���
 */
ACL_API ACL_ARGV *acl_ifconf_search(const char *pattern);

#ifdef	__cplusplus
}
#endif

#endif
