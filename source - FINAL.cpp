//============================================================================================
// Current version:				V0.3.4 Beta
// Updated date and time:		19/4/2017 21:15
// 
// Changelog
// V0.0 Beta			Release
// V0.1 Beta			Fix a bug that John discovered
// V0.2 Beta			Truly fix a bug that John discovered
// V0.2.1 Beta			"Player's sea" and "PC's sea" will aligned  
//						properly depends on sea_size
// V0.3 Beta			Add requirement R1.5 (not completed thoroughly) and
//						add R7 --- validation on user input (wrong data type, etc.)
// V0.3.1 Beta			Add a sentence in credits, arrnagement and battle stage to
//						remind player they can use "Quit" command
// V0.3.2 Beta			Change condition of adding probability upon hitting a ship body
// V0.3.3 Beta			Rewrite the grid to be changed in probability upon a ship is destroyed
// V0.3.4 Beta			Add R1.5 "other input is not acceptable..." requirement
//============================================================================================

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstring>

using namespace std;


char player_sea[10][10] = { '\0' };
char PC_sea[10][10] = { '\0' };
const int sizeCarrier = 5;
const int sizeBattleship = 4;
const int sizeSubmarine = 3;
const int sizeDestroyer = 2;

//=========Object used start here=========
class Carrier
{
public:
	Carrier()
	{
		valid = 0;
		placedDirection = '0';
		placedLocationRow = '0';
		placedLocationColumn = 0;
		for(int i = 0; i < sizeCarrier; i++)
		{
			health[i] = 0;
		}
		
	}

	void arrangement(bool inUse, char placedD, char placedLR, int placedLC)
	{
		valid = inUse;
		placedDirection = placedD;
		placedLocationRow = placedLR; //Y
		placedLocationColumn = placedLC; //X
		for(int i = 0; i < sizeCarrier; i++)
		{
			health[i] = 1;
		}
	}

	int attack_check(bool attacker, char attack_Y, int attack_X, char player_sea[][10], char PC_sea[][10], int sea_size, bool PC_sea_attacked[][10], bool player_sea_attacked[][10], int seaPCI[][10])
	{
		if(valid == 1)
		{
			int testSumHealth = 0;

			if(placedDirection == 'X')
			{
				if(attack_Y == placedLocationRow)
				{
					if( (placedLocationColumn <= attack_X) && (attack_X < placedLocationColumn + sizeCarrier))
					{
						if(health[attack_X - placedLocationColumn] == 0)
						{
							return 1;
							//cout << "The grid has been hit before. Nothing happens." << endl;
						}
						else
						{
							health[attack_X - placedLocationColumn]--;
						}

						for(int i = 0; i < sizeCarrier; i++)
						{
							testSumHealth += health[i];
						}

						if(testSumHealth == 0)
						{
							if(attacker == 0) //The player did that attack
							{
								PC_sea[(int)attack_Y-65][attack_X] = 'H';
								PC_sea_attacked[(int)attack_Y-65][attack_X] = 1;
							}
							else if(attacker == 1) // The PC did that attack
							{
								player_sea[(int)attack_Y-65][attack_X] = 'H';
								player_sea_attacked[(int)attack_Y-65][attack_X] = 1;

								//reset probability inside seaPCI
								if(placedLocationColumn != 0) // Left hand side < 0
									if(seaPCI[(int)placedLocationRow-65][placedLocationColumn-1] >= 25)
										seaPCI[(int)placedLocationRow-65][placedLocationColumn-1] -= 25;
								if(placedLocationColumn + sizeCarrier != sea_size ) // Right hand side > sea_size
									if(seaPCI[(int)placedLocationRow-65][placedLocationColumn+1] >= 25)
										seaPCI[(int)placedLocationRow-65][placedLocationColumn+1] -= 25;
								for(int i = 0; i < sizeCarrier; i++)
								{
									if((int)placedLocationRow - 65 != 0) //Upper head < 0
										if(seaPCI[(int)placedLocationRow-65-1][placedLocationColumn + i] >= 25)
											seaPCI[(int)placedLocationRow-65-1][placedLocationColumn + i] -= 25;
									if((int)placedLocationRow - 65 + 1 != sea_size) //Lower > sea_size
										if(seaPCI[(int)placedLocationRow-65+1][placedLocationColumn + i] >= 25)
											seaPCI[(int)placedLocationRow-65+1][placedLocationColumn + i] -= 25;
								}
							}
							return 3;
							//cout << "A carrier is sunk!" << endl;
						}
						else
						{
							if(attacker == 0) //The player did that attack
							{
								PC_sea[(int)attack_Y-65][attack_X] = 'H';
								PC_sea_attacked[(int)attack_Y-65][attack_X] = 1;
							}
							else if(attacker == 1) // The PC did that attack
							{
								player_sea[(int)attack_Y-65][attack_X] = 'H';
								player_sea_attacked[(int)attack_Y-65][attack_X] = 1;
							}
							return 2;
							//cout << "Part of a ship is destoryed!" << endl;
						}
					}
					else //cout << "It is a miss!" << endl;
					{
						if(attacker == 0) //The player did that attack
						{
							if(PC_sea_attacked[(int)attack_Y-65][attack_X] == 0)
							{
								PC_sea[(int)attack_Y-65][attack_X] = '*';
							}
						}
						else if(attacker == 1) // The PC did that attack
						{
							if(player_sea_attacked[(int)attack_Y-65][attack_X] == 0)
								player_sea[(int)attack_Y-65][attack_X] = '*';
						}
						return 0;
					}
				}
				else //cout << "It is a miss!" << endl;
				{
					if(attacker == 0) //The player did that attack
					{
						if(PC_sea_attacked[(int)attack_Y-65][attack_X] == 0) 
						{
							PC_sea[(int)attack_Y-65][attack_X] = '*';
						}
					}
					else if(attacker == 1) // The PC did that attack
					{
						if(player_sea_attacked[(int)attack_Y-65][attack_X] == 0)
							player_sea[(int)attack_Y-65][attack_X] = '*';
					}
					return 0;
				}
			}
			else if(placedDirection == 'Y')
			{
				if(attack_X == placedLocationColumn)
				{
					if((int)placedLocationRow <= (int)attack_Y && (int)attack_Y < (int)placedLocationRow + sizeCarrier)
					{
						if(health[(int)attack_Y - (int)placedLocationRow] == 0)
						{
							return 1;
							//cout << "The grid has been hit before. Nothing happens." << endl;
						}
						else
						{
							health[(int)attack_Y - (int)placedLocationRow]--;
						}

						for(int i = 0; i < sizeCarrier; i++)
						{
							testSumHealth += health[i];
						}

						if(testSumHealth == 0)
						{
							if(attacker == 0) //The player did that attack
							{
								PC_sea[(int)attack_Y-65][attack_X] = 'H';
								PC_sea_attacked[(int)attack_Y-65][attack_X] = 1;
							}
							else if(attacker == 1) // The PC did that attack
							{
								player_sea[(int)attack_Y-65][attack_X] = 'H';
								player_sea_attacked[(int)attack_Y-65][attack_X] = 1;

								//reset probability inside seaPCI
								if((int)placedLocationRow - 65 != 0) // Upper head < 0
									if(seaPCI[(int)placedLocationRow-65-1][placedLocationColumn] >= 25)
										seaPCI[(int)placedLocationRow-65-1][placedLocationColumn] -= 25;
								if((int)placedLocationRow + sizeCarrier - 65 != sea_size ) // Lower > sea_size
									if(seaPCI[(int)placedLocationRow-65+sizeCarrier][placedLocationColumn] >= 25)
										seaPCI[(int)placedLocationRow-65+sizeCarrier][placedLocationColumn] -= 25;
								for(int i = 0; i < sizeCarrier; i++)
								{
									if(placedLocationColumn != 0) //Left < 0
										if(seaPCI[(int)placedLocationRow - 65 + i][placedLocationColumn - 1] >= 25)
											seaPCI[(int)placedLocationRow - 65 + i][placedLocationColumn - 1] -= 25;
									if(placedLocationColumn + 1 != sea_size) //Right > sea_size
										if(seaPCI[(int)placedLocationRow - 65 + i][placedLocationColumn + 1] >= 25)
											seaPCI[(int)placedLocationRow - 65 + i][placedLocationColumn + 1] -= 25;
								}
							}
							return 3;
							//cout << "A carrier is sunk!" << endl;
						}
						else
						{
							if(attacker == 0) //The player did that attack
							{
								PC_sea[(int)attack_Y-65][attack_X] = 'H';
								PC_sea_attacked[(int)attack_Y-65][attack_X] = 1;
							}
							else if(attacker == 1) // The PC did that attack
							{
								player_sea[(int)attack_Y-65][attack_X] = 'H';
								player_sea_attacked[(int)attack_Y-65][attack_X] = 1;
							}
							return 2;
							//cout << "Part of a ship is destoryed!" << endl;
						}
					}
					else
					{
						if(attacker == 0) //The player did that attack
						{
							if(PC_sea_attacked[(int)attack_Y-65][attack_X] == 0)
							{
								PC_sea[(int)attack_Y-65][attack_X] = '*';
							}
						}
						else if(attacker == 1) // The PC did that attack
						{
							if(player_sea_attacked[(int)attack_Y-65][attack_X] == 0)
								player_sea[(int)attack_Y-65][attack_X] = '*';
						}
						return 0;
						//cout << "It is a miss!" << endl;
					}
				}
				else
				{
					if(attacker == 0) //The player did that attack
					{
						if(PC_sea_attacked[(int)attack_Y-65][attack_X] == 0)
						{

							PC_sea[(int)attack_Y-65][attack_X] = '*';
						}
					}
					else if(attacker == 1) // The PC did that attack
					{
						if(player_sea_attacked[(int)attack_Y-65][attack_X] == 0)
							player_sea[(int)attack_Y-65][attack_X] = '*';
					}
					return 0;
					//cout << "It is a miss!" << endl;
				}
			}
			else
			{
					return 0;
			}
		}
		return 0;
	}

private:
	bool valid;  // number of ship < current number = invalid (more than what user specify)
	char placedDirection; // either h for horizontal or v for vertical
	char placedLocationRow; // the row that the ship head is at (A~J) / Along the Y-axis
	int placedLocationColumn; // the column that the ship head is at (0~9) / Along the X-axis
	int health[sizeCarrier];
};

class Battleship
{
public:
	Battleship()
	{
		valid = 0;
		placedDirection = '0';
		placedLocationRow = '0';
		placedLocationColumn = 0;
		for(int i = 0; i < sizeBattleship; i++)
		{
			health[i] = 0;
		}
		
	}

	void arrangement(bool inUse, char placedD, char placedLR, int placedLC)
	{
		valid = inUse;
		placedDirection = placedD;
		placedLocationRow = placedLR; //Y
		placedLocationColumn = placedLC; //X
		for(int i = 0; i < sizeBattleship; i++)
		{
			health[i] = 1;
		}
	}

