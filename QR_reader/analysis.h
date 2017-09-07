#pragma once

#include<Siv3D.hpp>

/// <summary>
/// 頂点座標の取得
/// </summary>
/// <param name="piece">頂点情報</param>
/// <returns>頂点の配列</returns>
Array<Vec2> get_node(String piece);

/// <summary>
/// QR情報の解析→ピースの情報ごとに分割
/// </summary>
/// <param name="decoded">デコードしたデータ</param>
/// <returns>ピース情報の配列</returns>
Array<String> get_pieceinfo(Array<String> decoded);