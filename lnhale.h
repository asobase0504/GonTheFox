//==================================================
// 
// �z�����ݏ���
// Author  :Hamada Ryuuga
// 
//==================================================


#ifndef _LNHALE_H_		// ���̃}�N����`������Ȃ�������
#define _LNHALE_H_		// 2�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

class CLnhale			//Lnhale�N���X
{
public:
	CLnhale();				//�R���X�g���N�^�i�C���X�^���X�������ɒʂ�܂��j
	~CLnhale();				//�f�X�g���N�^�i�C���X�^���X�j�����ɒʂ�܂��j
	void Init();			//������
	void Uninit();			//�j��
	void Update();			//����
	void Draw();			//�o��
	void Set(D3DXVECTOR3 setpos);
	bool Hit(D3DXVECTOR3 nowpos, D3DXVECTOR3 hitsize);
private:
	D3DXVECTOR3 pos;					//���W
	D3DXVECTOR3 size;					//�T�C�Y
	LPDIRECT3DTEXTURE9		pTexture;	// �e�N�X�`��
	LPDIRECT3DVERTEXBUFFER9	pVtxBuff;	// ���_�o�b�t�@
	bool					bUse;		// �g�p���Ă��邩�ǂ���
};


#endif // !_FAN_H_