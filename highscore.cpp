#include "highscore.hpp"

void writeHighscore(int levelID, int score, int scoreGoal, int completionTime) {
	if (score >= scoreGoal && levelID > 0) {
		if (!std::filesystem::exists("Scores/" + std::to_string(levelID) + ".txt")) {
			std::ofstream fout("Scores/" + std::to_string(levelID) + ".txt");
			fout << "Level " << levelID << " Scores:" << std::endl;
			fout << (double)completionTime / 1000. << std::endl;
			fout.close();
		}
		else {
			std::ofstream fout("Scores/" + std::to_string(levelID) + ".txt", std::ios_base::app);
			fout << (double)completionTime / 1000. << std::endl;
			fout.close();
		}
		

		if (std::filesystem::exists("Highscores/" + std::to_string(levelID) + ".txt")) {
			std::ifstream fin("Highscores/" + std::to_string(levelID) + ".txt");
			double highscore;
			fin >> highscore;
			fin.close();
			if (highscore > (double)completionTime / 1000.) {
				std::ofstream fout("Highscores/" + std::to_string(levelID) + ".txt");
				fout << (double)completionTime / 1000. << " Level " << levelID << " Highscore" << std::endl;
				fout.close();
			}
		}
		else {
			std::ofstream fout("Highscores/" + std::to_string(levelID) + ".txt");
			fout << (double)completionTime / 1000. << " Level " << levelID << " Highscore" << std::endl;
			fout.close();
		}
	}
}