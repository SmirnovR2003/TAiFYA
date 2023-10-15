#include <vector>
#include <string>
#include <fstream>
#include<map>

using namespace std;

typedef vector<vector<pair<int, int>>> Mily;
typedef vector<vector< int>> Mur;
typedef vector<int> MurSignals;

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
    for (int i = 0; i < n-1; ++i)
    {
        string s;
        getline(inp, s, ' ');
        s.erase(0, 2);
        sygn.push_back(atoi(s.c_str()));
    }
    string s;
    getline(inp, s);
    s.erase(0, 1);
    sygn.push_back(atoi(s.c_str()));

    for (int i = 0; i < m; ++i)
    {
        vector<int> vec;
        for (int i = 0; i < n-1; ++i) {

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
    outDot << "digraph {\n";
    for (int i = 0; i < mur.size(); ++i) {
        for (int k = 0; k < mur[i].size(); ++k) {
            outDot << "  \"q" << k << "/y" << sygn[k]  << "\" -> \"q" << mur[i][k] << "/y" << sygn[mur[i][k]] << "\" [ label = \"x" << i << "\" ];\n";
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


void FromMilyToMur(MurSignals& sygn, Mur& mur, Mily& mily)
{
    map<pair<int, int>, pair<int, int>> sections;
    for (int i = 0; i < mily.size();++i)
    {
        for (int k = 0; k < mily[i].size(); ++k)
        {
            if (sections.count(mily[i][k]) == 0)
            {
                sections.insert({ mily[i][k], {0, k} });
            }
        }
    }
    int count = 0;
    for (auto& sec : sections)
    {
        sec.second.first = count++;
    }

    sygn.resize(sections.size());
    mur.resize(mily.size());
    for (auto& m : mur)
    {
        m.resize(sections.size());
    }

    for (const auto& sec : sections)
    {
        for (int i = 0; i < mily.size(); i++)
        {
            mur[i][sec.second.first] = sections.find(mily[i][sec.first.first])->second.first;
            sygn[sec.second.first] = sec.first.second;
        }
    }

}

void FromMurToMily(MurSignals& sygn, Mur& mur, Mily& mily)
{
    for (int i = 0; i < mur.size();++i)
    {
        vector<pair<int, int>> mi;
        for (int k = 0; k < mur[i].size(); ++k)
        {
            pair<int, int> p = { mur[i][k], sygn[k]};
            mi.push_back(p);
        }
        mily.push_back(mi);
    }
}



int main(int argc, char* argv[])
{
    ifstream inp(argv[1]);
    ofstream outDot("out.dot");
    ofstream out(argv[2]);

    int n, m, type;
    inp >> n >> m >> type;
    Mily mily;
    Mur mur;
    MurSignals murSign;

    if (type == 1)
    {
        ReadMilyGraph(inp, n, m, mily);
        FromMilyToMur(murSign, mur, mily);
        WriteMurGraph(outDot, out, murSign, mur);
    }
    else
    {
        ReadMurGraph(inp, n, m, murSign, mur);
        FromMurToMily(murSign, mur, mily);
        WriteMily(outDot, out, mily);
    }

    outDot.close();

    system("C:/roman/ТАиФЯ/Graphviz/bin/dot.exe out.dot -Tpng -oout.png");
    system("out.png");
}