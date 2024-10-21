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
	//コンスタントバッファー:アプリ側から、シェーダーに毎フレーム渡したい情報
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matW;
	};

	//頂点情報
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
	};

	uint64_t vertexNum_;			//頂点数
	vector<VERTEX> vertices_;		//頂点情報
	uint64_t indexNum_;				//インデックス数
	vector<int> indices_;			//インデックス情報

	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ
	ID3D11Buffer* pIndexBuffer_;	//インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ

	Texture* pTexture_;//テクスチャへのポインタ
public:
	Sprite();//コンストラクタ
	~Sprite();//デストラクタ
	HRESULT Load(std::string fileName);//初期化用
	void Draw(Transform& transform);//描画関数
	void Release();//解放処理
private:
	void InitVertexData();	//頂点情報の準備
	HRESULT CreateVertexBuffer();
	void InitIndexData();
	HRESULT CreateIndexBuffer();
	HRESULT CreateConstantBuffer();
	HRESULT LoadTexture(std::string fileName);

	void PassDataToCB(DirectX::XMMATRIX worldMatrix);
	void SetBufferToPipeline();
};

