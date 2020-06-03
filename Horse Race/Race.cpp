#include <iostream>
#include <string>
#include <random>
#include <windows.h>

#define MAX_PEOPLE 99
#define PAUSE_ENABLED 1

using namespace std;

// ���a�C
char race[5][20];
int where_is_the_horse[5];
int horse_round = 1;
int horse_goal;
int finished[3][5];

// �ƥ�
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
	/* ���� */
	// �߲v�G1st : 150%, 2nd: 125%, 3rd: 120%
	// ��o�������H�ơ֩��L�����H��+���ɡA
	// ���y�ס]100% + (�߲v - 100%) �� (���L�����H�ơ��㰨�`�H��)�^�ѤU�`���B�C
	// �_�h���y�׽߲v�ѤU�`���B�C
	const double odds[3] = { 1.5, 1.25, 1.2 };

	cout << "��e���߲v�Ҧ�" << endl <<endl;
	cout << "�߲v�G�Ĥ@: 150%, �ĤG: 125%, �ĤT: 120%" << endl << endl;
	cout << "��o�������H�ơ֩��L�����H��+���ɡA" << endl << endl;
	cout << "���y�ס]100% + (�߲v - 100%) �� (���L�����H�ơ��㰨�`�H��)�^�ѤU�`���B�C" << endl << endl;
	cout << "�_�h���y�׽߲v�ѤU�`���B�C" << endl << endl;
	
	/* �ǳƳ��a�C*/
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

	/* �}�l�U�`�C*/
	bool win_lose[MAX_PEOPLE] = { false };
	int people, num[5] = { 0 }, horse_to_people[5][MAX_PEOPLE] = { -1 };
	pair<int, int> info[MAX_PEOPLE];  // �Ĥ@�ӬO�����s��, �ĤG�ӬO�U�`���B
	do {
		cout << "�п�J�ѥ[�H��" << endl << endl;
		cin >> people;
		cout << endl;
	} while (people > MAX_PEOPLE);
	cout << "�п�J�㪺���s���M�U�`���B" << endl << endl;
	for (int i = 0; i < people; i++) {
		cin >> info[i].first >> info[i].second;
		horse_to_people[info[i].first - 1][num[info[i].first - 1]++] = i;
	}

	/* �}�l�ɰ��C*/
	cout << endl << "�}�l�ɰ�" << endl << endl;
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

		/* �e�X��e���p�C*/
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 20; j++) {
				cout << race[i][j];
			}
			cout << endl;
		}
		cout << endl << endl;

		/* �ˬd���ǰ��]���F*/
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
	/* ������y�C*/
	for (int i = 0; i < 3; i++) {
		cout << "�� " + to_string(i+1) + " �W�G " ;
		for (int j = 0; j < 5; j++) {
			if (finished[i][j] == -1) {
				continue;
			}
			int win_horse = finished[i][j];
			int win_people_num = num[win_horse];
			cout << to_string(win_horse + 1) + " ����  ";
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

	/* ��ܼ��y */
	for (int i = 0; i < people; i++) {
		cout << "�� " + to_string(i + 1) << " ��ѥ[��";
		if (win_lose[i]) {
			cout << "��o�F ";
		} else {
			cout << "���h�F ";
		}
		cout << to_string(info[i].second) + " BB" << endl << endl;
	}
	return 0;
}

void MJZS(int horse) {
	int place = where_is_the_horse[horse];
	if (place == -1) {  
		// �]�����������ʡA���Өƥ�C
		return;
	}
	int new_place = ((place - 2) < 1) ? 1 : (place - 2);
	race[horse][place] = '=';
	race[horse][new_place] = 'H';
	where_is_the_horse[horse] = new_place;
	cout << to_string(horse + 1) + "������o�F���������ߡA�i��F�T���[�t�P��" << endl << endl;
}

void NMFL(int horse) {
	int place = where_is_the_horse[horse];
	if (place == -1) {
		return;
	}
	cout << to_string(horse+1)+"����ı���ۧڷN�ѡA���۵ۡu�A�K�O�F�v���f���a���L���ϦV�b�]" << endl << endl;
}

void hunter(int horse) {
	cout << "�@���y���H���L�A��½�F" + to_string(horse + 1) + "�����C" << endl << endl;
}

void female() {
	nothing(true);
	nothing(true);
	cout << "�e���M���L�@�Y�����A�Ҧ������^�X�[�t�e�i" << endl << endl;
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
	cout << to_string(horse + 1) + "�����M" + to_string(horse_2 + 1) + "�����]���D�A�洫�F�ɹD" << endl << endl;
}

void wet() {
	if (wet_road.first) {
		// �D���w�g�O�㪺�F�C
		return;
	}
	wet_road.first = true;
	wet_road.second = 3;
	cout << "�D���d�U���A�w���Ĥ@���A�ɰ����`�N�A�[�����\�C" << endl << endl;
	cout << "�D����ơA�C�Y���T�^�X������50%���v�L�ˡC" << endl << endl;
}

void nothing(bool direction) {
	int vec = direction ? 1 : -1;
	for (int i = 0; i < 5; i++) {
		int place = where_is_the_horse[i];
		if (place == -1 || i == kick) {
			continue;
		}
		if (wet_road.first && wet_dist(rng)) {
			cout << to_string(i + 1) + "�����L�ˤF�C" << endl << endl;
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