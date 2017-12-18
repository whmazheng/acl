#pragma once

class WebsocketServlet_impl : public acl::WebSocketServlet
{
public:
	WebsocketServlet_impl(acl::redis_client_cluster& cluster, size_t max_conns);
	~WebsocketServlet_impl();

	acl::session& get_session() const
	{
		return *session_;
	}

protected:
	virtual bool doUnknown(acl::HttpServletRequest&,
		acl::HttpServletResponse& res);
	virtual bool doGet(acl::HttpServletRequest& req,
		acl::HttpServletResponse& res);
	virtual bool doPost(acl::HttpServletRequest& req,
		acl::HttpServletResponse& res);


	//for websocket
	/**
	* websocket �ر���Ϣ�ص�
	* @return {void}
	*/
	virtual void on_close()
	{

	}
	/**
	* websocket ping ��Ϣ�ص�.
	* @param {const char *} buf ��Ϣ����
	* @param {int} len ��Ϣ���ݳ���
	* @return {bool} false �Ͽ����ӡ�
	*/
	virtual bool on_ping(const char *buf, unsigned long long len);
	/**
	* websocket pong ��Ϣ�ص�.
	* @param {const char *} buf ��Ϣ����
	* @param {int} len ��Ϣ���ݳ���
	* @return {bool} false �Ͽ����ӡ�
	*/
	virtual bool on_pong(const char *buf, unsigned long long len);

	/**
	* websocket ping ��Ϣ�ص�.
	* @param data{char *} �ص����ݻ�������
	* @param len{unsigned long long}�ص����ݻ��������ȡ�
	* @param text{bool } true Ϊ�ı����ݡ������� ���������ݡ�
	* @return {bool} false �Ͽ����ӡ�
	*/
	virtual bool on_message(char *data, unsigned long long len, bool text);

private:
	acl::session* session_;

	int step_;

	acl::string filename_;

	acl::ofstream *file_;

	int filesize_;

	int current_filesize_;
};
