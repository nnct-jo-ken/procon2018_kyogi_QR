#pragma once

#include"json.hpp"
#include<Siv3D.hpp>

using json = nlohmann::json;

/// <summary>
/// ピースのオブジェクトの作成
/// </summary>
/// <param name="creating_target">作成先のjsonオブジェクト</param>
/// <param name="object_no">作成するオブジェクトのid</param>
void object_init(json *creating_target, unsigned int object_no);

/// <summary>
/// 頂点の定義
/// </summary>
/// <param name="set_target">頂点を定義するオブジェクトのあるjson</param>
/// <param name="object_no">頂点を定義するオブジェクトのid</param>
/// <param name="node">頂点</param>
void node_set(json *set_target, unsigned int object_no, Array<Vec2> node);