	int attack_check(bool attacker, char attack_Y, int attack_X, char player_sea[][10], char PC_sea[][10], int sea_size, bool PC_sea_attacked[][10], bool player_sea_attacked[][10], int seaPCI[][10])
	{
		if(valid == 1)
		{
			int testSumHealth = 0;

			if(placedDirection == 'X')
			{
				if(attack_Y == placedLocationRow)
				{
					if( (placedLocationColumn <= attack_X) && (attack_X < placedLocationColumn + sizeBattleship))
					{
						if(health[attack_X - placedLocationColumn] == 0)
						{
							return 1;
							//cout << "The grid has been hit before. Nothing happens." << endl;
						}
						else
						{
							health[attack_X - placedLocationColumn]--;
						}

						for(int i = 0; i < sizeBattleship; i++)
						{
							testSumHealth += health[i];
						}

						if(testSumHealth == 0)
						{
							if(attacker == 0) //The player did that attack
							{
								PC_sea[(int)attack_Y-65][attack_X] = 'H';
								PC_sea_attacked[(int)attack_Y-65][attack_X] = 1;
							}
							else if(attacker == 1) // The PC did that attack
							{
								player_sea[(int)attack_Y-65][attack_X] = 'H';
								player_sea_attacked[(int)attack_Y-65][attack_X] = 1;

								//reset probability inside seaPCI
								if(placedLocationColumn != 0) // Left hand side < 0
									if(seaPCI[(int)placedLocationRow-65][placedLocationColumn-1] >= 25)
										seaPCI[(int)placedLocationRow-65][placedLocationColumn-1] -= 25;
								if(placedLocationColumn + sizeBattleship != sea_size ) // Right hand side > sea_size
									if(seaPCI[(int)placedLocationRow-65][placedLocationColumn+1] >= 25)
										seaPCI[(int)placedLocationRow-65][placedLocationColumn+1] -= 25;
								for(int i = 0; i < sizeBattleship; i++)
								{
									if((int)placedLocationRow - 65 != 0) //Upper head < 0
										if(seaPCI[(int)placedLocationRow-65-1][placedLocationColumn + i] >= 25)
											seaPCI[(int)placedLocationRow-65-1][placedLocationColumn + i] -= 25;
									if((int)placedLocationRow - 65 + 1 != sea_size) //Lower head > sea_size
										if(seaPCI[(int)placedLocationRow-65+1][placedLocationColumn + i] >= 25)
											seaPCI[(int)placedLocationRow-65+1][placedLocationColumn + i] -= 25;
								}
							}
							return 3;
							//cout << "A battleship is sunk!" << endl;
						}
						else
						{
							if(attacker == 0) //The player did that attack
							{
								PC_sea[(int)attack_Y-65][attack_X] = 'H';
								PC_sea_attacked[(int)attack_Y-65][attack_X] = 1;
							}
							else if(attacker == 1) // The PC did that attack
							{
								player_sea[(int)attack_Y-65][attack_X] = 'H';
								player_sea_attacked[(int)attack_Y-65][attack_X] = 1;
							}
							return 2;
							//cout << "Part of a ship is destoryed!" << endl;
						}
					}
					else //cout << "It is a miss!" << endl;
					{
						if(attacker == 0) //The player did that attack
						{
							if(PC_sea_attacked[(int)attack_Y-65][attack_X] == 0)
							{
								PC_sea[(int)attack_Y-65][attack_X] = '*';
							}
						}
						else if(attacker == 1) // The PC did that attack
						{
							if(player_sea_attacked[(int)attack_Y-65][attack_X] == 0)
								player_sea[(int)attack_Y-65][attack_X] = '*';
						}
						return 0;
					}
				}
				else //cout << "It is a miss!" << endl;
				{
					if(attacker == 0) //The player did that attack
					{
						if(PC_sea_attacked[(int)attack_Y-65][attack_X] == 0) 
						{
							PC_sea[(int)attack_Y-65][attack_X] = '*';
						}
					}
					else if(attacker == 1) // The PC did that attack
					{
						if(player_sea_attacked[(int)attack_Y-65][attack_X] == 0)
							player_sea[(int)attack_Y-65][attack_X] = '*';
					}
					return 0;
				}
			}
			else if(placedDirection == 'Y')
			{
				if(attack_X == placedLocationColumn)
				{
					if((int)placedLocationRow <= (int)attack_Y && (int)attack_Y < (int)placedLocationRow + sizeBattleship)
					{
						if(health[(int)attack_Y - (int)placedLocationRow] == 0)
						{
							return 1;
							//cout << "The grid has been hit before. Nothing happens." << endl;
						}
						else
						{
							health[(int)attack_Y - (int)placedLocationRow]--;
						}

						for(int i = 0; i < sizeBattleship; i++)
						{
							testSumHealth += health[i];
						}

						if(testSumHealth == 0)
						{
							if(attacker == 0) //The player did that attack
							{
								PC_sea[(int)attack_Y-65][attack_X] = 'H';
								PC_sea_attacked[(int)attack_Y-65][attack_X] = 1;
							}
							else if(attacker == 1) // The PC did that attack
							{
								player_sea[(int)attack_Y-65][attack_X] = 'H';
								player_sea_attacked[(int)attack_Y-65][attack_X] = 1;

								//reset probability inside seaPCI
								if((int)placedLocationRow - 65 != 0) // Upper head < 0
									if(seaPCI[(int)placedLocationRow-65-1][placedLocationColumn] >= 25)
										seaPCI[(int)placedLocationRow-65-1][placedLocationColumn] -= 25;
								if((int)placedLocationRow + sizeBattleship - 65 != sea_size ) // Lower > sea_size
									if(seaPCI[(int)placedLocationRow-65+sizeBattleship][placedLocationColumn] >= 25)
										seaPCI[(int)placedLocationRow-65+sizeBattleship][placedLocationColumn] -= 25;
								for(int i = 0; i < sizeBattleship; i++)
								{
									if(placedLocationColumn != 0) //Left < 0
										if(seaPCI[(int)placedLocationRow - 65 + i][placedLocationColumn - 1] >= 25)
											seaPCI[(int)placedLocationRow - 65 + i][placedLocationColumn - 1] -= 25;
									if(placedLocationColumn + 1 != sea_size) //Right > sea_size
										if(seaPCI[(int)placedLocationRow - 65 + i][placedLocationColumn + 1] >= 25)
											seaPCI[(int)placedLocationRow - 65 + i][placedLocationColumn + 1] -= 25;
								}
							}
							return 3;
							//cout << "A battleship is sunk!" << endl;
						}
						else
						{
							if(attacker == 0) //The player did that attack
							{
								PC_sea[(int)attack_Y-65][attack_X] = 'H';
								PC_sea_attacked[(int)attack_Y-65][attack_X] = 1;
							}
							else if(attacker == 1) // The PC did that attack
							{
								player_sea[(int)attack_Y-65][attack_X] = 'H';
								player_sea_attacked[(int)attack_Y-65][attack_X] = 1;
							}
							return 2;
							//cout << "Part of a ship is destoryed!" << endl;
						}
					}
					else
					{
						if(attacker == 0) //The player did that attack
						{
							if(PC_sea_attacked[(int)attack_Y-65][attack_X] == 0)
							{
								PC_sea[(int)attack_Y-65][attack_X] = '*';
							}
						}
						else if(attacker == 1) // The PC did that attack
						{
							if(player_sea_attacked[(int)attack_Y-65][attack_X] == 0)
								player_sea[(int)attack_Y-65][attack_X] = '*';
						}
						return 0;
						//cout << "It is a miss!" << endl;
					}
				}
				else
				{
					if(attacker == 0) //The player did that attack
					{
						if(PC_sea_attacked[(int)attack_Y-65][attack_X] == 0)
						{

							PC_sea[(int)attack_Y-65][attack_X] = '*';
						}
					}
					else if(attacker == 1) // The PC did that attack
					{
						if(player_sea_attacked[(int)attack_Y-65][attack_X] == 0)
							player_sea[(int)attack_Y-65][attack_X] = '*';
					}
					return 0;
					//cout << "It is a miss!" << endl;
				}
			}
			else
			{
					return 0;
			}
		}
		return 0;
	}

private:
	bool valid;  // number of ship < current number = invalid (more than what user specify)
	char placedDirection; // either h for horizontal or v for vertical
	char placedLocationRow; // the row that the ship head is at (A~J) / Along the Y-axis
	int placedLocationColumn; // the column that the ship head is at (0~9) / Along the X-axis
	int health[sizeBattleship];
};

class Submarine
{
public:
	Submarine()
	{
		valid = 0;
		placedDirection = '0';
		placedLocationRow = '0';
		placedLocationColumn = 0;
		for(int i = 0; i < sizeSubmarine; i++)
		{
			health[i] = 0;
		}
		
	}

	void arrangement(bool inUse, char placedD, char placedLR, int placedLC)
	{
		valid = inUse;
		placedDirection = placedD;
		placedLocationRow = placedLR; //Y
		placedLocationColumn = placedLC; //X
		for(int i = 0; i < sizeSubmarine; i++)
		{
			health[i] = 1;
		}
	}

