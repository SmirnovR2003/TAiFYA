#include <vector>
#include <string>
#include <fstream>
#include<map>
#include <set>

using namespace std;

struct SMinimiz
{
    int vertexTo;
    int y;
};

typedef vector<vector<pair<int, int>>> Mily;
typedef vector<vector<int>> Mur;
typedef vector<int> MurSignals;

typedef map<int, map<int, vector<pair<int, int>>>> Minimiz;

void ReadMilyGraph(ifstream& inp, int n, int m, Mily& mily)
{
    for (int i = 0; i < m; ++i)
    {
        vector<pair<int, int>> vecPairs;
        for (int k = 0; k < n; ++k) {
            char ch;
            int to, signal;
            inp >> ch >> to >> ch >> ch >> signal;
            vecPairs.push_back({ to,signal });
        }
        mily.push_back(vecPairs);
    }
}

void ReadMurGraph(ifstream& inp, int n, int m, MurSignals& sygn, Mur& mur)
{

    inp.get();
    for (int i = 0; i < n - 1; ++i)
    {
        string s;
        getline(inp, s, ' ');
        s.erase(0, 1);
        sygn.push_back(atoi(s.c_str()));
    }
    string s;
    getline(inp, s);
    s.erase(0, 1);
    sygn.push_back(atoi(s.c_str()));

    for (int i = 0; i < m; ++i)
    {
        vector<int> vec;
        for (int i = 0; i < n - 1; ++i) {

            string s;
            getline(inp, s, ' ');
            s.erase(0, 1);
            vec.push_back(atoi(s.c_str()));
        }
        string s;
        getline(inp, s);
        s.erase(0, 1);
        vec.push_back(atoi(s.c_str()));
        mur.push_back(vec);
    }
}

void WriteMily(ofstream& outDot, ofstream& out, const Mily& mily)
{
    outDot << "digraph {\n";
    for (int i = 0; i < mily.size(); ++i) {
        for (int k = 0; k < mily[i].size(); ++k) {
            outDot << "  \"s" << k << "\" -> \"s" << mily[i][k].first << "\" [ label = \"x" << i << "/" << "y" << mily[i][k].second << "\" ];\n";
        }
    }
    outDot << "}\n";

    for (auto& m : mily)
    {
        for (auto& section : m)
        {
            out << "s" << section.first << "/y" << section.second << " ";
        }
        out << "\n";
    }
}

void WriteMurGraph(ofstream& outDot, ofstream& out, MurSignals& sygn, Mur& mur)
{
    bool isZero = false;
    for (int i = 0; i < mur.size(); ++i) {
        for (int k = 0; k < mur[i].size(); ++k) {
            if (mur[i][k] == 0)
            {
                isZero = true;
                break;
            };
        }
    }
    if (!isZero)
    {
        for (int i = 0; i < mur.size(); ++i) {
            for (int k = 0; k < mur[i].size(); ++k) {
                mur[i][k] -= 1;
            }
        }
    }
    outDot << "digraph {\n";
    for (int i = 0; i < mur.size(); ++i) {
        for (int k = 0; k < mur[i].size(); ++k) {
            outDot << "  \"q" << k << "/y" << sygn[k] << "\" -> \"q" << mur[i][k] << "/y" << sygn[mur[i][k]] << "\" [ label = \"x" << i << "\" ];\n";
        }
    }
    outDot << "}\n";

    for (auto s : sygn)
    {
        out << "y" << s << " ";
    }
    out << "\n";
    for (auto& m : mur)
    {
        for (auto& section : m)
        {
            out << "q" << section << " ";
        }
        out << "\n";
    }
}

int FindSection(Minimiz& minimiz, int c)
{
    for (auto& section : minimiz)
    {
        for (auto& col : section.second)
        {
            if (col.first == c)return section.first;
        }
    }
    return -1;
} 

bool CheckEqual(Minimiz& minimiz1, Minimiz& minimiz2)
{
    for (auto& section : minimiz1)
    {
        for (auto& col : section.second)
        {
            int i = 0;
            for (auto& c : col.second)
            {
                if (minimiz2[section.first][col.first][i] != c)
                {
                    return false;
                }
                i++;
            }
        }
    }
    return true;
}

