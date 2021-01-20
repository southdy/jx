#ifndef KObstacleH
#define	KObstacleH

enum Obstacle_Type			// �����������ϰ�����
{
	Obstacle_Empty = 0,		// �գ����ϰ�
	Obstacle_Full,			// �����ϰ�
	Obstacle_LT,			// ��������Ϊ�ϰ�
	Obstacle_RT,			// ��������Ϊ�ϰ�
	Obstacle_LB,			// ��������Ϊ�ϰ�
	Obstacle_RB,			// ��������Ϊ�ϰ�
	Obstacle_Type_Num,		// �ϰ�������
};

enum Obstacle_Kind
{
	Obstacle_NULL = 0,		// û���ϰ�
	Obstacle_Normal,		// һ���ϰ�
	Obstacle_Fly,			// �ϰ����ǿ��Էɹ�
	Obstacle_Jump,			// �ϰ����ǿ�������
	Obstacle_JumpFly,		// �ϰ����ǿ����������ҷɹ�
	Obstacle_Kind_Num,		// �ϰ�������
};
#endif