	int attack_check(bool attacker, char attack_Y, int attack_X, char player_sea[][10], char PC_sea[][10], int sea_size, bool PC_sea_attacked[][10], bool player_sea_attacked[][10], int seaPCI[][10])
	{
		if(valid == 1)
		{
			int testSumHealth = 0;

			if(placedDirection == 'X')
			{
				if(attack_Y == placedLocationRow)
				{
					if( (placedLocationColumn <= attack_X) && (attack_X < placedLocationColumn + sizeSubmarine))
					{
						if(health[attack_X - placedLocationColumn] == 0)
						{
							return 1;
							//cout << "The grid has been hit before. Nothing happens." << endl;
						}
						else
						{
							health[attack_X - placedLocationColumn]--;
						}

						for(int i = 0; i < sizeSubmarine; i++)
						{
							testSumHealth += health[i];
						}

						if(testSumHealth == 0)
						{
							if(attacker == 0) //The player did that attack
							{
								PC_sea[(int)attack_Y-65][attack_X] = 'H';
								PC_sea_attacked[(int)attack_Y-65][attack_X] = 1;
							}
							else if(attacker == 1) // The PC did that attack
							{
								player_sea[(int)attack_Y-65][attack_X] = 'H';
								player_sea_attacked[(int)attack_Y-65][attack_X] = 1;

								//reset probability inside seaPCI
								if(placedLocationColumn != 0) // Left hand side < 0
									if(seaPCI[(int)placedLocationRow-65][placedLocationColumn-1] >= 25)
										seaPCI[(int)placedLocationRow-65][placedLocationColumn-1] -= 25;
								if(placedLocationColumn + sizeSubmarine != sea_size ) // Right hand side > sea_size
									if(seaPCI[(int)placedLocationRow-65][placedLocationColumn+1] >= 25)
										seaPCI[(int)placedLocationRow-65][placedLocationColumn+1] -= 25;
								for(int i = 0; i < sizeSubmarine; i++)
								{
									if((int)placedLocationRow - 65 != 0) //Upper head < 0
										if(seaPCI[(int)placedLocationRow-65-1][placedLocationColumn + i] >= 25)
											seaPCI[(int)placedLocationRow-65-1][placedLocationColumn + i] -= 25;
									if((int)placedLocationRow - 65 + 1 != sea_size) //Lower head > sea_size
										if(seaPCI[(int)placedLocationRow-65+1][placedLocationColumn + i] >= 25)
											seaPCI[(int)placedLocationRow-65+1][placedLocationColumn + i] -= 25;
								}
							}
							return 3;
							//cout << "A submarine is sunk!" << endl;
						}
						else
						{
							if(attacker == 0) //The player did that attack
							{
								PC_sea[(int)attack_Y-65][attack_X] = 'H';
								PC_sea_attacked[(int)attack_Y-65][attack_X] = 1;
							}
							else if(attacker == 1) // The PC did that attack
							{
								player_sea[(int)attack_Y-65][attack_X] = 'H';
								player_sea_attacked[(int)attack_Y-65][attack_X] = 1;
							}
							return 2;
							//cout << "Part of a ship is destoryed!" << endl;
						}
					}
					else //cout << "It is a miss!" << endl;
					{
						if(attacker == 0) //The player did that attack
						{
							if(PC_sea_attacked[(int)attack_Y-65][attack_X] == 0)
							{
								PC_sea[(int)attack_Y-65][attack_X] = '*';
							}
						}
						else if(attacker == 1) // The PC did that attack
						{
							if(player_sea_attacked[(int)attack_Y-65][attack_X] == 0)
								player_sea[(int)attack_Y-65][attack_X] = '*';
						}
						return 0;
					}
				}
				else //cout << "It is a miss!" << endl;
				{
					if(attacker == 0) //The player did that attack
					{
						if(PC_sea_attacked[(int)attack_Y-65][attack_X] == 0) 
						{
							PC_sea[(int)attack_Y-65][attack_X] = '*';
						}
					}
					else if(attacker == 1) // The PC did that attack
					{
						if(player_sea_attacked[(int)attack_Y-65][attack_X] == 0)
							player_sea[(int)attack_Y-65][attack_X] = '*';
					}
					return 0;
				}
			}
			else if(placedDirection == 'Y')
			{
				if(attack_X == placedLocationColumn)
				{
					if((int)placedLocationRow <= (int)attack_Y && (int)attack_Y < (int)placedLocationRow + sizeSubmarine)
					{
						if(health[(int)attack_Y - (int)placedLocationRow] == 0)
						{
							return 1;
							//cout << "The grid has been hit before. Nothing happens." << endl;
						}
						else
						{
							health[(int)attack_Y - (int)placedLocationRow]--;
						}

						for(int i = 0; i < sizeSubmarine; i++)
						{
							testSumHealth += health[i];
						}

						if(testSumHealth == 0)
						{
							if(attacker == 0) //The player did that attack
							{
								PC_sea[(int)attack_Y-65][attack_X] = 'H';
								PC_sea_attacked[(int)attack_Y-65][attack_X] = 1;
							}
							else if(attacker == 1) // The PC did that attack
							{
								player_sea[(int)attack_Y-65][attack_X] = 'H';
								player_sea_attacked[(int)attack_Y-65][attack_X] = 1;

								//reset probability inside seaPCI
								if((int)placedLocationRow - 65 != 0) // Upper head < 0
									if(seaPCI[(int)placedLocationRow-65-1][placedLocationColumn] >= 25)
										seaPCI[(int)placedLocationRow-65-1][placedLocationColumn] -= 25;
								if((int)placedLocationRow + sizeSubmarine - 65 != sea_size ) // Lower > sea_size
									if(seaPCI[(int)placedLocationRow-65+sizeSubmarine][placedLocationColumn] >= 25)
										seaPCI[(int)placedLocationRow-65+sizeSubmarine][placedLocationColumn] -= 25;
								for(int i = 0; i < sizeSubmarine; i++)
								{
									if(placedLocationColumn != 0) //Left < 0
										if(seaPCI[(int)placedLocationRow - 65 + i][placedLocationColumn - 1] >= 25)
											seaPCI[(int)placedLocationRow - 65 + i][placedLocationColumn - 1] -= 25;
									if(placedLocationColumn + 1 != sea_size) //Right > sea_size
										if(seaPCI[(int)placedLocationRow - 65 + i][placedLocationColumn + 1] >= 25)
											seaPCI[(int)placedLocationRow - 65 + i][placedLocationColumn + 1] -= 25;
								}
							}
							return 3;
							//cout << "A battleship is sunk!" << endl;
						}
						else
						{
							if(attacker == 0) //The player did that attack
							{
								PC_sea[(int)attack_Y-65][attack_X] = 'H';
								PC_sea_attacked[(int)attack_Y-65][attack_X] = 1;
							}
							else if(attacker == 1) // The PC did that attack
							{
								player_sea[(int)attack_Y-65][attack_X] = 'H';
								player_sea_attacked[(int)attack_Y-65][attack_X] = 1;
							}
							return 2;
							//cout << "Part of a ship is destoryed!" << endl;
						}
					}
					else
					{
						if(attacker == 0) //The player did that attack
						{
							if(PC_sea_attacked[(int)attack_Y-65][attack_X] == 0)
							{
								PC_sea[(int)attack_Y-65][attack_X] = '*';
							}
						}
						else if(attacker == 1) // The PC did that attack
						{
							if(player_sea_attacked[(int)attack_Y-65][attack_X] == 0)
								player_sea[(int)attack_Y-65][attack_X] = '*';
						}
						return 0;
						//cout << "It is a miss!" << endl;
					}
				}
				else
				{
					if(attacker == 0) //The player did that attack
					{
						if(PC_sea_attacked[(int)attack_Y-65][attack_X] == 0)
						{

							PC_sea[(int)attack_Y-65][attack_X] = '*';
						}
					}
					else if(attacker == 1) // The PC did that attack
					{
						if(player_sea_attacked[(int)attack_Y-65][attack_X] == 0)
							player_sea[(int)attack_Y-65][attack_X] = '*';
					}
					return 0;
					//cout << "It is a miss!" << endl;
				}
			}
			else
			{
					return 0;
			}
		}
		return 0;
	}

private:
	bool valid;  // number of ship < current number = invalid (more than what user specify)
	char placedDirection; // either h for horizontal or v for vertical
	char placedLocationRow; // the row that the ship head is at (A~J) / Along the Y-axis
	int placedLocationColumn; // the column that the ship head is at (0~9) / Along the X-axis
	int health[sizeSubmarine];
};

class Destroyer
{
public:
	Destroyer()
	{
		valid = 0;
		placedDirection = '0';
		placedLocationRow = '0';
		placedLocationColumn = 0;
		for(int i = 0; i < sizeDestroyer; i++)
		{
			health[i] = 0;
		}
		
	}

	void arrangement(bool inUse, char placedD, char placedLR, int placedLC)
	{
		valid = inUse;
		placedDirection = placedD;
		placedLocationRow = placedLR; //Y
		placedLocationColumn = placedLC; //X
		for(int i = 0; i < sizeDestroyer; i++)
		{
			health[i] = 1;
		}
	}

	int attack_check(bool attacker, char attack_Y, int attack_X, char player_sea[][10], char PC_sea[][10], int sea_size, bool PC_sea_attacked[][10], bool player_sea_attacked[][10], int seaPCI[][10])
	{
		if(valid == 1)
		{
			int testSumHealth = 0;

			if(placedDirection == 'X')
			{
				if(attack_Y == placedLocationRow)
				{
					if( (placedLocationColumn <= attack_X) && (attack_X < placedLocationColumn + sizeDestroyer))
					{
						if(health[attack_X - placedLocationColumn] == 0)
						{
							return 1;
							//cout << "The grid has been hit before. Nothing happens." << endl;
						}
						else
						{
							health[attack_X - placedLocationColumn]--;
						}

						for(int i = 0; i < sizeDestroyer; i++)
						{
							testSumHealth += health[i];
						}

						if(testSumHealth == 0)
						{
							if(attacker == 0) //The player did that attack
							{
								PC_sea[(int)attack_Y-65][attack_X] = 'H';
								PC_sea_attacked[(int)attack_Y-65][attack_X] = 1;
							}
							else if(attacker == 1) // The PC did that attack
							{
								player_sea[(int)attack_Y-65][attack_X] = 'H';
								player_sea_attacked[(int)attack_Y-65][attack_X] = 1;

								//reset probability inside seaPCI
								if(placedLocationColumn != 0) // Left hand side < 0
									if(seaPCI[(int)placedLocationRow-65][placedLocationColumn-1] >= 25)
										seaPCI[(int)placedLocationRow-65][placedLocationColumn-1] -= 25;
								if(placedLocationColumn + sizeDestroyer != sea_size ) // Right hand side > sea_size
									if(seaPCI[(int)placedLocationRow-65][placedLocationColumn+1] >= 25)
										seaPCI[(int)placedLocationRow-65][placedLocationColumn+1] -= 25;
								for(int i = 0; i < sizeDestroyer; i++)
								{
									if((int)placedLocationRow - 65 != 0) //Upper head < 0
										if(seaPCI[(int)placedLocationRow-65-1][placedLocationColumn + i] >= 25)
											seaPCI[(int)placedLocationRow-65-1][placedLocationColumn + i] -= 25;
									if((int)placedLocationRow - 65 + 1 != sea_size) //Lower head > sea_size
										if(seaPCI[(int)placedLocationRow-65+1][placedLocationColumn + i] >= 25)
											seaPCI[(int)placedLocationRow-65+1][placedLocationColumn + i] -= 25;
								}
							}
							return 3;
							//cout << "A destroyer is sunk!" << endl;
						}
						else
						{
							if(attacker == 0) //The player did that attack
							{
								PC_sea[(int)attack_Y-65][attack_X] = 'H';
								PC_sea_attacked[(int)attack_Y-65][attack_X] = 1;
							}
							else if(attacker == 1) // The PC did that attack
							{
								player_sea[(int)attack_Y-65][attack_X] = 'H';
								player_sea_attacked[(int)attack_Y-65][attack_X] = 1;
							}
							return 2;
							//cout << "Part of a ship is destoryed!" << endl;
						}
					}
					else //cout << "It is a miss!" << endl;
					{
						if(attacker == 0) //The player did that attack
						{
							if(PC_sea_attacked[(int)attack_Y-65][attack_X] == 0)
							{
								PC_sea[(int)attack_Y-65][attack_X] = '*';
							}
						}
						else if(attacker == 1) // The PC did that attack
						{
							if(player_sea_attacked[(int)attack_Y-65][attack_X] == 0)
								player_sea[(int)attack_Y-65][attack_X] = '*';
						}
						return 0;
					}
				}
				else //cout << "It is a miss!" << endl;
				{
					if(attacker == 0) //The player did that attack
					{
						if(PC_sea_attacked[(int)attack_Y-65][attack_X] == 0) 
						{
							PC_sea[(int)attack_Y-65][attack_X] = '*';
						}
					}
					else if(attacker == 1) // The PC did that attack
					{
						if(player_sea_attacked[(int)attack_Y-65][attack_X] == 0)
							player_sea[(int)attack_Y-65][attack_X] = '*';
					}
					return 0;
				}
			}
			else if(placedDirection == 'Y')
			{
				if(attack_X == placedLocationColumn)
				{
					if((int)placedLocationRow <= (int)attack_Y && (int)attack_Y < (int)placedLocationRow + sizeDestroyer)
					{
						if(health[(int)attack_Y - (int)placedLocationRow] == 0)
						{
							return 1;
							//cout << "The grid has been hit before. Nothing happens." << endl;
						}
						else
						{
							health[(int)attack_Y - (int)placedLocationRow]--;
						}

						for(int i = 0; i < sizeDestroyer; i++)
						{
							testSumHealth += health[i];
						}

						if(testSumHealth == 0)
						{
							if(attacker == 0) //The player did that attack
							{
								PC_sea[(int)attack_Y-65][attack_X] = 'H';
								PC_sea_attacked[(int)attack_Y-65][attack_X] = 1;
							}
							else if(attacker == 1) // The PC did that attack
							{
								player_sea[(int)attack_Y-65][attack_X] = 'H';
								player_sea_attacked[(int)attack_Y-65][attack_X] = 1;

								//reset probability inside seaPCI
								if((int)placedLocationRow - 65 != 0) // Upper head < 0
									if(seaPCI[(int)placedLocationRow-65-1][placedLocationColumn] >= 25)
										seaPCI[(int)placedLocationRow-65-1][placedLocationColumn] -= 25;
								if((int)placedLocationRow + sizeDestroyer - 65 != sea_size ) // Lower > sea_size
									if(seaPCI[(int)placedLocationRow-65+sizeDestroyer][placedLocationColumn] >= 25)
										seaPCI[(int)placedLocationRow-65+sizeDestroyer][placedLocationColumn] -= 25;
								for(int i = 0; i < sizeDestroyer; i++)
								{
									if(placedLocationColumn != 0) //Left < 0
										if(seaPCI[(int)placedLocationRow - 65 + i][placedLocationColumn - 1] >= 25)
											seaPCI[(int)placedLocationRow - 65 + i][placedLocationColumn - 1] -= 25;
									if(placedLocationColumn + 1 != sea_size) //Right > sea_size
										if(seaPCI[(int)placedLocationRow - 65 + i][placedLocationColumn + 1] >= 25)
											seaPCI[(int)placedLocationRow - 65 + i][placedLocationColumn + 1] -= 25;
								}
							}
							return 3;
							//cout << "A destroyer is sunk!" << endl;
						}
						else
						{
							if(attacker == 0) //The player did that attack
							{
								PC_sea[(int)attack_Y-65][attack_X] = 'H';
								PC_sea_attacked[(int)attack_Y-65][attack_X] = 1;
							}
							else if(attacker == 1) // The PC did that attack
							{
								player_sea[(int)attack_Y-65][attack_X] = 'H';
								player_sea_attacked[(int)attack_Y-65][attack_X] = 1;
							}
							return 2;
							//cout << "Part of a ship is destoryed!" << endl;
						}
					}
					else
					{
						if(attacker == 0) //The player did that attack
						{
							if(PC_sea_attacked[(int)attack_Y-65][attack_X] == 0)
							{
								PC_sea[(int)attack_Y-65][attack_X] = '*';
							}
						}
						else if(attacker == 1) // The PC did that attack
						{
							if(player_sea_attacked[(int)attack_Y-65][attack_X] == 0)
								player_sea[(int)attack_Y-65][attack_X] = '*';
						}
						return 0;
						//cout << "It is a miss!" << endl;
					}
				}
				else
				{
					if(attacker == 0) //The player did that attack
					{
						if(PC_sea_attacked[(int)attack_Y-65][attack_X] == 0)
						{

							PC_sea[(int)attack_Y-65][attack_X] = '*';
						}
					}
					else if(attacker == 1) // The PC did that attack
					{
						if(player_sea_attacked[(int)attack_Y-65][attack_X] == 0)
							player_sea[(int)attack_Y-65][attack_X] = '*';
					}
					return 0;
					//cout << "It is a miss!" << endl;
				}
			}
			else
			{
					return 0;
			}
		}
		return 0;
	}

private:
	bool valid;  // number of ship < current number = invalid (more than what user specify)
	char placedDirection; // either h for horizontal or v for vertical
	char placedLocationRow; // the row that the ship head is at (A~J) / Along the Y-axis
	int placedLocationColumn; // the column that the ship head is at (0~9) / Along the X-axis
	int health[sizeDestroyer];
};
//=========Object used end here=========

//===Function protypes start here===
void game(char player_sea[][10], char PC_sea[][10], bool WhoStartFirst, bool DisplayPCShips, int sea_size, int numCarrier, int numBattleship, int numSubmarine, int numDestroyer);

void game_arrangement_player(char player_sea[][10], int sea_size, int numCarrier, int numBattleship, int numSubmarine, int numDestroyer, Carrier PlayerCarrier[5], Battleship PlayerBattleship[5], Submarine PlayerSubmarine[5], Destroyer PlayerDestroyer[5]);
void game_arrangement_PC(char PC_sea[][10], int sea_size, int numCarrier, int numBattleship, int numSubmarine, int numDestroyer, Carrier PCCarrier[5], Battleship PCBattleship[5], Submarine PCSubmarine[5], Destroyer PCDestroyer[5]);
void game_arrangement_instruction(int sea_size);

void game_printSea_player(int sea_size);
void game_printSea_PC(int sea_size, bool DisplayPCShips);

void game_battlestage(char player_sea[][10], char PC_sea[][10], bool WhoStartFirst, bool DisplayPCShips, int sea_size, int numCarrier, int numBattleship, int numSubmarine, int numDestroyer, Carrier PlayerCarrier[5], Battleship PlayerBattleship[5], Submarine PlayerSubmarine[5], Destroyer PlayerDestroyer[5], Carrier PCCarrier[5], Battleship PCBattleship[5], Submarine PCSubmarine[5], Destroyer PCDestroyer[5]);
void game_printSea_battlestage(int sea_size, bool DisplayPCShips);
void game_quit();

void PCI_decide_attack(int size, int seaPCI[][10], int &, int &, int &, int &, int &);
int PCI_calculate_Pro(int size, int pro, int seaPCI[][10]);

void PCI_attack_phrase(int size, int seaPCI[][10], int numUnknownPro, int numLowPro, int numMedPro, int numHighPro, int numVHighPro, char &attack_Y, int &attack_X);
void PCI_fire(int size, int pro, int numPro, int seaPCI[][10], char &attack_Y, int &attack_X);

void setting(bool &WhoStartFirst, bool &DisplayPCShips, int &sea_size, int &numCarrier, int &numBattleship, int &numSubmarine, int &numDestroyer);
void whoFirst(bool &WhoStartFirst);
void displayPCships(bool &DisplayPCShips);
void seaSize(int &sea_size, int numCarrier, int numBattleship, int numSubmarine, int numDestroyer);
void numberOFships(int sea_size, int &numCarrier, int &numBattleship, int &numSubmarine, int &numDestroyer);

void instructions();

void credits();
//===Function protypes end here===

int main()
{

	bool WhoStartFirst = 0;
	bool DisplayPCShips = 0;
	int sea_size = 10;
	int numCarrier = 1;
	int numBattleship = 2;
	int numSubmarine = 3;
	int numDestroyer = 4;

	int prog_choice;

	MAIN_MENU:
	do {
		system("cls");
		cout << right << endl;
		cout << "Welcome to the Battleship game!" << endl;
		cout << "We hope you enjoy it :)" << endl;

		cout << "\n";
		cout << "*** Game Menu ***" << endl;
		cout << "[1] Start Game" << endl;
		cout << "[2] Settings" << endl;
		cout << "[3] Instructions" << endl;
		cout << "[4] Credits" << endl;
		cout << "[5] Exit" << endl;
		cout << "*****************" << endl;
		cout << "Option (1 - 5): ";
		cin >> prog_choice;
		switch (prog_choice)
		{
		case 1: game(player_sea, PC_sea, WhoStartFirst, DisplayPCShips, sea_size, numCarrier, numBattleship, numSubmarine, numDestroyer); break;
		case 2: setting(WhoStartFirst, DisplayPCShips, sea_size, numCarrier, numBattleship, numSubmarine, numDestroyer); break;
		case 3: instructions(); break;
		case 4: credits(); break;
		case 5: {
			char ans;

			cout << "Are you sure to leave?" << endl;
			cout << "If yes, please input 'y'/'Y'." << endl;
			cout << "If no, please input 'n'/'N'." << endl;
			cin >> ans;

			if (ans == 'y' || ans == 'Y')
				cout << "Good Bye!" << endl;
			else if (ans == 'n' || ans == 'N')
				goto MAIN_MENU;
			else
			{
				cout << "Wrong input! The program will not terminate." << endl;
				system("pause");
			}
		}	break;
		default:
			cout << "Please enter choice 1 - 5 only." << endl;
			system("pause");
			break;
		}
		cin.clear();
		cin.ignore(10000, '\n');
	} while (prog_choice != 5);
	return 0;
}

void game(char player_sea[][10], char PC_sea[][10], bool WhoStartFirst, bool DisplayPCShips, int sea_size, int numCarrier, int numBattleship, int numSubmarine, int numDestroyer)
{
	
	Carrier PlayerCarrier[5];
	Battleship PlayerBattleship[5];
	Submarine PlayerSubmarine[5];
	Destroyer PlayerDestroyer[5];

	Carrier PCCarrier[5];
	Battleship PCBattleship[5];
	Submarine PCSubmarine[5];
	Destroyer PCDestroyer[5];
	

	system("cls");
	game_arrangement_player(player_sea, sea_size, numCarrier, numBattleship, numSubmarine, numDestroyer, PlayerCarrier, PlayerBattleship, PlayerSubmarine, PlayerDestroyer); // Player place ship first
	game_arrangement_PC(PC_sea, sea_size, numCarrier, numBattleship, numSubmarine, numDestroyer, PCCarrier, PCBattleship, PCSubmarine, PCDestroyer); //PC arrange ship randomly in this function

	game_battlestage(player_sea, PC_sea, WhoStartFirst, DisplayPCShips, sea_size, numCarrier, numBattleship, numSubmarine, numDestroyer, PlayerCarrier, PlayerBattleship, PlayerSubmarine, PlayerDestroyer, PCCarrier, PCBattleship, PCSubmarine, PCDestroyer);
}

