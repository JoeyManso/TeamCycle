#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int gPlayersTotal = 8;
int gPlayersPerTeam = 2;
bool gUniqueTeams = true;
bool gPrintColors = true;

const vector<char> COLORS = {'R', 'B', 'G', 'Y', 'W', 'T', 'P', 'A'};
char GetTeamColor(int teamIdx)
{
    return(COLORS[teamIdx]);
}
string PlayerStr(int playerIdx)
{
    return("P" + to_string(playerIdx + 1));
}

typedef vector<string> Team;
int GetPlayerTeamIdx(string player, const vector<Team>& teams)
{
    for(size_t i = 0; i<teams.size(); ++i)
    {
        const Team& team = teams[i];
        if(find(team.begin(), team.end(), player) != team.end())
        {
            return(i);
        }
    }
    return -1;
}

struct Game
{
    vector<Team> teams;
    int totalPlayers() const {return(teams.size() * gPlayersPerTeam);}

    bool HasTeam(const Team& teamOther) const
    {
        for(const Team& team : teams)
        {
            if(team == teamOther)
            {
                return(true);
            }
        }
        return(false);
    }

    bool HasTeamPlayer(const Team& teamOther) const
    {
        for(string player : teamOther)
        {
            if(GetPlayerTeamIdx(player, teams) != -1)
            {
                return(true);
            }
        }
        return(false);
    }

    void PrintColors() const
    {   
        for(int i = 0; i < gPlayersTotal; ++i)
        {
            int tIdx = GetPlayerTeamIdx(PlayerStr(i), teams);
            cout << GetTeamColor(tIdx);
            if((i+1) % gPlayersPerTeam == 0)
            {
                cout << "  ";
            }
        }
        cout << endl;
    }

    void PrintPlayers() const
    {
        for(const Team& team : teams)
        {
            for(const string& player : team)
            {
                cout << player;
            }
            cout << "  ";
        }
        cout << endl;
    }
};

void PopulateTeams(int offset, Team& team, vector<Team>& teams)
{
    if(team.size() == gPlayersPerTeam)
    {
        teams.push_back(team);
        return;
    }
    for(int i = offset; i < gPlayersTotal; ++i)
    {
        team.push_back(PlayerStr(i));
        PopulateTeams(i+1, team, teams);
        team.pop_back();
    }
}

bool IsGameUnique(const Game& gameOther, const vector<Game>& games)
{
    for(const Game& game : games)
    {
        for(const Team& team : game.teams)
        {
            if(gameOther.HasTeam(team))
            {
                return(false);
            }
        }
    }
    return(true);
}

void PopulateGames(int offset, Game& game, vector<Game>& games, vector<Team>& teams)
{
    if(game.totalPlayers() == gPlayersTotal)
    {
        if(!gUniqueTeams || IsGameUnique(game, games))
        {
            games.push_back(game);
        }
        return;
    }
    for(size_t i = offset; i < teams.size(); ++i)
    {
        const Team& team = teams[i];
        if(!game.HasTeamPlayer(team))
        {
            game.teams.push_back(team);
            PopulateGames(i + 1, game, games, teams);
            game.teams.pop_back();
        }
    }
}

int main(int argc, const char* argv[])
{
    if(argc > 1) {gPlayersTotal = atoi(argv[1]);}
    if(argc > 2) {gPlayersPerTeam = atoi(argv[2]);}
    if(argc > 3) {gUniqueTeams = (string(argv[3]) == "true");}
    if(argc > 4) {gPrintColors = (string(argv[4]) == "true");}
    
    cout << "UNIQUE TEAM CYCLE" << endl << endl;

    if(gPlayersTotal < 0 || gPlayersPerTeam < 0 || gPlayersPerTeam > gPlayersTotal 
    || gPlayersTotal % gPlayersPerTeam != 0 
    || size_t(gPlayersPerTeam / gPlayersTotal) > COLORS.size())
    {
        cout << "INVALID PARAMS" << endl;
        system("pause");
        return EXIT_FAILURE;
    }

    Team t;
    vector<Team> teams;
    PopulateTeams(0, t, teams);

    Game g;
    vector<Game> games;
    PopulateGames(0, g, games, teams);

    for(const Game& g : games)
    {
        gPrintColors ? g.PrintColors() : g.PrintPlayers();
    }
    cout << endl;

    system("pause");
    return EXIT_SUCCESS;
}