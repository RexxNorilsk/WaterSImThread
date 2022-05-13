#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

#define H 0
#define O 1

using namespace std;

bool** statuses;
int** liveDuration;


int randomRange(int max, int min = 1) {
	return min + rand() % (max - min);
}

//Процесс-молекула
void molecule(int id, int type) {
	std::this_thread::sleep_for(chrono::seconds(randomRange(2)));
	statuses[type][id] = true;
	while (true) {
		if (liveDuration[type][id] != 0) {
			if (liveDuration[type][id] == -1)return;
			statuses[type][id] = false;
			std::this_thread::sleep_for(chrono::seconds(liveDuration[type][id]));
			if (liveDuration[type][id] == -1)return;
			liveDuration[type][id] = 0;
			statuses[type][id] = true;
		}
	}
}



int main()
{
	//Исходные данные
	int moleculeCountH = 8;
	int moleculeCountO = 6;
	int countMaxMolecules = 16;

	//Массивы "обмена"
	statuses = new bool*[2];
	statuses[H] = new bool[moleculeCountH];
	statuses[O] = new bool[moleculeCountO];
	liveDuration = new int*[2];
	liveDuration[H] = new int[moleculeCountH];
	liveDuration[O] = new int[moleculeCountO];
	for (int i = 0; i < moleculeCountH; i++) {
		statuses[H][i] = false;
		liveDuration[H][i] = 0;
	}
	for (int i = 0; i < moleculeCountO; i++) {
		statuses[O][i] = false;
		liveDuration[O][i] = 0;
	}
		

	cout << "################# START #################" << endl;

	std::vector <std::thread> th_vecH;
	std::vector <std::thread> th_vecO;

	//Создать треды молекул
	for (int i = 0; i < moleculeCountH; i++)
		th_vecH.push_back(std::thread(molecule, i , H));
	for (int i = 0; i < moleculeCountO; i++)
		th_vecO.push_back(std::thread(molecule, i, O));

	//Жизненный цикл
	while (countMaxMolecules > 0)
	{
		int firstH = -1;
		int secondH = -1;
		int firstO = -1;
		for (int i = 0; i < moleculeCountH; i++)
			if (statuses[H][i]) {
				if (firstH == -1)firstH = i;
				else if (secondH == -1) {
					secondH = i;
					break;
				}
			}

		for (int i = 0; i < moleculeCountO; i++)
			if (statuses[O][i]) {
				if (firstO == -1) {
					firstO = i;
					break;
				}
			}

		//Если можем создать молекулу
		if (firstH != -1 && secondH != -1 && firstO != -1) {
			int liveTime = randomRange(10, 3);
			liveDuration[H][firstH] = liveTime;
			liveDuration[H][secondH] = liveTime;
			liveDuration[O][firstO] = liveTime;
			statuses[H][firstH] = false;
			statuses[H][secondH] = false;
			statuses[O][firstO] = false;

			cout << "Molecule H:" << firstH << " O:" << firstO << " H:" << secondH << " Live time:" << liveTime << endl;
			countMaxMolecules--;
		}

	}

	
	

	//Убить все треды молекул
	for (int i = 0; i < moleculeCountH; i++) {
		liveDuration[H][i] = -1;
		th_vecH[i].join();
	}
	for (int i = 0; i < moleculeCountO; i++) {
		liveDuration[O][i] = -1;
		th_vecO[i].join();
	}
	cout << "################# END #################" << endl;

}



