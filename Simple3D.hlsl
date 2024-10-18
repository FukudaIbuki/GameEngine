//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー

//───────────────────────────────────────
// コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global
{
    //変換行列、視点、光源
    float4x4 matWVP; // ワールド・ビュー・プロジェクションの合成行列
    float4x4 matW;
    float4 diffuseColor;//拡散反射係数
    bool isTextured;//テクスチャが張られているかどうか
};

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
    float4 pos : SV_POSITION; //位置
    float2 uv : TEXCOORD; //UV座標
    float4 color : COLOR; //色（明るさ）
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//ピクセルシェーダーへ渡す情報
    VS_OUT outData;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
    outData.pos = mul(pos, matWVP);
    outData.uv = uv;

    float4 light = float4(-1, -1, -1, 0);//光源ベクトルの逆ベクトル化
    light = normalize(light);//単位ベクトル化
    
    normal = mul(normal, matW);
    normal = normalize(normal);
    normal.w = 0;
    light.w = 0;
    
    outData.color = clamp(dot(normal, light), 0, 1);

	//まとめて出力
    return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
    //return float4(0, 0, 0, 1);//ピクセルを塗る色
    float4 Id = { 1.0, 1.0, 1.0, 0 }; //光の強さ、通常前職１．０
    float4 Kd = g_texture.Sample(g_sampler, inData.uv);//テクスチャの色をサンプリング
    float cos_alpha = inData.color;//拡散反射係数の計算（光源ベクトルと面の法線のcos)
    float4 ambentSource = { 0.3, 0.3, 0.3, 0.0 };//環境光の強さ
    if(isTextured == false)
    {
        //サンプラーの色じゃなく、コンスタントバッファで渡された拡散反射係数から色を決める
        return Id * diffuseColor * cos_alpha + Id * diffuseColor * ambentSource;
    }
    else
    {
        return Id * Kd * cos_alpha + Id * Kd * ambentSource;
    }
    //return g_texture.Sample(g_sampler, inData.uv);
}