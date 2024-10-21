#pragma once
#include "Texture.h"
#include "Direct3D.h"
#include <DirectXMath.h>
#include "vector"
#include "Transform.h"

using std::vector;
using namespace DirectX;

class Sprite
{
	//�R���X�^���g�o�b�t�@�[:�A�v��������A�V�F�[�_�[�ɖ��t���[���n���������
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matW;
	};

	//���_���
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
	};

	uint64_t vertexNum_;			//���_��
	vector<VERTEX> vertices_;		//���_���
	uint64_t indexNum_;				//�C���f�b�N�X��
	vector<int> indices_;			//�C���f�b�N�X���

	ID3D11Buffer* pVertexBuffer_;	//���_�o�b�t�@
	ID3D11Buffer* pIndexBuffer_;	//�C���f�b�N�X�o�b�t�@
	ID3D11Buffer* pConstantBuffer_;	//�R���X�^���g�o�b�t�@

	Texture* pTexture_;//�e�N�X�`���ւ̃|�C���^
public:
	Sprite();//�R���X�g���N�^
	~Sprite();//�f�X�g���N�^
	HRESULT Load(std::string fileName);//�������p
	void Draw(Transform& transform);//�`��֐�
	void Release();//�������
private:
	void InitVertexData();	//���_���̏���
	HRESULT CreateVertexBuffer();
	void InitIndexData();
	HRESULT CreateIndexBuffer();
	HRESULT CreateConstantBuffer();
	HRESULT LoadTexture(std::string fileName);

	void PassDataToCB(DirectX::XMMATRIX worldMatrix);
	void SetBufferToPipeline();
};

