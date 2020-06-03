#include <iostream>
#include <string>
#include <random>
#include <windows.h>

#define MAX_PEOPLE 99
#define PAUSE_ENABLED 1

using namespace std;

// 場地。
char race[5][20];
int where_is_the_horse[5];
int horse_round = 1;
int horse_goal;
int finished[3][5];

// 事件
pair<bool, int> wet_road;
int kick;

void MJZS(int horse);
void NMFL(int horse);
void hunter(int horse);
void female();
void change(int horse);
void wet();
void nothing(bool direction);

//RNG
random_device dev;
mt19937 rng(dev());
uniform_int_distribution<mt19937::result_type> event_dist(1, 12);
uniform_int_distribution<mt19937::result_type> horse_dist(0, 4);
uniform_int_distribution<mt19937::result_type> wet_dist(0, 1);
uniform_int_distribution<mt19937::result_type> run_dist(0, 2);

int main() {
	/* 說明 */
	// 賠率：1st : 150%, 2nd: 125%, 3rd: 120%
	// 押這隻馬的人數＞押其他馬的人數+１時，
	// 獎勵＝（100% + (賠率 - 100%) × (押其他馬的人數／押馬總人數)）×下注金額。
	// 否則獎勵＝賠率×下注金額。
	const double odds[3] = { 1.5, 1.25, 1.2 };

	cout << "當前為賠率模式" << endl <<endl;
	cout << "賠率：第一: 150%, 第二: 125%, 第三: 120%" << endl << endl;
	cout << "押這隻馬的人數＞押其他馬的人數+１時，" << endl << endl;
	cout << "獎勵＝（100% + (賠率 - 100%) × (押其他馬的人數／押馬總人數)）×下注金額。" << endl << endl;
	cout << "否則獎勵＝賠率×下注金額。" << endl << endl;
	
	/* 準備場地。*/
	wet_road.first = false;
	kick = -1;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 5; j++) {
			finished[i][j] = -1;
		}
	}
	for (int i = 0; i < 5; i++) {
		race[i][0] = '0' + (i + 1);
		race[i][19] = 'H';
		where_is_the_horse[i] = 19;
		for (int j = 1; j < 19; j++) {
			race[i][j] = '=';
		}
		for (int j = 0; j < 20; j++) {
			cout << race[i][j];
		}
		cout << endl;
	}
	cout << endl;

	/* 開始下注。*/
	bool win_lose[MAX_PEOPLE] = { false };
	int people, num[5] = { 0 }, horse_to_people[5][MAX_PEOPLE] = { -1 };
	pair<int, int> info[MAX_PEOPLE];  // 第一個是馬的編號, 第二個是下注金額
	do {
		cout << "請輸入參加人數" << endl << endl;
		cin >> people;
		cout << endl;
	} while (people > MAX_PEOPLE);
	cout << "請輸入押的馬編號和下注金額" << endl << endl;
	for (int i = 0; i < people; i++) {
		cin >> info[i].first >> info[i].second;
		horse_to_people[info[i].first - 1][num[info[i].first - 1]++] = i;
	}

	/* 開始賽馬。*/
	cout << endl << "開始賽馬" << endl << endl;
	while (horse_goal < 3) {
		bool direction = true;
		int event = event_dist(rng);
		int horse = horse_dist(rng);
		switch (event) {
		case 1:
			MJZS(horse);
			break;
		case 2:
			NMFL(horse);
			direction = false;
			break;
		case 3:
			hunter(horse);
			kick = horse;
			break;
		case 4:
			female();
			break;
		case 5:
			change(horse);
			break;
		case 6:
			wet();
			break;
		default:
			break;
		}
		nothing(direction);
		if (wet_road.first) {
			wet_road.second--;
			if (wet_road.second == 0) {
				wet_road.first = false;
			}
		}
		if (kick != -1) {
			kick = -1;
		}

		/* 畫出當前狀況。*/
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 20; j++) {
				cout << race[i][j];
			}
			cout << endl;
		}
		cout << endl << endl;

		/* 檢查哪些馬跑完了*/
		int num = 0;
		for  (int i = 0; i < 5; i++) {
			if (where_is_the_horse[i] == 1) {
				finished[horse_round - 1][num++] = i;
				horse_goal++;
				where_is_the_horse[i] = -1;
			}
		}
		if (num) {
			horse_round++;
		}
		if (PAUSE_ENABLED && event < 7) {
			system("pause");
			cout << endl << endl;
		}
	 }
	/* 結算獎勵。*/
	for (int i = 0; i < 3; i++) {
		cout << "第 " + to_string(i+1) + " 名： " ;
		for (int j = 0; j < 5; j++) {
			if (finished[i][j] == -1) {
				continue;
			}
			int win_horse = finished[i][j];
			int win_people_num = num[win_horse];
			cout << to_string(win_horse + 1) + " 號馬  ";
			if (win_people_num == 0) {
				continue;
			}
			for (int k = 0; k < win_people_num; k++) {
				int win_people = horse_to_people[win_horse][k];
				if (win_people == -1) {
					continue;
				}
				int money = info[win_people].second;
				if (win_people_num > (people - win_people_num + 1)) {
					info[win_people].second = (int)(money * (1 + (odds[i] - 1) * (people - win_people_num) / people));
				}
				else {
					info[win_people].second = (int)(money * odds[i]);
				}
				win_lose[win_people] = true;
			}
		}
		cout << endl << endl;
	}
	cout << "--------------------";
	cout << endl << endl;

	/* 顯示獎勵 */
	for (int i = 0; i < people; i++) {
		cout << "第 " + to_string(i + 1) << " 位參加者";
		if (win_lose[i]) {
			cout << "獲得了 ";
		} else {
			cout << "失去了 ";
		}
		cout << to_string(info[i].second) + " BB" << endl << endl;
	}
	return 0;
}

