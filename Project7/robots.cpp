// robots.cpp

// Portions you are to complete are marked with a TODO: comment.
// We've provided some incorrect return statements (so indicated) just
// to allow this skeleton program to compile and run, albeit incorrectly.
// The first thing you probably want to do is implement the utterly trivial
// functions (marked TRIVIAL).  Then get Arena::display going.  That gives
// you more flexibility in the order you tackle the rest of the functionality.
// As you finish implementing each TODO: item, remove its TODO: comment.

#include <iostream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <cctype>
#include <ctime>
#include <cassert>
using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;             // max number of rows in the arena
const int MAXCOLS = 20;             // max number of columns in the arena
const int MAXROBOTS = 100;          // max number of robots allowed
const int MAXCHANNELS = 3;          // max number of channels
const int MAXHEALTH = 3;
const double WALL_DENSITY = 0.12;   // density of walls

const int NORTH = 0;
const int EAST = 1;
const int SOUTH = 2;
const int WEST = 3;
const int NUMDIRS = 4;

const int EMPTY = 0;
const int WALL = 1;

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Arena;  // This is needed to let the compiler know that Arena is a
			  // type name, since it's mentioned in the Robot declaration.

class Robot
{
public:
	// Constructor
	Robot(Arena* ap, int r, int c, int channel);

	// Accessors
	int  row() const;
	int  col() const;
	int  channel() const;
	bool isDead() const;

	// Mutators
	void forceMove(int dir);
	void move();

private:
	Arena* m_arena;
	int    m_row;
	int    m_col;
	int    m_channel;
	int    m_health;
};

class Player
{
public:
	// Constructor
	Player(Arena *ap, int r, int c);

	// Accessors
	int  row() const;
	int  col() const;
	bool isDead() const;

	// Mutators
	string stand();
	string move(int dir);
	void   setDead();

private:
	Arena* m_arena;
	int    m_row;
	int    m_col;
	bool   m_dead;
};

class Arena
{
public:
	// Constructor/destructor
	Arena(int nRows, int nCols);
	~Arena();

	// Accessors
	int     rows() const;
	int     cols() const;
	Player* player() const;
	int     robotCount() const;
	int     getCellStatus(int r, int c) const;
	int     numberOfRobotsAt(int r, int c) const;
	void    display(string msg) const;

	// Mutators
	void   setCellStatus(int r, int c, int status);
	bool   addRobot(int r, int c, int channel);
	bool   addPlayer(int r, int c);
	string moveRobots(int channel, int dir);

private:
	int     m_grid[MAXROWS][MAXCOLS];
	int     m_rows;
	int     m_cols;
	Player* m_player;
	Robot*  m_robots[MAXROBOTS];
	int     m_nRobots;

	// Helper functions
	void checkPos(int r, int c) const;
};

class Game
{
public:
	// Constructor/destructor
	Game(int rows, int cols, int nRobots);
	~Game();

	// Mutators
	void play();

private:
	Arena* m_arena;