vector<pair<int, int>>GetCol(Minimiz& minimiz, int colNumber)
{
    for (auto& section : minimiz)
    {
        for (auto& col : section.second)
        {
            if (col.first == colNumber) return col.second;
        }
    }
}

Minimiz Split(Minimiz& minimiz)
{
    //typedef map<int, map<int, vector<pair<int, int>>>> Minimiz;*/
    Minimiz newMinimiz{};
    int i = 0;
    for (auto& section : minimiz)
    {
        //set<vector<pair<int,int>>> cols;
        map<vector<int>, vector<int>> cols;
        for (auto& col : section.second)
        {
            vector<int> vec;
            for (auto& c : col.second)
            {
                vec.push_back(c.first);
            }
            cols[vec].push_back(col.first);
        }
        map<int, vector<pair<int, int>>> newCols;
        for (auto& col : cols)
        {
            for (auto& c : col.second)
            {
                newCols[c] = GetCol(minimiz, c);
            }
            newMinimiz[i++] = newCols;
            newCols.clear();
        }
    }
    return newMinimiz;
}

Minimiz Minimization(Minimiz minimiz)
{
    Minimiz newMinimiz{};
    for (auto& section : minimiz)
    {
        for (auto& col : section.second)
        {
            for (auto& c : col.second)
            {
                newMinimiz[section.first][col.first].push_back({ FindSection(minimiz, c.second), c.second });
            }
        }
    }

    newMinimiz = Split(newMinimiz);

    for (auto& section : newMinimiz)
    {
        for (auto& col : section.second)
        {
            vector<pair<int, int>> vec;
            for (auto& c : col.second)
            {
                vec.push_back({ FindSection(newMinimiz, c.second), c.second });
            }
            newMinimiz[section.first][col.first] = vec;
        }
    }

    if (minimiz.size() == newMinimiz.size())
    {
        return minimiz;
    }
    else
    {
        return Minimization(newMinimiz);
    }

}

void MinimizationMur(Mur& mur, Mur& minMur, MurSignals& murSign, MurSignals& newMurSign)
{
    Minimiz minimiz;
    for (int i = 0; i < mur.size(); i++)
    {
        for (int k = 0; k < mur[i].size(); k++)
        {
            minimiz[murSign[k]][k].push_back({ murSign[mur[i][k]], mur[i][k] });
        }
    }
    minimiz = Minimization(minimiz);
    minMur.resize(mur.size());
    newMurSign.clear();
    
    for (auto& section : minimiz)
    {
        vector<int> murCol;
        int i = 0;
        for (auto& col : section.second)
        {
            for (int k = 0; k < col.second.size(); k++)
            {
                minMur[k].push_back(col.second[i].first);
                i++;
            }
            newMurSign.push_back(murSign[col.first]);
            break;
        }
    }

}

void MinimizationMily(Mily& mily)
{
    /*
    typedef vector<vector<pair<int, int>>> Mily;

     typedef map<int, map<int, vector<pair<int, int>>>> Minimiz;*/


    Minimiz minimiz;

    map<vector<int>, vector <int>> uniqueSignals;
    vector<int> colSignals;

    int i = 0;
    for (auto& col : mily)
    {
        for (auto& c : col)
        {
            colSignals.push_back(c.second);
        }
        uniqueSignals[colSignals].push_back(i++);
    }

    for (auto& sec : uniqueSignals)
    {

    }

}

int main(int argc, char* argv[])
{
    ifstream inp("MurWithoutMin.txt");
    ofstream outDot("out.dot");
    ofstream out("out.txt");

    int n, m, type;
    inp >> n >> m >> type;
    Mily mily;
    Mur mur, minMur;
    MurSignals murSign, minMurSign;

    if (type == 1)
    {
        ReadMilyGraph(inp, n, m, mily);
        WriteMily(outDot, out, mily);
    }
    else
    {
        ReadMurGraph(inp, n, m, murSign, mur);
        MinimizationMur(mur, minMur, murSign, minMurSign);
        WriteMurGraph(outDot, out, minMurSign, minMur);
    }

    outDot.close();

    system("C:/roman/ТАиФЯ/Graphviz/bin/dot.exe out.dot -Tpng -oout.png");
    system("out.png");
}