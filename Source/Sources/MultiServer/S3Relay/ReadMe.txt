========================================================================
       WIN32 APPLICATION : S3Relay
========================================================================



Ԥ����꣺
_WORKMODE_SINGLETHREAD / _WORKMODE_MULTITHREAD / _WORKMODE_MULTITHREAD2 : ����ģʽ�����߳�/���߳�/�ص����̣߳������붨������֮һ
_TESTING : ����˺걻���壬dTRACE��DEBUGDO���Զ���Debugָ��Ҳ����Release������Ч��assert��ϵͳָ��������ڣ�






��������������Heaven����
CNetServer: ��������
CNetConnect: �յ�������
	������ʽ�ķ������������������������������ӿ���

�������ͻ���Rainbow����
CNetCenter: �ͻ�����
CNetClient: �ͻ�����
	������ʽ�Ŀͻ��������������������������ӿ���



CRootCenter / CRootClient: ��RelayServer������أ�������һ��ʵ��
CGatewayCenter / CGatewayClient: ��Gateway������أ�������һ��ʵ��
CDBCenter / CDBClient: ��DBRole������أ�������һ��ʵ��

CRelayCenter / CRelayClient: ������ͬ��Relay���ӣ�CRelayCenter��һ��ʵ����CRelayClient��ͬ��Relay���
CRelayServer / CRelayConnect: ����ͬ��Relay�뱾�����ӣ�CRelayServer��һ��ʵ����CRelayConnect�����Ӷ�Ӧ

ÿ��GameServer��S3Relay֮�䶼��3�����ӣ�Host��Chat��Tong���ֱ����������ӡ����졢��Ƶ��
CHostServer / CHostConnect: GameServer�뱾�����ӣ�CHostServer��һ��ʵ����CHostConnect�����Ӷ�Ӧ
CChatServer / CChatConnect: GameServer�뱾�����ӣ�CChatServer��һ��ʵ����CChatConnect�����Ӷ�Ӧ
CTongServer / CTongConnect: GameServer�뱾�����ӣ�CTongServer��һ��ʵ����CTongConnect�����Ӷ�Ӧ




�����ļ���INI��ʽ����

relay_config.ini : ���������ļ�

	[root]						//��Ϊ�ͻ���S3RelayServer��������ز���
	address = 192.168.20.63		//����IP��ַ
	freebuffer = 15				//������ײ������
	buffersize = 1048576		//������ײ������
	retryinterval = 12000		//���������0����������


	[gateway]					//��Ϊ�ͻ���Bishop��������ز������μ�[root]
	address = 192.168.20.63
	freebuffer = 15
	buffersize = 1048576
	retryinterval = 12000

	[dbrole]					//��Ϊ�ͻ���Goddess��������ز������μ�[root]
	address = 192.168.20.63
	freebuffer = 15
	buffersize = 1048576
	retryinterval = 12000

	[relay]						//��Ϊ��������Ϊ����S3Relay�����ķ������ز���
	playercnt = 10				//������ײ������
	precision = 1				//������ײ������
	freebuffer = 15				//������ײ������
	buffersize = 1048576		//������ײ������

	[host]						//��Ϊ��������ΪGameServer��Host���ӿ����ķ������ز������μ�[relay]
	playercnt = 10
	precision = 1
	freebuffer = 15
	buffersize = 1048576

	[chat]						//��Ϊ��������ΪGameServer��Chat���ӿ����ķ������ز������μ�[relay]
	playercnt = 10
	precision = 1
	freebuffer = 15
	buffersize = 1048576

	[tong]						//��Ϊ��������ΪGameServer��Tong���ӿ����ķ������ز������μ�[relay]
	playercnt = 10
	precision = 1
	freebuffer = 15
	buffersize = 1048576


relay_channcfg.ini : Ƶ�����������ļ�

	[system]					//ϵͳ����
	nameGM = GM					//GMƵ����
	charEsc = \					//���ڲ�������
	charSplt = :				//���ڲ�������
	defCost = 0					//Ĭ�ϻ���

	[team]						//����Ƶ��
	escSpec = T					//���ڲ�������
	minID = 0					//������СID
	maxID = -1					//�������ID
	cost = 0					//�������컨��

	[faction]					//����Ƶ��
	escSpec = F					//���ڲ�������
	minID = 0					//������СID
	maxID = -1					//�������ID
	cost = 1					//�������컨��

	[tong]						//���Ƶ��
	escSpec = O					//���ڲ�������
	minID = 0					//������СID
	maxID = -1					//�������ID
	cost = 1					//������컨��

	[screen]					//ͬ������
	escSpec = S					//���ڲ�������
	cost = 0					//ͬ�����컨��

	[broadcast]					//����Ƶ��
	escSpec = B					//���ڲ�������
	cost = 2					//�������컨��


relay_channel.ini : StockƵ���б�

	[����]						//Ƶ������
	cost = 2					//Ƶ������


relay_friendcfg.ini : ���Ѳ��������ļ�

	[database]					//�������ݿ����
	path = dbfriend				//���ݿ��ļ���
	file = friend				//���ݿ�����
	updateinterval = 300000		//Update���ݿ�����0�򲻻��Զ�Update
	storeplayerperudtdb = 8		//���ڲ�������
	loadplayerperudtdb = 2		//���ڲ�������
