//==================================================
// 
// 色の区分
// Author  : katsuki mizuki
// 
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "main.h"
#include "color.h"

#include <assert.h>

//--------------------------------------------------
// 定義
//--------------------------------------------------
namespace
{
const D3DXCOLOR	RED(1.0f, 0.0f, 0.0f, 1.0f);		// 赤
const D3DXCOLOR	GREEN(0.0f, 1.0f, 0.0f, 1.0f);		// 緑
const D3DXCOLOR	BLUE(0.0f, 0.0f, 1.0f, 1.0f);		// 青
const D3DXCOLOR	YELLOW(1.0f, 1.0f, 0.0f, 1.0f);		// 黄色
const D3DXCOLOR	PURPLE(1.0f, 0.0f, 1.0f, 1.0f);		// 紫
const D3DXCOLOR	LIGHTBLUE(0.0f, 1.0f, 1.0f, 1.0f);	// 水色
const D3DXCOLOR	WHITE(1.0f, 1.0f, 1.0f, 1.0f);		// 白
const D3DXCOLOR	GRAY(0.5f, 0.5f, 0.5f, 1.0f);		// 灰色
const D3DXCOLOR	BLACK(0.0f, 0.0f, 0.0f, 1.0f);		// 黒

const D3DXCOLOR* s_Colors[] =
{// 色の配列
	&RED,
	&GREEN,
	&BLUE,
	&YELLOW,
	&PURPLE,
	&LIGHTBLUE,
	&WHITE,
	&GRAY,
	&BLACK
};
}// namespaceはここまで

static_assert(sizeof(s_Colors) / sizeof(s_Colors[0]) == COLOR_MAX, "aho");

//#define ARRAY_LENGTH(array_) (sizeof(array_)/sizeof(array_[0]))
//static_assert(ARRAY_LENGTH(s_Colors) == COLOR_MAX, "aho");
//#define STATIC_ASSERT(a) static_assert(a, #a)
//STATIC_ASSERT(ARRAY_LENGTH(s_Colors) == COLOR_MAX);

//==================================================
// 取得
//==================================================
D3DXCOLOR GetColor(COLOR color)
{
	assert(color >= 0 && color < COLOR_MAX);

	return *s_Colors[color];
}
