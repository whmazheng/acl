#include "lib_acl.h"

int main(void)
{
        ACL_IFCONF *ifconf;	/* ������ѯ������� */
        ACL_IFADDR *ifaddr;	/* ÿ��������Ϣ���� */
        ACL_ITER iter;		/* �������� */
	const char *pattern = "127.*.*.*:8290, 192.168.*.*:8290, 172.16.*.*.:8290, 172.17.*.*.:8290, /unix_server@unix";
	ACL_ARGV   *addrs;

	/* ��ѯ��������������Ϣ */
	ifconf = acl_get_ifaddrs();

	if (ifconf == NULL) {
		printf("acl_get_ifaddrs error: %s\r\n", acl_last_serror());
		return 1;
	}

	/* ����������������Ϣ */
	acl_foreach(iter, ifconf) {
		ifaddr = (ACL_IFADDR*) iter.data;
		printf(">>>ip: %s, name: %s\r\n", ifaddr->ip, ifaddr->name);
	}

	/* �ͷŲ�ѯ��� */
	acl_free_ifaddrs(ifconf);

	printf("\r\n----------------------------------------------\r\n");

	addrs = acl_ifconf_search(pattern);
	if (addrs == NULL) {
		printf("acl_ifconf_search error\r\n");
		return 1;
	}

	printf("pattern=%s\r\n", pattern);
	acl_foreach(iter, addrs) {
		const char *addr = (const char *)iter.data;
		printf(">>>ip: %s\r\n", addr);
	}
	acl_argv_free(addrs);


	return 0;
}