void game_arrangement_player(char player_sea[][10], int sea_size, int numCarrier, int numBattleship, int numSubmarine, int numDestroyer, Carrier PlayerCarrier[5], Battleship PlayerBattleship[5], Submarine PlayerSubmarine[5], Destroyer PlayerDestroyer[5])
{
	/*
	Carrier PlayerCarrier[5];
	Battleship PlayerBattleship[5];
	Submarine PlayerSubmarine[5];
	Destroyer PlayerDestroyer[5];
	*/

	char direction, y; // direction means the ship is put vertically or horizontally, x is the coordinate of x-axis, y is the coordinate of y-axis
	int x;

	//Arrange carrier
	for (int i = 1; i <= numCarrier; i++) {
		game_printSea_player(sea_size);
		game_arrangement_instruction(sea_size);
		cout << "\n(" << i << " out of " << numCarrier << ") Please input the coordinate of the carrier (size of " << sizeCarrier << "): ";
		cin >> direction >> y >> x;	

		if ((direction == 'Q') && (y == 'u') && (x = 'i'))
		{
			game_quit();
			i--;
			continue;
		}

		if (direction == 'X') {  // X means ship place towards x-axis/horizontally
			while ((int)y < 65 || (int)y > 64 + sea_size || x < 0 || x > sea_size - 1 || x >= sea_size - 4 || player_sea[(int)y - 65][x] == 'O' || player_sea[(int)y - 65][x + 1] == 'O' || player_sea[(int)y - 65][x + 2] == 'O' || player_sea[(int)y - 65][x + 3] == 'O' || player_sea[(int)y - 65][x + 4] == 'O') {  // Prevent wrong input
				cout << "Wrong input! Try again." << endl;

				cin.clear();
				cin.ignore(10000, '\n');

				system("pause");
				system("cls");

				game_printSea_player(sea_size);
				game_arrangement_instruction(sea_size);
				cout << "\n(" << i << " out of " << numCarrier << ") Please input the coordinate of the carrier (size of " << sizeCarrier << "): ";
				cin >> direction >> y >> x;
			}
			for (int i = 0; i < sizeCarrier; i++) {
				player_sea[(int)y - 65][x + i] = 'O';
			}
		}
		else if (direction == 'Y') { // Y means ship place towards y-axis/vertically
			while ((int)y < 65 || (int)y > 64 + sea_size || x < 0 || x > sea_size - 1 || y >= (char)(sea_size + 61) || player_sea[(int)y - 65][x] == 'O' || player_sea[(int)y - 64][x] == 'O' || player_sea[(int)y - 63][x] == 'O' || player_sea[(int)y - 62][x] == 'O' || player_sea[(int)y - 61][x] == 'O') { // Prevent wrong input
				cout << "Wrong input! Try again." << endl;
				
				cin.clear();
				cin.ignore(10000, '\n');

				system("pause");
				system("cls");

				game_printSea_player(sea_size);
				game_arrangement_instruction(sea_size);
				cout << "\n(" << i << " out of " << numCarrier << ") Please input the coordinate of the carrier (size of " << sizeCarrier << "): ";
				cin >> direction >> y >> x;
			}
			for (int i = 0; i < sizeCarrier; i++) {
				player_sea[(int)y - 65 + i][x] = 'O';
			}
		}
		if(direction == 'X' || direction == 'Y') 
		{
			PlayerCarrier[i-1].arrangement(1, direction, y, x);
		}
		else // Neither X nor Y
		{
			cout << "Wrong input! Try again." << endl;
			
			cin.clear();
			cin.ignore(10000, '\n');

			system("pause");
			system("cls");
			i--;
			continue;
		}

		for(int i = numCarrier; i <= 4; i++) // For carrier not in use
			PlayerCarrier[i].arrangement(0, '0', '0', -1);

		cout << endl;
		system("cls");
	}

	// Arrange Battleship
	for (int i = 1; i <= numBattleship; i++) {
		game_printSea_player(sea_size);
		game_arrangement_instruction(sea_size);
		cout << "\n(" << i << " out of " << numBattleship << ") Please input the coordinate of the battleship (size of " << sizeBattleship << "): ";
		cin >> direction >> y >> x;

		if ((direction == 'Q') && (y == 'u') && (x = 'i'))
		{
			game_quit();
			i--;
			continue;
		}

		if (direction == 'X') {  // X means ship place towards x-axis/horizontally
			while ((int)y < 65 || (int)y > 64 + sea_size || x < 0 || x > sea_size - 1 || x >= sea_size - 3 || player_sea[(int)y - 65][x] == 'O' || player_sea[(int)y - 65][x + 1] == 'O' || player_sea[(int)y - 65][x + 2] == 'O' || player_sea[(int)y - 65][x + 3] == 'O') {  // Prevent wrong input
				cout << "Wrong input! Try again." << endl;
				
				cin.clear();
				cin.ignore(10000, '\n');

				system("pause");
				system("cls");

				game_printSea_player(sea_size);
				game_arrangement_instruction(sea_size);
				cout << "\n(" << i << " out of " << numBattleship << ") Please input the coordinate of the battleship (size of " << sizeBattleship << "): ";
				cin >> direction >> y >> x;
			}
			for (int i = 0; i < sizeBattleship; i++) {
				player_sea[(int)y - 65][x + i] = 'O';
			}
		}
		else if (direction == 'Y') { // Y means ship place towards y-axis/vertically
			while ((int)y < 65 || (int)y > 64 + sea_size || x < 0 || x > sea_size - 1 || y >= (char)(sea_size + 62) || player_sea[(int)y - 65][x] == 'O' || player_sea[(int)y - 64][x] == 'O' || player_sea[(int)y - 63][x] == 'O' || player_sea[(int)y - 62][x] == 'O') { // Prevent wrong input
				cout << "Wrong input! Try again." << endl;
				
				cin.clear();
				cin.ignore(10000, '\n');

				system("pause");
				system("cls");

				game_printSea_player(sea_size);
				game_arrangement_instruction(sea_size);
				cout << "\n(" << i << " out of " << numBattleship << ") Please input the coordinate of the battleship (size of " << sizeBattleship << "): ";
				cin >> direction >> y >> x;
			}
			for (int i = 0; i < sizeBattleship; i++) {
				player_sea[(int)y - 65 + i][x] = 'O';
			}
		}
		if(direction == 'X' || direction == 'Y') 
		{
			PlayerBattleship[i-1].arrangement(1, direction, y, x);
		}
		else // Neither X nor Y
		{
			cout << "Wrong input! Try again." << endl;
				
			cin.clear();
			cin.ignore(10000, '\n');

			system("pause");
			system("cls");
			i--;
			continue;
		}

		for(int i = numBattleship; i <= 4; i++) // For battleship not in use
			PlayerBattleship[i].arrangement(0, '0', '0', -1);

		cout << endl;
		system("cls");
	}

	// Arrange Submarine
	for (int i = 1; i <= numSubmarine; i++) {
		game_printSea_player(sea_size);
		game_arrangement_instruction(sea_size);
		cout << "\n(" << i << " out of " << numSubmarine << ") Please input the coordinate of the submarine (size of " << sizeSubmarine << "): ";
		cin >> direction >> y >> x;

		if ((direction == 'Q') && (y == 'u') && (x = 'i'))
		{
			game_quit();
			i--;
			continue;
		}

		if (direction == 'X') {  // X means ship place towards x-axis/horizontally
			while ((int)y < 65 || (int)y > 64 + sea_size || x < 0 || x > sea_size - 1 || x >= sea_size - 2 || player_sea[(int)y - 65][x] == 'O' || player_sea[(int)y - 65][x + 1] == 'O' || player_sea[(int)y - 65][x + 2] == 'O') {  // Prevent wrong input
				cout << "Wrong input! Try again." << endl;
				
				cin.clear();
				cin.ignore(10000, '\n');

				system("pause");
				system("cls");

				game_printSea_player(sea_size);
				game_arrangement_instruction(sea_size);
				cout << "\n(" << i << " out of " << numSubmarine << ") Please input the coordinate of the submarine (size of " << sizeSubmarine << "): ";
				cin >> direction >> y >> x;
			}
			for (int i = 0; i < sizeSubmarine; i++) {
				player_sea[(int)y - 65][x + i] = 'O';
			}
		}
		else if (direction == 'Y') { // Y means ship place towards y-axis/vertically
			while ((int)y < 65 || (int)y > 64 + sea_size || x < 0 || x > sea_size - 1 || y >= (char)(sea_size + 63) || player_sea[(int)y - 65][x] == 'O' || player_sea[(int)y - 64][x] == 'O' || player_sea[(int)y - 63][x] == 'O') { // Prevent wrong input
				cout << "Wrong input! Try again." << endl;
				
				cin.clear();
				cin.ignore(10000, '\n');

				system("pause");
				system("cls");

				game_printSea_player(sea_size);
				game_arrangement_instruction(sea_size);
				cout << "\n(" << i << " out of " << numSubmarine << ") Please input the coordinate of the submarine (size of " << sizeSubmarine << "): ";
				cin >> direction >> y >> x;
			}
			for (int i = 0; i < sizeSubmarine; i++) {
				player_sea[(int)y - 65 + i][x] = 'O';
			}
		}
		if(direction == 'X' || direction == 'Y') 
		{
			PlayerSubmarine[i-1].arrangement(1, direction, y, x);
		}
		else // Neither X nor Y
		{
			cout << "Wrong input! Try again." << endl;
				
			cin.clear();
			cin.ignore(10000, '\n');

			system("pause");
			system("cls");
			i--;
			continue;
		}

		for(int i = numSubmarine; i <= 4; i++) // For submarine not in use
			PlayerSubmarine[i].arrangement(0, '0', '0', -1);

		cout << endl;
		system("cls");
	}

	// Arrange Destroyer
	for (int i = 1; i <= numDestroyer; i++) {
		game_printSea_player(sea_size);
		game_arrangement_instruction(sea_size);
		cout << "\n(" << i << " out of " << numDestroyer << ") Please input the coordinate of the destroyer (size of " << sizeDestroyer << "): ";
		cin >> direction >> y >> x;

		if ((direction == 'Q') && (y == 'u') && (x = 'i'))
		{
			game_quit();
			i--;
			continue;
		}

		if (direction == 'X') {  // X means ship place towards x-axis/horizontally
			while ((int)y < 65 || (int)y > 64 + sea_size || x < 0 || x > sea_size - 1 || x >= sea_size - 1 || player_sea[(int)y - 65][x] == 'O' || player_sea[(int)y - 65][x + 1] == 'O') {  // Prevent wrong input
				cout << "Wrong input! Try again." << endl;
				
				cin.clear();
				cin.ignore(10000, '\n');

				system("pause");
				system("cls");

				game_printSea_player(sea_size);
				game_arrangement_instruction(sea_size);
				cout << "\n(" << i << " out of " << numDestroyer << ") Please input the coordinate of the destroyer (size of " << sizeDestroyer << "): ";
				cin >> direction >> y >> x;
			}
			for (int i = 0; i < sizeDestroyer; i++) {
				player_sea[(int)y - 65][x + i] = 'O';
			}
		}
		else if (direction == 'Y') { // Y means ship place towards y-axis/vertically
			while ((int)y < 65 || (int)y > 64 + sea_size || x < 0 || x > sea_size - 1 || y >= (char)(sea_size + 64) || player_sea[(int)y - 65][x] == 'O' || player_sea[(int)y - 64][x] == 'O') { // Prevent wrong input
				cout << "Wrong input! Try again." << endl;
				
				cin.clear();
				cin.ignore(10000, '\n');

				system("pause");
				system("cls");

				game_printSea_player(sea_size);
				game_arrangement_instruction(sea_size);
				cout << "\n(" << i << " out of " << numDestroyer << ") Please input the coordinate of the destroyer (size of " << sizeDestroyer << "): ";
				cin >> direction >> y >> x;
			}
			for (int i = 0; i < sizeDestroyer; i++) {
				player_sea[(int)y - 65 + i][x] = 'O';
			}
		}
		if(direction == 'X' || direction == 'Y') 
		{
			PlayerDestroyer[i-1].arrangement(1, direction, y, x);
		}
		else // Neither X nor Y
		{
			cout << "Wrong input! Try again." << endl;
			
			cin.clear();
			cin.ignore(10000, '\n');

			system("pause");
			system("cls");
			i--;
			continue;
		}

		for(int i = numDestroyer; i <= 4; i++) // For destroyer not in use
			PlayerDestroyer[i].arrangement(0, '0', '0', -1);

		cout << endl;
		system("cls");
	}
	system("cls");
}