void MJZS(int horse) {
	int place = where_is_the_horse[horse];
	if (place == -1) {  
		// 跑完的馬不能行動，換個事件。
		return;
	}
	int new_place = ((place - 2) < 1) ? 1 : (place - 2);
	race[horse][place] = '=';
	race[horse][new_place] = 'H';
	where_is_the_horse[horse] = new_place;
	cout << to_string(horse + 1) + "號馬獲得了明鏡止水之心，進行了三角加速同調" << endl << endl;
}

void NMFL(int horse) {
	int place = where_is_the_horse[horse];
	if (place == -1) {
		return;
	}
	cout << to_string(horse+1)+"號馬覺醒自我意識，高喊著「你免費了」的口號帶領其他馬反向奔跑" << endl << endl;
}

void hunter(int horse) {
	cout << "一個獵馬人路過，踹翻了" + to_string(horse + 1) + "號馬。" << endl << endl;
}

void female() {
	nothing(true);
	nothing(true);
	cout << "前方突然路過一頭母馬，所有馬本回合加速前進" << endl << endl;
}

void change(int horse) {
	int place = where_is_the_horse[horse];
	if (place == -1) {
		return;
	}
	int horse_2;
	do{
		horse_2 = horse_dist(rng);
	} while (horse_2 == horse || where_is_the_horse[horse_2] == -1);
	int place_2 = where_is_the_horse[horse_2];
	race[horse][place] = race[horse_2][place_2] = '=';
	race[horse][place_2] = race[horse_2][place] = 'H';
	where_is_the_horse[horse] = place_2;
	where_is_the_horse[horse_2] = place;
	cout << to_string(horse + 1) + "號馬和" + to_string(horse_2 + 1) + "號馬跑錯道，交換了賽道" << endl << endl;
}

void wet() {
	if (wet_road.first) {
		// 道路已經是濕的了。
		return;
	}
	wet_road.first = true;
	wet_road.second = 3;
	cout << "道路千萬條，安全第一條，賽馬不注意，觀眾兩行淚。" << endl << endl;
	cout << "道路濕滑，每頭馬三回合內都有50%機率摔倒。" << endl << endl;
}

void nothing(bool direction) {
	int vec = direction ? 1 : -1;
	for (int i = 0; i < 5; i++) {
		int place = where_is_the_horse[i];
		if (place == -1 || i == kick) {
			continue;
		}
		if (wet_road.first && wet_dist(rng)) {
			cout << to_string(i + 1) + "號馬摔倒了。" << endl << endl;
		}
		else {
			int forward = run_dist(rng);
			int new_place = place - forward * vec;
			if (new_place < 1) {
				new_place = 1;
			}
			else if (new_place > 19) {
				new_place = 19;
			}
			race[i][place] = '=';
			race[i][new_place] = 'H';
			where_is_the_horse[i] = new_place;
		}
	}
}