	// Helper functions
	string takePlayerTurn();
	string takeRobotsTurn();
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int randInt(int lowest, int highest);
bool charToDir(char ch, int& dir);
bool attemptMove(const Arena& a, int dir, int& r, int& c);
bool recommendMove(const Arena& a, int r, int c, int& bestDir);
void clearScreen();

///////////////////////////////////////////////////////////////////////////
//  Robot implementation
///////////////////////////////////////////////////////////////////////////

Robot::Robot(Arena* ap, int r, int c, int channel)
{
	if (ap == nullptr)
	{
		cout << "***** A robot must be created in some Arena!" << endl;
		exit(1);
	}
	if (r < 1 || r > ap->rows() || c < 1 || c > ap->cols())
	{
		cout << "***** Robot created with invalid coordinates (" << r << ","
			<< c << ")!" << endl;
		exit(1);
	}
	if (channel < 1 || channel > MAXCHANNELS)
	{
		cout << "***** Robot created with invalid channel " << channel << endl;
		exit(1);
	}
	m_arena = ap;
	m_row = r;
	m_col = c;
	m_channel = channel;
	m_health = MAXHEALTH;
}

int Robot::row() const
{
	return m_row;
}

int Robot::col() const
{
	return m_col;
}

int Robot::channel() const
{
	return m_channel;
}

bool Robot::isDead() const
{
	if (m_health == 0)
		return true;
	else
		return false;
}

void Robot::forceMove(int dir)
{
	if (!isDead())
	{
		bool b = attemptMove(*m_arena, dir, m_row, m_col); //If the move fails, health is lost
		if (!b)
			m_health--;
	}
}

void Robot::move()
{
	// Attempt to move in a random direction; if we can't move, don't move
	if (!isDead())
		attemptMove(*m_arena, randInt(0, NUMDIRS - 1), m_row, m_col);
}

///////////////////////////////////////////////////////////////////////////
//  Player implementation
///////////////////////////////////////////////////////////////////////////

Player::Player(Arena* ap, int r, int c)
{
	if (ap == nullptr)
	{
		cout << "***** The player must be created in some Arena!" << endl;
		exit(1);
	}
	if (r < 1 || r > ap->rows() || c < 1 || c > ap->cols())
	{
		cout << "**** Player created with invalid coordinates (" << r
			<< "," << c << ")!" << endl;
		exit(1);
	}
	m_arena = ap;
	m_row = r;
	m_col = c;
	m_dead = false;
}

int Player::row() const
{
	return m_row;
}

int Player::col() const
{
	return m_col;
}

string Player::stand()
{
	return "Player stands.";
}

string Player::move(int dir)
{
	switch (dir)
	{
		case NORTH:
			if (m_row > 1 && m_arena->getCellStatus(m_row - 1, m_col) != WALL) //Check if room to move is there and there's no wall in the way
			{
				m_row--;
				if (m_arena->numberOfRobotsAt(m_row, m_col) > 0) //If there is a least 1 robot in the way, player dies
				{
					m_dead = true;
					return "Player walked into a robot and died.";
				}
				else
					return "Player moved north.";
				break;
			}
			else
				break;
		case EAST:
			if (m_col < m_arena->cols() && m_arena->getCellStatus(m_row, m_col + 1) != WALL) //Same as above for this case and cases below
			{
				m_col++;
				if (m_arena->numberOfRobotsAt(m_row, m_col) > 0)
				{
					m_dead = true;
					return "Player walked into a robot and died.";
				}
				else
					return "Player moved east.";
				break;
			}
			else
				break;
		case SOUTH:
			if (m_row < m_arena->rows() && m_arena->getCellStatus(m_row + 1, m_col) != WALL)
			{
				m_row++;
				if (m_arena->numberOfRobotsAt(m_row, m_col) > 0)
				{
					m_dead = true;
					return "Player walked into a robot and died.";
				}
				else
					return "Player moved south.";
				break;
			}
			else
				break;
		case WEST:
			if (m_col > 1 && m_arena->getCellStatus(m_row, m_col - 1) != WALL)
			{
				m_col--;
				if (m_arena->numberOfRobotsAt(m_row, m_col) > 0)
				{
					m_dead = true;
					return "Player walked into a robot and died.";
				}
				else
					return "Player moved west.";
				break;
			}
			else
				break;
	}
	return "Player couldn't move; player stands."; //If move not possible, nothing changes
}

bool Player::isDead() const
{
	return m_dead;
}

void Player::setDead()
{
	m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Arena implementation
///////////////////////////////////////////////////////////////////////////

Arena::Arena(int nRows, int nCols)
{
	if (nRows <= 0 || nCols <= 0 || nRows > MAXROWS || nCols > MAXCOLS)
	{
		cout << "***** Arena created with invalid size " << nRows << " by "
			<< nCols << "!" << endl;
		exit(1);
	}
	m_rows = nRows;
	m_cols = nCols;
	m_player = nullptr;
	m_nRobots = 0;
	for (int r = 1; r <= m_rows; r++)
		for (int c = 1; c <= m_cols; c++)
			setCellStatus(r, c, EMPTY);
}

Arena::~Arena()
{
	for (int i = 0; i < m_nRobots; i++) //Go through the robot array and delete anything left
	{
		if (m_robots[i] != nullptr)
			delete m_robots[i];
	}
	delete m_player;
}

int Arena::rows() const
{
	return m_rows;
}

int Arena::cols() const
{
	return m_cols;
}

Player* Arena::player() const
{
	return m_player;
}

int Arena::robotCount() const
{
	return m_nRobots;
}

int Arena::getCellStatus(int r, int c) const
{
	checkPos(r, c);
	return m_grid[r - 1][c - 1];
}

int Arena::numberOfRobotsAt(int r, int c) const
{
	int count = 0;
	for (int i = 0; i < m_nRobots; i++) //Go through robot array and check if any robots are in the position being checked
		if (m_robots[i] != nullptr && !(m_robots[i]->isDead()) && m_robots[i]->row() == r && m_robots[i]->col() == c)
			count++;
	return count;
}

void Arena::display(string msg) const
{
	char displayGrid[MAXROWS][MAXCOLS];
	int r, c;

	// Fill displayGrid with dots (empty) and stars (wall)
	for (r = 1; r <= rows(); r++)
		for (c = 1; c <= cols(); c++)
			displayGrid[r - 1][c - 1] = (getCellStatus(r, c) == EMPTY ? '.' : '*');

	// Indicate robot positions by their channels.  If more than one robot
	// occupies a cell, show just one (any one will do).

	for (int i = 0; i < m_nRobots; i++)
	{
		if (m_robots[i] != nullptr && !(m_robots[i]->isDead()))
		{
			char ch;
			switch (m_robots[i]->channel()) //Get the right character for the channel the robot has
			{
				case 1:
					ch = '1';
					break;
				case 2:
					ch = '2';
					break;
				case 3:
					ch = '3';
					break;
			}
			displayGrid[m_robots[i]->row() - 1][m_robots[i]->col() - 1] = ch;
		}
	}

	// Indicate player's position
	if (m_player != nullptr)
		displayGrid[m_player->row() - 1][m_player->col() - 1] = (m_player->isDead() ? 'X' : '@');

	// Draw the grid
	clearScreen();
	for (r = 1; r <= rows(); r++)
	{
		for (c = 1; c <= cols(); c++)
			cout << displayGrid[r - 1][c - 1];
		cout << endl;
	}
	cout << endl;

	// Write message, robot, and player info
	if (msg != "")
		cout << msg << endl;
	cout << "There are " << robotCount() << " robots remaining." << endl;
	if (m_player == nullptr)
		cout << "There is no player!" << endl;
	else if (m_player->isDead())
		cout << "The player is dead." << endl;
}

void Arena::setCellStatus(int r, int c, int status)
{
	checkPos(r, c);
	m_grid[r - 1][c - 1] = status;
}

bool Arena::addRobot(int r, int c, int channel)
{
	if (m_nRobots == MAXROBOTS)
		return false;
	m_robots[m_nRobots] = new Robot(this, r, c, channel);
	m_nRobots++;
	return true;
}

bool Arena::addPlayer(int r, int c)
{
	if (m_player != nullptr)
		return false;
	m_player = new Player(this, r, c);
	return true;
}

string Arena::moveRobots(int channel, int dir)
{
	// Robots on the channel will respond with probability 1/2
	bool willRespond = (randInt(0, 1) == 0);

	// Move all robots
	int nRobotsOriginally = m_nRobots;

	for (int i = 0; i < m_nRobots; i++)
	{
		if (m_robots[i] != nullptr && !(m_robots[i]->isDead()))
		{
			if (m_robots[i]->channel() == channel && willRespond) //If the channel is right and the transmitter works, force a move, otherwise random movement
			{
				m_robots[i]->forceMove(dir);
				if (m_robots[i]->isDead()) //If any robots died, delete them
				{
					delete m_robots[i];
					for (int k = i; k < m_nRobots - 1; k++) //Move all the pointers in the robot array one to the left (no empty spaces)
						m_robots[k] = m_robots[k + 1];
					m_nRobots--;
				}
			}
			else
			{
				m_robots[i]->move();
			}
		}
	}
	if (numberOfRobotsAt(m_player->row(), m_player->col()) > 0) //If a robot moves into the player, the player dies
		m_player->setDead();

	if (m_nRobots < nRobotsOriginally)
		return "Some robots have been destroyed.";
	else
		return "No robots were destroyed.";
}

void Arena::checkPos(int r, int c) const
{
	if (r < 1 || r > m_rows || c < 1 || c > m_cols)
	{
		cout << "***** " << "Invalid arena position (" << r << ","
			<< c << ")" << endl;
		exit(1);
	}
}

///////////////////////////////////////////////////////////////////////////
//  Game implementation
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nRobots)
{
	if (nRobots < 0 || nRobots > MAXROBOTS)
	{
		cout << "***** Game created with invalid number of robots:  "
			<< nRobots << endl;
		exit(1);
	}
	int nEmpty = rows * cols - nRobots - 1;  // 1 for Player
	if (nEmpty < 0)
	{
		cout << "***** Game created with a " << rows << " by "
			<< cols << " arena, which is too small too hold a player and "
			<< nRobots << " robots!" << endl;
		exit(1);
	}

	// Create arena
	m_arena = new Arena(rows, cols);

	// Add some walls in WALL_DENSITY of the empty spots
	assert(WALL_DENSITY >= 0 && WALL_DENSITY <= 1);
	int nWalls = static_cast<int>(WALL_DENSITY * nEmpty);
	while (nWalls > 0)
	{
		int r = randInt(1, rows);
		int c = randInt(1, cols);
		if (m_arena->getCellStatus(r, c) == WALL)
			continue;
		m_arena->setCellStatus(r, c, WALL);
		nWalls--;
	}

	// Add player
	int rPlayer;
	int cPlayer;
	do
	{
		rPlayer = randInt(1, rows);
		cPlayer = randInt(1, cols);
	} while (m_arena->getCellStatus(rPlayer, cPlayer) != EMPTY);
	m_arena->addPlayer(rPlayer, cPlayer);

	// Populate with robots
	while (nRobots > 0)
	{
		int r = randInt(1, rows);
		int c = randInt(1, cols);
		if (m_arena->getCellStatus(r, c) != EMPTY || (r == rPlayer && c == cPlayer))
			continue;
		m_arena->addRobot(r, c, randInt(1, MAXCHANNELS));
		nRobots--;
	}
}

Game::~Game()
{
	delete m_arena;
}

string Game::takePlayerTurn()
{
	for (;;)
	{
		cout << "Your move (n/e/s/w/x or nothing): ";
		string playerMove;
		getline(cin, playerMove);

		Player* player = m_arena->player();
		int dir;

		if (playerMove.size() == 0)
		{
			if (recommendMove(*m_arena, player->row(), player->col(), dir))
				return player->move(dir);
			else
				return player->stand();
		}
		else if (playerMove.size() == 1)
		{
			if (tolower(playerMove[0]) == 'x')
				return player->stand();
			else if (charToDir(playerMove[0], dir))
				return player->move(dir);
		}
		cout << "Player move must be nothing, or 1 character n/e/s/w/x." << endl;
	}
}

string Game::takeRobotsTurn()
{
	for (;;)
	{
		cout << "Broadcast (e.g., 2n): ";
		string broadcast;
		getline(cin, broadcast);
		if (broadcast.size() != 2)
			cout << "Broadcast must be channel followed by direction." << endl;
		else if (broadcast[0] < '1' || broadcast[0] > '0' + MAXCHANNELS)
			cout << "Channel must be a valid digit." << endl;
		else
		{
			int dir;
			if (charToDir(broadcast[1], dir))
				return m_arena->moveRobots(broadcast[0] - '0', dir);
			else
				cout << "Direction must be n, e, s, or w." << endl;
		}
	}
}

void Game::play()
{
	m_arena->display("");
	while (!m_arena->player()->isDead() && m_arena->robotCount() > 0)
	{
		string msg = takePlayerTurn();
		m_arena->display(msg);
		Player* player = m_arena->player();
		if (player->isDead())
			break;
		msg = takeRobotsTurn();
		m_arena->display(msg);
	}
	if (m_arena->player()->isDead())
		cout << "You lose." << endl;
	else
		cout << "You win." << endl;
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementation
///////////////////////////////////////////////////////////////////////////

// Return a uniformly distributed random int from lowest to highest, inclusive
int randInt(int lowest, int highest)
{
	if (highest < lowest)
		swap(highest, lowest);
	return lowest + (rand() % (highest - lowest + 1));
}

bool charToDir(char ch, int& dir)
{
	switch (tolower(ch))
	{
	default:  return false;
	case 'n': dir = NORTH; break;
	case 'e': dir = EAST;  break;
	case 's': dir = SOUTH; break;
	case 'w': dir = WEST;  break;
	}
	return true;
}

// Return false without changing anything if moving one step from (r,c)
// in the indicated direction would hit a wall or run off the edge of the
// arena.  Otherwise, update r and c to the position resulting from the
// move and return true.
bool attemptMove(const Arena& a, int dir, int& r, int& c)
{
	switch (dir)
	{
	case NORTH:
		if (r > 1 && a.getCellStatus(r - 1, c) != WALL) //If there's space for the move and there is no wall, move, otherwise, nothing
		{
			r--;
			return true;
			break;
		}
		else
			break;
	case EAST:
		if (c < a.cols() && a.getCellStatus(r, c + 1) != WALL)
		{
			c++;
			return true;
			break;
		}
		else
			break;
	case SOUTH:
		if (r < a.rows() && a.getCellStatus(r + 1, c) != WALL)
		{
			r++;
			return true;
			break;
		}
		else
			break;
	case WEST:
		if (c > 1 && a.getCellStatus(r, c - 1) != WALL)
		{
			c--;
			return true;
			break;
		}
		else
			break;
	}
	return false;
}

// Recommend a move for a player at (r,c):  A false return means the
// recommendation is that the player should stand; otherwise, bestDir is
// set to the recommended direction to move.
bool recommendMove(const Arena& a, int r, int c, int& bestDir)
{
	int xAdjacent = 0;
	int nAdjacent = 0;
	int eAdjacent = 0;
	int sAdjacent = 0;
	int wAdjacent = 0;

	if (r > 1 && a.getCellStatus(r - 1, c) != WALL) //Add up robots surrounding the player's current position
		xAdjacent += a.numberOfRobotsAt(r - 1, c);
	if (c < a.cols() && a.getCellStatus(r, c + 1) != WALL)
		xAdjacent += a.numberOfRobotsAt(r, c + 1);
	if (r < a.rows() && a.getCellStatus(r + 1, c) != WALL)
		xAdjacent += a.numberOfRobotsAt(r + 1, c);
	if (c > 1 && a.getCellStatus(r, c - 1) != WALL)
		xAdjacent += a.numberOfRobotsAt(r, c - 1);

	if (r - 1 > 0) //Check if one spot to the north is a valid position, then add up adjacent robots
	{
		if (r - 1 > 1 && a.getCellStatus(r - 1 - 1, c) != WALL)
			nAdjacent += a.numberOfRobotsAt(r - 1 - 1, c);
		if (c < a.cols() && a.getCellStatus(r - 1, c + 1) != WALL)
			nAdjacent += a.numberOfRobotsAt(r - 1, c + 1);
		if (r - 1 < a.rows() && a.getCellStatus(r - 1 + 1, c) != WALL)
			nAdjacent += a.numberOfRobotsAt(r - 1 + 1, c);
		if (c > 1 && a.getCellStatus(r - 1, c - 1) != WALL)
			nAdjacent += a.numberOfRobotsAt(r - 1, c - 1);
	}

	if (c + 1 < a.cols() + 1) //Check if one spot to the east is a valid position, then add up adjacent robots
	{
		if (r > 1 && a.getCellStatus(r - 1, c + 1) != WALL)
			eAdjacent += a.numberOfRobotsAt(r - 1, c + 1);
		if (c + 1 < a.cols() && a.getCellStatus(r, c + 1 + 1) != WALL)
			eAdjacent += a.numberOfRobotsAt(r, c + 1 + 1);
		if (r < a.rows() && a.getCellStatus(r + 1, c + 1) != WALL)
			eAdjacent += a.numberOfRobotsAt(r + 1, c + 1);
		if (c + 1 > 1 && a.getCellStatus(r, c + 1 - 1) != WALL)
			eAdjacent += a.numberOfRobotsAt(r, c + 1 - 1);
	}

	if (r + 1 < a.rows() + 1) //Check if one spot to the south is a valid position, then add up adjacent robots
	{
		if (r + 1 > 1 && a.getCellStatus(r + 1 - 1, c) != WALL)
			sAdjacent += a.numberOfRobotsAt(r + 1 - 1, c);
		if (c < a.cols() && a.getCellStatus(r + 1, c + 1) != WALL)
			sAdjacent += a.numberOfRobotsAt(r + 1, c + 1);
		if (r + 1 < a.rows() && a.getCellStatus(r + 1 + 1, c) != WALL)
			sAdjacent += a.numberOfRobotsAt(r + 1 + 1, c);
		if (c > 1 && a.getCellStatus(r + 1, c - 1) != WALL)
			sAdjacent += a.numberOfRobotsAt(r + 1, c - 1);
	}

	if (c - 1 > 0) //Check if one spot to the west is a valid position, then add up adjacent robots
	{
		if (r > 1 && a.getCellStatus(r - 1, c - 1) != WALL)
			wAdjacent += a.numberOfRobotsAt(r - 1, c - 1);
		if (c - 1 < a.cols() && a.getCellStatus(r, c - 1 + 1) != WALL)
			wAdjacent += a.numberOfRobotsAt(r, c - 1 + 1);
		if (r < a.rows() && a.getCellStatus(r + 1, c - 1) != WALL)
			wAdjacent += a.numberOfRobotsAt(r + 1, c - 1);
		if (c - 1 > 1 && a.getCellStatus(r, c - 1 - 1) != WALL)
			wAdjacent += a.numberOfRobotsAt(r, c - 1 - 1);
	}

	bool canMoveNorth;
	bool canMoveEast;
	bool canMoveSouth;
	bool canMoveWest;

	if (r > 1 && a.getCellStatus(r - 1, c) != WALL && a.numberOfRobotsAt(r - 1, c) == 0) //Check which moves the player could make without running into a wall, edge, or robot
		canMoveNorth = true;
	else
		canMoveNorth = false;
	if (c < a.cols() && a.getCellStatus(r, c + 1) != WALL && a.numberOfRobotsAt(r, c + 1) == 0)
		canMoveEast = true;
	else
		canMoveEast = false;
	if (r < a.rows() && a.getCellStatus(r + 1, c) != WALL && a.numberOfRobotsAt(r + 1, c) == 0)
		canMoveSouth = true;
	else
		canMoveSouth = false;
	if (c > 1 && a.getCellStatus(r, c - 1) != WALL && a.numberOfRobotsAt(r, c - 1) == 0)
		canMoveWest = true;
	else
		canMoveWest = false;

	if (xAdjacent <= nAdjacent && xAdjacent <= eAdjacent && xAdjacent <= sAdjacent && xAdjacent <= wAdjacent) //Move to an available spot with the least adjacent robots, if necessary, or possible, otherwise stand still
		return false;
	else if (canMoveNorth && nAdjacent <= xAdjacent && nAdjacent <= eAdjacent && nAdjacent <= sAdjacent && nAdjacent <= wAdjacent)
	{
		bestDir = NORTH;
		return true;
	}
	else if (canMoveSouth && sAdjacent <= xAdjacent && sAdjacent <= eAdjacent && sAdjacent <= nAdjacent && sAdjacent <= wAdjacent)
	{
		bestDir = SOUTH;
		return true;
	}
	else if (canMoveEast && eAdjacent <= xAdjacent && eAdjacent <= nAdjacent && eAdjacent <= sAdjacent && eAdjacent <= wAdjacent)
	{
		bestDir = EAST;
		return true;
	}
	else if (canMoveWest && wAdjacent <= xAdjacent && wAdjacent <= nAdjacent && wAdjacent <= sAdjacent && wAdjacent <= eAdjacent)
	{
		bestDir = WEST;
		return true;
	}
	else
		return false;
}

///////////////////////////////////////////////////////////////////////////
//  main()
///////////////////////////////////////////////////////////////////////////

int main()
{
	// Initialize the random number generator
	srand(static_cast<unsigned int>(time(0)));

	// Create a game
	// Use this instead to create a mini-game:   Game g(3, 5, 2);
	Game g(10, 12, 50);

	// Play the game
	g.play();
}

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANY CODE BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++ UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _MSC_VER  //  Microsoft Visual C++

#pragma warning(disable : 4005)
#include <windows.h>

void clearScreen()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	COORD upperLeft = { 0, 0 };
	DWORD dwCharsWritten;
	FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
		&dwCharsWritten);
	SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Microsoft Visual C++, so assume UNIX interface

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen()  // will just write a newline in an Xcode output window
{
	static const char* term = getenv("TERM");
	if (term == nullptr || strcmp(term, "dumb") == 0)
		cout << endl;
	else
	{
		static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
		cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
	}
}

#endif