void game_arrangement_PC(char PC_sea[][10], int sea_size, int numCarrier, int numBattleship, int numSubmarine, int numDestroyer, Carrier PCCarrier[5], Battleship PCBattleship[5], Submarine PCSubmarine[5], Destroyer PCDestroyer[5])
{
	/*
	Carrier PCCarrier[5];
	Battleship PCBattleship[5];
	Submarine PCSubmarine[5];
	Destroyer PCDestroyer[5];
	*/

	int numDirection = 0, numY = 0; // for random
	char direction = '0', y = '0'; // direction means the ship is put vertically or horizontally, x is the coordinate of x-axis, y is the coordinate of y-axis
	int x = 0;

	srand((unsigned int)time(0));

	//Arrange Carrier
	for (int i = 1; i <= numCarrier; i++) {

		numDirection = rand() % 2;
		switch(numDirection)
		{case 0: direction = 'X'; break;		case 1: direction = 'Y'; break;}

		numY = rand() % sea_size;
		switch(numY)
		{	case 0: y = 'A'; break;			case 1: y = 'B'; break;			case 2: y = 'C'; break;			case 3: y = 'D'; break;			case 4: y = 'E'; break;
			case 5: y = 'F'; break;			case 6: y = 'G'; break;			case 7: y = 'H'; break;			case 8: y = 'I'; break;			case 9: y = 'J'; break; }

		x = rand() % sea_size;

		if (direction == 'X') {  // X means ship place towards x-axis/horizontally
			if (x >= sea_size - 4 || PC_sea[(int)y - 65][x] == 'O' || PC_sea[(int)y - 65][x + 1] == 'O' || PC_sea[(int)y - 65][x + 2] == 'O' || PC_sea[(int)y - 65][x + 3] == 'O' || PC_sea[(int)y - 65][x + 4] == 'O') {  // Prevent wrong input
				i--;
				continue;
			}
			for (int i = 0; i < sizeCarrier; i++) {
				PC_sea[(int)y - 65][x + i] = 'O';
			}
		}
		else if (direction == 'Y') { // Y means ship place towards y-axis/vertically
			if (y >= (char)(sea_size + 61) || PC_sea[(int)y - 65][x] == 'O' || PC_sea[(int)y - 64][x] == 'O' || PC_sea[(int)y - 63][x] == 'O' || PC_sea[(int)y - 62][x] == 'O' || PC_sea[(int)y - 61][x] == 'O') { // Prevent wrong input
				i--;
				continue;
			}
			for (int i = 0; i < sizeCarrier; i++) {
				PC_sea[(int)y - 65 + i][x] = 'O';
			}
		}
		PCCarrier[i-1].arrangement(1, direction, y, x);
	}
	for(int i = numCarrier; i <= 4; i++) // For carrier not in use
			PCCarrier[i].arrangement(0, '0', '0', -1);

	//Arrange battleship
	for (int i = 1; i <= numBattleship; i++) {
		numDirection = rand() % 2;
		switch(numDirection)
		{case 0: direction = 'X'; break;		case 1: direction = 'Y'; break;}

		numY = rand() % sea_size;
		switch(numY)
		{	case 0: y = 'A'; break;			case 1: y = 'B'; break;			case 2: y = 'C'; break;			case 3: y = 'D'; break;			case 4: y = 'E'; break;
			case 5: y = 'F'; break;			case 6: y = 'G'; break;			case 7: y = 'H'; break;			case 8: y = 'I'; break;			case 9: y = 'J'; break; }

		x = rand() % sea_size;

		if (direction == 'X') {  // X means ship place towards x-axis/horizontally
			if (x >= sea_size - 3 || PC_sea[(int)y - 65][x] == 'O' || PC_sea[(int)y - 65][x + 1] == 'O' || PC_sea[(int)y - 65][x + 2] == 'O' || PC_sea[(int)y - 65][x + 3] == 'O') {  // Prevent wrong input
				i--;
				continue;
			}
			for (int i = 0; i < sizeBattleship; i++) {
				PC_sea[(int)y - 65][x + i] = 'O';
			}
		}
		else if (direction == 'Y') { // Y means ship place towards y-axis/vertically
			if (y >= (char)(sea_size + 62) || PC_sea[(int)y - 65][x] == 'O' || PC_sea[(int)y - 64][x] == 'O' || PC_sea[(int)y - 63][x] == 'O' || PC_sea[(int)y - 62][x] == 'O') { // Prevent wrong input
				i--;
				continue;
			}
			for (int i = 0; i < sizeBattleship; i++) {
				PC_sea[(int)y - 65 + i][x] = 'O';
			}
		}
		PCBattleship[i-1].arrangement(1, direction, y, x);
	}
	for(int i = numBattleship; i <= 4; i++) // For battleship not in use
			PCBattleship[i].arrangement(0, '0', '0', -1);

	//Arrange submarine
	for (int i = 1; i <= numSubmarine; i++) {
		numDirection = rand() % 2;
		switch(numDirection)
		{case 0: direction = 'X'; break;		case 1: direction = 'Y'; break;}

		numY = rand() % sea_size;
		switch(numY)
		{	case 0: y = 'A'; break;			case 1: y = 'B'; break;			case 2: y = 'C'; break;			case 3: y = 'D'; break;			case 4: y = 'E'; break;
			case 5: y = 'F'; break;			case 6: y = 'G'; break;			case 7: y = 'H'; break;			case 8: y = 'I'; break;			case 9: y = 'J'; break; }

		x = rand() % sea_size;

		if (direction == 'X') {  // X means ship place towards x-axis/horizontally
			if (x >= sea_size - 2 || PC_sea[(int)y - 65][x] == 'O' || PC_sea[(int)y - 65][x + 1] == 'O' || PC_sea[(int)y - 65][x + 2] == 'O') {  // Prevent wrong input
				i--;
				continue;
			}
			for (int i = 0; i < sizeSubmarine; i++) {
				PC_sea[(int)y - 65][x + i] = 'O';
			}
		}
		else if (direction == 'Y') { // Y means ship place towards y-axis/vertically
			if (y >= (char)(sea_size + 63) || PC_sea[(int)y - 65][x] == 'O' || PC_sea[(int)y - 64][x] == 'O' || PC_sea[(int)y - 63][x] == 'O') { // Prevent wrong input
				i--;
				continue;
			}
			for (int i = 0; i < sizeSubmarine; i++) {
				PC_sea[(int)y - 65 + i][x] = 'O';
			}
		}
		PCSubmarine[i-1].arrangement(1, direction, y, x);
	}
	for(int i = numSubmarine; i <= 4; i++) // For submarine not in use
			PCSubmarine[i].arrangement(0, '0', '0', -1);

	//Arrange destroyer
	for (int i = 1; i <= numDestroyer; i++) {
		numDirection = rand() % 2;
		switch(numDirection)
		{case 0: direction = 'X'; break;		case 1: direction = 'Y'; break;}

		numY = rand() % sea_size;
		switch(numY)
		{	case 0: y = 'A'; break;			case 1: y = 'B'; break;			case 2: y = 'C'; break;			case 3: y = 'D'; break;			case 4: y = 'E'; break;
			case 5: y = 'F'; break;			case 6: y = 'G'; break;			case 7: y = 'H'; break;			case 8: y = 'I'; break;			case 9: y = 'J'; break; }

		x = rand() % sea_size;
		if (direction == 'X') {  // X means ship place towards x-axis/horizontally
			if (x >= sea_size - 1 || PC_sea[(int)y - 65][x] == 'O' || PC_sea[(int)y - 65][x + 1] == 'O') {  // Prevent wrong input
				i--;
				continue;
			}
			for (int i = 0; i < sizeDestroyer; i++) {
				PC_sea[(int)y - 65][x + i] = 'O';
			}
		}
		else if (direction == 'Y') { // Y means ship place towards y-axis/vertically
			if (y >= (char)(sea_size + 64) || PC_sea[(int)y - 65][x] == 'O' || PC_sea[(int)y - 64][x] == 'O') { // Prevent wrong input
				i--;
				continue;
			}
			for (int i = 0; i < sizeDestroyer; i++) {
				PC_sea[(int)y - 65 + i][x] = 'O';
			}
		}
		PCDestroyer[i-1].arrangement(1, direction, y, x);
	}
	for(int i = numDestroyer; i <= 4; i++) // For destroyer not in use
			PCDestroyer[i].arrangement(0, '0', '0', -1);
}

void game_arrangement_instruction(int sea_size)
{
	cout << "\nDirection: X(horizontal) / Y(vertical)" << endl;
	cout << "Row: A ~ " << (char)(sea_size + 64) << endl;
	cout << "Column: 0 ~ " << sea_size - 1 << endl;
	cout << "Format: (Direction)(Row)(Column)" << endl;
            cout << "Example: XA0, means the head of the ship starts at A0 point from left to right" << endl;
	cout << "Example: YA0, means the head of the ship starts at A0 point from top to bottom" << endl;
	cout << "You can also use the special command \"Quit\" (without quotes) to quit the game." << endl;
}

void game_printSea_player(int sea_size)
{
	int count = 0;
	int z = 65;

	cout << "\n";
	for (int i = 0; i < sea_size; i++) {
		if (i == 0)
			cout << setw(5) << i;
		else
			cout << setw(2) << i;
	}
	cout << endl;

	do {
		for (int i = 0; i < sea_size * 2 + 3; i++) {
			if (i == 0)
				cout << setw(3) << "+";
			else if (i == sea_size * 2 + 2)
				cout << "+";
			else
				cout << "-";
		}
		cout << endl;

		if (count == 1) break;

		for (int row = 0; row < sea_size; row++) {
			cout << (char)z << setw(2) << "|";
			z++;

			for (int col = 0; col < sea_size; col++) {
				cout << " " << player_sea[row][col];
			}

			cout << " |" << endl;
		}
		count++;
	} while (count < 2);
}

void game_printSea_PC(int sea_size, bool DisplayPCShips)
{
	int count = 0;
	int z = 65;

	cout << "\n";
	for (int i = 0; i < sea_size; i++) {
		if (i == 0)
			cout << setw(5) << i;
		else
			cout << setw(2) << i;
	}
	cout << endl;

	do {
		for (int i = 0; i < sea_size * 2 + 3; i++) {
			if (i == 0)
				cout << setw(3) << "+";
			else if (i == sea_size * 2 + 2)
				cout << "+";
			else
				cout << "-";
		}
		cout << endl;

		if (count == 1) break;

		for (int row = 0; row < sea_size; row++) {
			cout << (char)z << setw(2) << "|";
			z++;

			for (int col = 0; col < sea_size; col++) {
				if(DisplayPCShips == 0)
				{
					if(PC_sea[row][col] == 'O')
					{
						cout << "  ";
					}
					else
					{
						cout << " " << PC_sea[row][col];
					}
				}
				else
				{
					cout << " " << PC_sea[row][col];
				}	
			}
			cout << " |" << endl;
		}
		count++;
	} while (count < 2);
}

