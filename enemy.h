//============================
//
// �v���C���[�ݒ�w�b�^�[
// Author:hamada ryuuga
//
//============================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "motion.h"
#include "player.h"

//------------------------------------
// �}�N��
//------------------------------------
#define Attenuation	(0.5f)		//�����W��
#define Speed	(1.0f)			//�X�s�[�h
#define WIDTH (10.0f)			//���f���̔��a
#define MAX_PRAYER (16)			//�ő吔
#define MAX_MOVE (9)			//�A�j���[�V�����̍ő吔
#define INVINCIBLE (300)		//���G����
#define MAX_MODELPARTS (9)
#define MAX_COPY  (4)

//------------------------------------
// ��ʂ̗񋓌^
//------------------------------------
enum ENEMY_TYPE
{
	ENEMY_TYPE_HUMANSOUL = 0,	// �l��
	ENEMY_TYPE_SKELETON,		// ��������
	ENEMY_TYPE_UNGAIKYO,		// �_�O��
	ENEMY_TYPE_KAMAITACHI,		// ���܂�����
	ENEMY_TYPE_BUDDHA,			// �u�c�[
	ENEMY_TYPE_MAX
};

typedef struct
{
	D3DXVECTOR3	pos;						// �ʒu
	D3DXVECTOR3	posOld;						// �ʒu�ߋ�
	D3DXVECTOR3	move;						// ���[�u
	D3DXVECTOR3	rot;						// ��]	
	D3DXVECTOR3	rotMove;					// ��]���[�u
	D3DXVECTOR3	modelMin;					// �T�C�Y�ŏ�
	D3DXVECTOR3	modelMax;					// �T�C�Y�ő�
	D3DXMATRIX	mtxWorld;					// �}�g���b�N�X//�|���S���̈ʒu���]�s�񂷂ׂĂ��߂Ă�i�j�J

	STATUS		status;						// ���̃X�e�[�^�X
	DAMEGE		damege;						// �_���[�W������Ă邩������ĂȂ���
	COPY		copy;						// �R�s�[
	ENEMY_TYPE	type;						// �G�l�~�[�̃^�C�v

	Parts		parts[MAX_MODELPARTS];		// ���f���p�[�c
	PartsFile	partsFile[MAX_MODELPARTS];	// �p�[�c�t�@�C��
	MyMotion	motion[ANIME_MAX];			// ���[�V����
	ANIME		motionType;					// ���[�V�����^�C�v(����)
	ANIME		motionTypeOld;				// ���[�V�����^�C�v(�ߋ�)
	int			nMaxModelType;				// ���f���̃^�C�v��
	int			nMaxModelParts;				// �������f���p�[�c��
	int			nMaxMotion;					// ���[�V������

	int			shadow;						// �e�ԍ�
	int			invincible;					// ���G����
	float		consumption;				// �v�Z�p

	bool		bMotionBlend;				// ���[�V�����u�����h
	bool		bMotion;					// ���[�V�������g�p��

	bool		isUse;						// �g���Ă邩�g���ĂȂ���
	bool		notLoop;					// ���[�v���邩���Ȃ���
}Enemy;

//�v���g�^�C�v�錾
void InitEnemy(void);	// ������
void UninitEnemy(void);	// �j��
void UpdateEnemy(void);	// �X�V
void DrawEnemy(void);	// �`��

void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, ENEMY_TYPE type);//�Z�b�g�������W�Ɠǂݍ��ރt�@�C����
void SizeSet(void);//�����蔻��擾
Enemy *GetEnemy(void);//�Q�b�g
MODELDATAPLAYER *GetModelDataEnemy(void);//motion�f�[�^�̃Q�b�g
void LoadEnemy(void);	// �Ǎ�
#endif
