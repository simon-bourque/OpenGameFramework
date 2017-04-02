#include "PlatformerGame.h"

#include "Core.h"
#include <iostream>

#include "Resources.h"

#include "Vector2f.h"
#include "Matrix3f.h"

int main() {

	PlatformerGame game;
	game.run();

	//string test = loadLevel("../res/level/level_0.lvl");

	//cout << endl << test;
	
	//Vector2f v0;
	//Vector2f v1 = { 5, 7 };
	//Vector2f v2 = { 1, 2 };
	//float f = 3.0;

	//Vector2f v3 = v1 + v2;
	//Vector2f v4 = v2 - v1;
	//Vector2f v5 = v2 * f;

	//cout << "<" << v0.x << ", " << v0.y << ">" << endl;
	//cout << "<" << v3.x << ", " << v3.y << ">" << endl;
	//cout << "<" << v4.x << ", " << v4.y << ">" << endl;
	//cout << "<" << v5.x << ", " << v5.y << ">" << endl;

	//Matrix3f m;

	//cout << sizeof(m) << endl;
	//for (auto i = 0; i < 9; i++) {
		//cout << m.values[i] << endl;
	//}

	//float32 array[] = {1,2,3,4,5,6,7,8,9};

	//Matrix3f m1(array);
	//Matrix3f m2 = array;

	//Matrix3f m = {1,2,3,4,5,6,7,8,9};

	//Matrix3f m3(m1);

	//for (auto i = 0; i < 9; i++) {
		//cout << m3.values[i] << endl;
	//}

	//int pause;
	//cin >> pause;

	return 0;
}