void game_battlestage(char player_sea[][10], char PC_sea[][10], bool WhoStartFirst, bool DisplayPCShips, int sea_size, int numCarrier, int numBattleship, int numSubmarine, int numDestroyer, Carrier PlayerCarrier[5], Battleship PlayerBattleship[5], Submarine PlayerSubmarine[5], Destroyer PlayerDestroyer[5], Carrier PCCarrier[5], Battleship PCBattleship[5], Submarine PCSubmarine[5], Destroyer PCDestroyer[5])
{
	bool FirstTurn = 0;
	char attack_Y = '0';
	int attack_X = 0;
	int sumAttackStatus = 0;

	int attack_status = 0;
	bool flag_destroyed = 0;
	bool PC_sea_attacked [10][10] = {0};
	bool player_sea_attacked [10][10] = {0};

	//Global variables
	int numCarrier_player = numCarrier;
	int numBattleship_player = numBattleship;
	int numSubmarine_player = numSubmarine;
	int numDestroyer_player = numDestroyer;

	int numCarrier_PC = numCarrier;
	int numBattleship_PC = numBattleship;
	int numSubmarine_PC = numSubmarine;
	int numDestroyer_PC = numDestroyer;

	//PC Intelligence variables
	int numUnknownPro = 0, numLowPro = 0, numMedPro = 0, numHighPro = 0, numVHighPro = 0; // number of all probability 
	int seaPCI[10][10] = {0}; // 2-dimensional [row][column] array for storing the probability

	system("cls");

	while(1)
	{
		//Player turn starts here
		if( WhoStartFirst == 1 && FirstTurn == 0) // PC starts first
		{
			FirstTurn = 1;
			goto PC_FIRST;
		}
		else
		{

QUIT_BUT_N:
			game_printSea_battlestage(sea_size, DisplayPCShips);
			cout << "Format: (Row)(Column)" << endl;
			cout << "Row: A ~ " << (char)(sea_size + 64) << "    Column: 0 ~ " << sea_size - 1 << endl;
            cout << "Example: C5, means you want to attack Row C, Column 5" << endl;
			cout << "You can also use the special command \"Quit\" (without quotes) to quit the game." << "\n\n";
			cout << "Please input the coordinate which you want to attack: ";
			cin >> attack_Y >> attack_X;

			if (attack_Y == 'Q')
			{
				game_quit();
				cin.clear();
				cin.ignore(10000, '\n');
				goto QUIT_BUT_N;
			}

			while ((attack_X >= sea_size || (int)attack_Y >= sea_size + 65 || attack_X < 0 || (int)attack_Y < 65)) 
			{  // Prevent wrong input (out of sea)
				if (attack_Y == 'Q')
				{
					game_quit();
					cin.clear();
					cin.ignore(10000, '\n');
					goto QUIT_BUT_N;
				}

				cout << "Input attack grid is out of sea! Try input again." << endl;

				cin.clear();
				cin.ignore(10000, '\n');

				system("pause");
				system("cls");

				game_printSea_battlestage(sea_size, DisplayPCShips);
				cout << "Format: (Row)(Column)" << endl;
				cout << "Row: A ~ " << (char)(sea_size + 64) << "    " << "Column: 0 ~ " << sea_size - 1 << endl;
				cout << "Example: C5, means you want to attack Row C, Column 5" << "\n\n";
				cout << "Please input the coordinate which you want to attack: ";
				cin >> attack_Y >> attack_X;
			}

			cout << "The player decides to attack " << attack_Y << attack_X << ". ";

			//Check fire status starts here
			for(int i = 0; i < 5; i++)
			{
				attack_status += PCCarrier[i].attack_check(0, attack_Y, attack_X, player_sea, PC_sea, sea_size, PC_sea_attacked, player_sea_attacked, seaPCI); // 0 --> Player did that attack
			}
			if(attack_status == 3)
			{
				cout << "A carrier is destroyed!" << endl;
				flag_destroyed = 1;
				attack_status = 0;
				numCarrier_PC--;
			}

			for(int i = 0; i < 5; i++)
			{
				attack_status += PCBattleship[i].attack_check(0, attack_Y, attack_X, player_sea, PC_sea, sea_size, PC_sea_attacked, player_sea_attacked, seaPCI); // 0 --> Player did that attack
			}
			if(attack_status == 3)
			{
				cout << "A battleship is destroyed!" << endl;
				flag_destroyed = 1;
				attack_status = 0;
				numBattleship_PC--;
			}

			for(int i = 0; i < 5; i++)
			{
				attack_status += PCSubmarine[i].attack_check(0, attack_Y, attack_X, player_sea, PC_sea, sea_size, PC_sea_attacked, player_sea_attacked, seaPCI); // 0 --> Player did that attack
			}
			if(attack_status == 3)
			{
				cout << "A submarine is destroyed!" << endl;
				flag_destroyed = 1;
				attack_status = 0;
				numSubmarine_PC--;
			}

			for(int i = 0; i < 5; i++)
			{
				attack_status += PCDestroyer[i].attack_check(0, attack_Y, attack_X, player_sea, PC_sea, sea_size, PC_sea_attacked, player_sea_attacked, seaPCI); // 0 --> Player did that attack
			}
			if(attack_status == 3)
			{
				cout << "A destroyer is destroyed!" << endl;
				flag_destroyed = 1;
				attack_status = 0;
				numDestroyer_PC--;
			}

			if(flag_destroyed != 1)
			{
				switch(attack_status)
				{	case 0: cout << "It is a miss!" << endl; break;					case 1: cout << "The grid has been hit before. Nothing happens." << endl; break;
					case 2: cout << "Part of a ship is destoryed!"<< endl; break;
				}
			}

			cout << endl;

			flag_destroyed = 0;
			attack_status = 0;
			//Check fire status ends here

			//Check win starts here
			if(numDestroyer_PC == 0 && numSubmarine_PC == 0 && numBattleship_PC == 0 && numCarrier_PC == 0)
			{
				system("pause");
				system("cls");
				game_printSea_battlestage(sea_size, DisplayPCShips);
				cout << "\n\n";
				cout << "Congradulation! You have destoryed all the enemy's ships and won this round!!" << endl;
				cout << "Press any key to return to the main menu." << "\n\n";
				goto GAME_END;
			}
			//Check win ends here

			system("pause");
			system("cls");
		}
		//Player turn ends here

		//PC turn starts here
PC_FIRST:
		game_printSea_battlestage(sea_size, DisplayPCShips);
		PCI_decide_attack(sea_size, seaPCI, numUnknownPro, numLowPro, numMedPro, numHighPro, numVHighPro); //calculate the probability
		PCI_attack_phrase(sea_size, seaPCI, numUnknownPro, numLowPro, numMedPro, numHighPro, numVHighPro, attack_Y, attack_X); //decide which to hit and also adjusting probability accordingly

		//Check fire status starts here
			for(int i = 0; i < 5; i++)
			{
				attack_status += PlayerCarrier[i].attack_check(1, attack_Y, attack_X, player_sea, PC_sea, sea_size, PC_sea_attacked, player_sea_attacked, seaPCI); // 1 --> PC did that attack
			}
			if(attack_status == 3)
			{
				cout << "A carrier is destroyed!" << endl;
				flag_destroyed = 1;
				attack_status = 0;
				numCarrier_player--;
			}

			for(int i = 0; i < 5; i++)
			{
				attack_status += PlayerBattleship[i].attack_check(1, attack_Y, attack_X, player_sea, PC_sea, sea_size, PC_sea_attacked, player_sea_attacked, seaPCI); // 1 --> PC did that attack
			}
			if(attack_status == 3)
			{
				cout << "A battleship is destroyed!" << endl;
				flag_destroyed = 1;
				attack_status = 0;
				numBattleship_player--;
			}

			for(int i = 0; i < 5; i++)
			{
				attack_status += PlayerSubmarine[i].attack_check(1, attack_Y, attack_X, player_sea, PC_sea, sea_size, PC_sea_attacked, player_sea_attacked, seaPCI); // 1 --> PC did that attack
			}
			if(attack_status == 3)
			{
				cout << "A submarine is destroyed!" << endl;
				flag_destroyed = 1;
				attack_status = 0;
				numSubmarine_player--;
			}

			for(int i = 0; i < 5; i++)
			{
				attack_status += PlayerDestroyer[i].attack_check(1, attack_Y, attack_X, player_sea, PC_sea, sea_size, PC_sea_attacked, player_sea_attacked, seaPCI); // 1 --> PC did that attack
			}
			if(attack_status == 3)
			{
				cout << "A destroyer is destroyed!" << endl;
				flag_destroyed = 1;
				attack_status = 0;
				numDestroyer_player--;
			}

			if(flag_destroyed != 1)
			{
				switch(attack_status)
				{	case 0: cout << "It is a miss!" << endl; break;					
					case 1: cout << "The grid has been hit before. Nothing happens." << endl; break;
					case 2: cout << "Part of a ship is destoryed!"<< endl;
							//change of probability inside seaPCI
							//ignore grid that is out of boundary
					case 3:	int i = (int)attack_Y - 65;
							int j = attack_X;
							if(i < sea_size - 1)
								seaPCI[i+1][j] += 25;
							if(i != 0)
								seaPCI[i-1][j] += 25;
							if(j < sea_size - 1)
								seaPCI[i][j+1] += 25;
							if(j != 0)
								seaPCI[i][j-1] += 25;
							break;
				}
			}

			cout << endl;

			flag_destroyed = 0;
			attack_status = 0;
			//Check fire status ends here

			//Check win starts here
			if(numDestroyer_player == 0 && numSubmarine_player == 0 && numBattleship_player == 0 && numCarrier_player == 0)
			{
				system("pause");
				system("cls");
				game_printSea_battlestage(sea_size, DisplayPCShips);
				cout << "\n\n";
				cout << "The PC has destoryed all of your ships and won this round!!" << endl;
				cout << "Press any key to return to the main menu." << "\n\n";
				goto GAME_END;
			}
			//Check win ends here
		system("pause");
		system("cls");
	}
GAME_END:
	//Reset sea
	for(int i = 0; i < sea_size; i++)
	{
		for(int j = 0; j < sea_size; j++)
		{
			player_sea[i][j] = ' ';
			PC_sea[i][j] = ' ';
		}
	}
	system("pause");
	system("cls");
}

void game_quit()
{
	char ans;

	cin.clear();
	cin.ignore(10000, '\n');

	cout << "\n";
	cout << "Are you sure to leave?" << endl;
	cout << "If yes, please input 'y'/'Y'." << endl;
	cout << "If no, please input 'n'/'N'." << endl;
	cin >> ans;

	if (ans == 'y' || ans == 'Y')
	{
		cout << "Good Bye!" << endl;
		exit(1);
	}
	else if (ans == 'n' || ans == 'N')
	{
		system("cls");
		cin.clear();
		cin.ignore(10000, '\n');
	}
	else
	{
		cout << "Wrong input! Please try again.";
		game_quit();
	}
}

void PCI_decide_attack(int size, int seaPCI[][10], int &numUnknownPro, int &numLowPro, int &numMedPro, int &numHighPro, int &numVHighPro)
{
	int numPro = 0; // for sotring number of that probability

	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			if( i >= size || j >= size )
				seaPCI[i][j] = -1;
		}
	}

	for(int i = 0; i <= 100; i += 25) // 0~100 (25 increament)
	{
		numPro = PCI_calculate_Pro(size, i, seaPCI);
		switch(i)
		{
			case 0: numUnknownPro = numPro; break;		case 25: numLowPro = numPro; break;		case 50:numMedPro = numPro; break;
			case 75:numHighPro = numPro; break;			case 100:numVHighPro = numPro; break;
		}
	}
}

int PCI_calculate_Pro(int size, int pro, int seaPCI[][10])
{
	int numPro = 0;
	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			if( seaPCI[i][j] == pro ) // when the probability stored in the array equals to the target probability
			{
				numPro++;
			}
		}
	}
	return numPro; // return the number of grid that matches that probability
}

void PCI_attack_phrase(int size, int seaPCI[][10], int numUnknownPro, int numLowPro, int numMedPro, int numHighPro, int numVHighPro, char &attack_Y, int &attack_X)
{
	// cout << "numVHighPro: " << numVHighPro << " numHighPro: " << numHighPro << " numMedPro: " << numMedPro << " numLowPro: " << numLowPro << " numUnknownPro: " << numUnknownPro << "." << endl;
	if(numVHighPro > 0)
		PCI_fire(size, 100, numVHighPro, seaPCI, attack_Y, attack_X); // PC will attack grid that has the highest probability first
	else if(numHighPro > 0)
		PCI_fire(size, 75, numHighPro, seaPCI, attack_Y, attack_X);
	else if(numMedPro > 0)
		PCI_fire(size, 50, numMedPro, seaPCI, attack_Y, attack_X);
	else if(numLowPro > 0)
		PCI_fire(size, 25, numLowPro, seaPCI, attack_Y, attack_X);
	else
		PCI_fire(size, 0, numUnknownPro, seaPCI, attack_Y, attack_X);
}

void PCI_fire(int size, int pro, int numPro, int seaPCI[][10], char &attack_Y, int &attack_X)
{
	int attack_position = 0;
	int orderPro = 0;

	srand((unsigned int)time(0)); // change seed of random
	attack_position = rand() % numPro + 1; // determine the position of attack randomly, pick one number from 1 ~ numPro

	for(int i = 0; i < size; i++) 
	{
		for(int j = 0; j < size; j++)
		{
			if(seaPCI[i][j] == pro)
			{
				orderPro++; 
				if(attack_position == orderPro) //Search the attack_position(th) grid
				{
					int attack_Y_to_int = 0;
					attack_Y = (char)(i + 65);
					attack_X = j;

					cout << "The PC decides to attack " << attack_Y << attack_X << ". ";
					switch(attack_Y)
					{
						case 'A': attack_Y_to_int = 0; break;  case 'B': attack_Y_to_int = 1; break;  case 'C': attack_Y_to_int = 2; break;	case 'D': attack_Y_to_int = 3; break;  
						case 'E': attack_Y_to_int = 4; break;  case 'F': attack_Y_to_int = 5; break;  case 'G': attack_Y_to_int = 6; break;  case 'H': attack_Y_to_int = 7; break;	
						case 'I': attack_Y_to_int = 8; break;  case 'J': attack_Y_to_int = 9; break;
					}

					seaPCI[attack_Y_to_int][attack_X] = -1; //reset that attack grid so as to indicate that it has been attacked by PC

					/* Below are for debugging only
					for(int i = 0; i < size; i++) 
					{
						for(int j = 0; j < size; j++)
						{
							cout << setw(5) << seaPCI[i][j];
						}
						cout << endl;
					}
					*/

					goto FOUND;
				}
			}
		}
	}
FOUND:
	;
}

void game_printSea_battlestage(int sea_size, bool DisplayPCShips)
{
	game_printSea_PC(sea_size, DisplayPCShips);
	cout << setw(sea_size) << "" << "PC's sea" << "\n\n";

	game_printSea_player(sea_size);
	cout << setw(sea_size - 2) << "" << "Player's sea" << "\n\n";
}

void setting(bool &WhoStartFirst, bool &DisplayPCShips, int &sea_size, int &numCarrier, int &numBattleship, int &numSubmarine, int &numDestroyer)
{
	system("cls");
	int option;

	cout << "\n";

	cout << "*** Settings Menu *****" << endl;
	cout << "[1] Who starts first" << endl;
	cout << "[2] Display PC's ships" << endl;
	cout << "[3] Sea size" << endl;
	cout << "[4] Number of Ships" << endl;
	cout << "[5] Return to game menu" << endl;
	cout << "***********************" << endl;
	cout << "Option (1 - 5): ";

	cin >> option;
	switch (option)
	{
	case 1: whoFirst(WhoStartFirst); break;
	case 2: displayPCships(DisplayPCShips); break;
	case 3: seaSize(sea_size, numCarrier, numBattleship, numSubmarine, numDestroyer); break;
	case 4: numberOFships(sea_size, numCarrier, numBattleship, numSubmarine, numDestroyer); break;
	case 5: system("cls"); break;
	default:
		cout << "Please enter choice 1 - 5 only." << endl;
		cin.clear();
		cin.ignore(10000, '\n');
		system("pause");
		system("cls");
		setting(WhoStartFirst, DisplayPCShips, sea_size, numCarrier, numBattleship, numSubmarine, numDestroyer);
		break;
	}
}

void whoFirst(bool &WhoStartFirst)
{
	system("cls");
	int option;

	cout << endl;
	cout << "How start first?" << endl;
	cout << "Current setting (default: Player): ";
	if (WhoStartFirst == 0)
		cout << "Player" << "\n\n";
	else
		cout << "PC" << "\n\n";
	cout << "Change of setting (0 - Player starts first [default], 1 - PC starts first): ";
	cin >> option;

	if (option == 0)
	{
		WhoStartFirst = 0;
		cout << "\n\n\n";
		cout << "Player starts first now." << "\n\n";
		cout << "Press any key to return to main menu." << "\n";
		system("pause");
		system("cls");
	}
	else if (option == 1)
	{
		WhoStartFirst = 1;
		cout << "\n\n\n";
		cout << "PC starts first now." << "\n\n";
		cout << "Press any key to return to main menu." << "\n";
		system("pause");
		system("cls");
	}
	else
	{
		cout << "Error input. Please try again." << endl;
		cin.clear();
		cin.ignore(10000, '\n');
		system("pause");
		whoFirst(WhoStartFirst);
	}
}

void displayPCships(bool &DisplayPCShips)
{
	system("cls");
	int option;

	cout << endl;
	cout << "Display PC ships?" << endl;
	cout << "Current setting (default: No): ";
	if (DisplayPCShips == 0)
		cout << "No" << "\n\n";
	else if (DisplayPCShips == 1)
		cout << "Yes" << "\n\n";
	cout << "Change of setting (Enter 0 - Do NOT display PC ships [default], Enter 1 - Display PC ships): ";
	cin >> option;

	if (option == 0)
	{
		DisplayPCShips = 0;
		cout << "\n\n\n";
		cout << "System will NOT display PC ships now." << "\n\n";
		cout << "Press any key to return to main menu." << "\n";
		system("pause");
		system("cls");
	}
	else if (option == 1)
	{
		DisplayPCShips = 1;
		cout << "\n\n\n";
		cout << "System will display PC ships now." << "\n\n";
		cout << "Press any key to return to main menu." << "\n";
		system("pause");
		system("cls");
	}
	else
	{
		cout << "Error input. Please try again." << endl;
		cin.clear();
		cin.ignore(10000, '\n');
		system("pause");
		displayPCships(DisplayPCShips);
	}
}

void seaSize(int &sea_size, int numCarrier, int numBattleship, int numSubmarine, int numDestroyer)
{
	system("cls");
	int option;

	cout << endl;
	cout << "The size of the sea" << endl;
	cout << "Current setting (default 10x10): ";
	cout << sea_size << "x" << sea_size << "\n\n";

	cout << "Please be aware that the capacity of the sea must be larger than the total size occupied by all the ships." << endl;
	cout << "Change of setting (Enter 5~10): ";
	cin >> option;

	if ((option >= 5 && option <= 10) && (numCarrier * 5 + numBattleship * 4 + numSubmarine * 3 + numDestroyer * 2 <= option * option))
	{
		sea_size = option;
		cout << "\n\n\n";
		cout << "The size of the sea has been changed to " << sea_size << "x" << sea_size << "." << "\n\n";
		cout << "Press any key to return to main menu." << "\n";
		system("pause");
		system("cls");
	}
	else
	{
		cout << "Error input. ";
		cin.clear();
		cin.ignore(10000, '\n');
		if ((option >= 5 && option <= 10) == 0)
		{
			cout << "The size of the sea must be within 5 to 10." << endl;
		}
		else if((option >= 5 && option <= 10) && (numCarrier * 5 + numBattleship * 4 + numSubmarine * 3 + numDestroyer * 2 >= option * option))
		{
			cout << "The capacity of the sea is smaller than the total size occupied by all the ships." << endl;
		}
		cout << "Please try again." << endl;
		system("pause");
		seaSize(sea_size, numCarrier, numBattleship, numSubmarine, numDestroyer);
	}
}

void numberOFships(int sea_size, int &numCarrier, int &numBattleship, int &numSubmarine, int &numDestroyer)
{
	int tempNumCarrier, tempNumBattleship, tempNumSubmarine, tempNumDestroyer;
	system("cls");

	cout << left << endl;
	cout << setw(21) << "Type of ship" << "Number" << endl;
	cout << setw(11) << "Carrier" << setw(12) << "(size 5):" << numCarrier << endl;
	cout << setw(11) << "Battleship" << setw(12) << "(size 4):" << numBattleship << endl;
	cout << setw(11) << "Submarine" << setw(12) << "(size 3):" << numSubmarine << endl;
	cout << setw(11) << "Destroyer" << setw(12) << "(size 2):" << numDestroyer << endl;

	cout << "\n\n";
	cout << "Please be aware that the total size occupied by all the ships must be smaller than or equal to the capacity of the sea" << endl;
	cout << "Please enter the new number of carrier (size 5)(Enter 1~5, default:1): ";
	cin >> tempNumCarrier;

	cout << "Please enter the new number of battleship (size 4)(Enter 1~5, default:2): ";
	cin >> tempNumBattleship;

	cout << "Please enter the new number of submarine (size 3)(Enter 1~5, default:3): ";
	cin >> tempNumSubmarine;

	cout << "Please enter the new number of destroyer (size 2)(Enter 1~5, default:4): ";
	cin >> tempNumDestroyer;

	if ((tempNumCarrier >= 1 && tempNumCarrier <= 5) && (tempNumBattleship >= 1 && tempNumBattleship <= 5) &&
		(tempNumSubmarine >= 1 && tempNumSubmarine <= 5) && (tempNumDestroyer >= 1 && tempNumDestroyer <= 5) &&
		(tempNumCarrier * 5 + tempNumBattleship * 4 + tempNumSubmarine * 3 + tempNumDestroyer * 2 <= sea_size * sea_size) == 1)
	{
		numCarrier = tempNumCarrier;
		numBattleship = tempNumBattleship;
		numSubmarine = tempNumSubmarine;
		numDestroyer = tempNumDestroyer;

		cout << "\n\n";
		cout << "All changes have been saved. The new setting are as follow:" << "\n\n";
		cout << setw(21) << "Type of ship" << "Number" << endl;
		cout << setw(11) << "Carrier" << setw(12) << "(size 5):" << numCarrier << endl;
		cout << setw(11) << "Battleship" << setw(12) << "(size 4):" << numBattleship << endl;
		cout << setw(11) << "Submarine" << setw(12) << "(size 3):" << numSubmarine << endl;
		cout << setw(11) << "Destroyer" << setw(12) << "(size 2):" << numDestroyer << endl;
		cout << "\n\n\n";
		cout << "Press any key to return to main menu." << "\n";
		system("pause");
		system("cls");
	}
	else
	{
		cout << "\n\nError input. ";
		cin.clear();
		cin.ignore(10000, '\n');
		if ((tempNumCarrier >= 1 && tempNumCarrier <= 5) && (tempNumBattleship >= 1 && tempNumBattleship <= 5) &&
			(tempNumSubmarine >= 1 && tempNumSubmarine <= 5) && (tempNumDestroyer >= 1 && tempNumDestroyer <= 5) == 0)
		{
			cout << "Number of Carrier, Battleship, Submarine and Destroyer must be within 1 to 5" << endl;
		}
		else if(tempNumCarrier * 5 + tempNumBattleship * 4 + tempNumSubmarine * 3 + tempNumDestroyer * 2 >= sea_size * sea_size)
		{
			cout << "The total size occupied by all the ships is larger than to the capacity of the sea" << endl;
		}
		cout << "Please try again." << endl;
		system("pause");
		numberOFships(sea_size, numCarrier, numBattleship, numSubmarine, numDestroyer);
	}
}

void instructions()
{
	system("cls");

	cout << "Battle Game Instructions" << "\n\n";

	cout << "How to win: " << endl;
	cout << "You and PC will take turns to attack the opponent's sea by inputting the desired slot (e.g. A0)" << "\n\n";
	cout << "There are two possible cases:" << endl;
	cout << "1) Empty slot is marked by '*'" << endl;
	cout << "2) Ship body is marked by 'H'" << endl;
	cout << "Take turns shooting until one player has lost all their ships and he/she will win" << "\n\n";
	cout << "(continued...)" << endl;

	system("pause");

	cout << "\nWhen you place your ship, the format should be:" << endl;
	cout << "(Direction 'X' / 'Y')(Row 'A' ~ 'J')(Column 0 ~ 9)" << endl;
	cout << "Direction X: the ship placed horizontally \nDirection Y: the ship placed vertically" << "\n\n";
	cout << "Example: XA0 (Enter)" << endl;
	cout << "The ship will place placed on Row A Column 0 horizontally" << endl;
	cout << "You can also use the special command \"Quit\" (without quotes) to quit the game at arrangement stage or battle stage." << "\n\n\n";

	cout << "The number and types of ships to be placed (by default):" << endl;
	cout << left << endl;
	cout << setw(21) << "Type of ship" << "Number" << endl;
	cout << setw(11) << "Carrier" << setw(12) << "(size 5):" << "1" << endl;
	cout << setw(11) << "Battleship" << setw(12) << "(size 4):" << "2" << endl;
	cout << setw(11) << "Submarine" << setw(12) << "(size 3):" << "3" << endl;
	cout << setw(11) << "Destroyer" << setw(12) << "(size 2):" << "4" << endl;
	cout << "\n\n\n";
	cout << "Press any key to return to main menu." << "\n";
	system("pause");
	system("cls");
}

void credits()
{
	system("cls");

	cout << "\n" << left << endl;
	cout << "Credit:" << "\n\n";
	cout << setw(20) << "Student Name" << setw(15) << "Student ID" << setw(5) << "Class" << endl;

	cout << setw(20) << "Chan Tik Shun" << setw(15) << "16109510A" << setw(5) << "201C" << endl;
	cout << setw(20) << "Chui Tsz Fung" << setw(15) << "16155274A" << setw(5) << "201B" << endl;
	cout << setw(20) << "Chan Cho Sin Nicole" << setw(15) << "16009941A" << setw(5) << "201A" << endl;
	cout << setw(20) << "Hau Shin Man" << setw(15) << "16048352A" << setw(5) << "201A" << endl;
	cout << setw(20) << "Cheung Hiu Ching" << setw(15) << "16005769A" << setw(5) << "201A" << endl;
	cout << setw(20) << "Chan Hoi Yu" << setw(15) << "16065268A" << setw(5) << "201C" << endl;
	cout << "\n\n\n" << right;
	cout << "Press any key to return to main menu." << "\n";
	system("pause");
	system("cls");
}




