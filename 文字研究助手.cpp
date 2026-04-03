#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;



string toLower(const string& str)//将所有单词转换成小写，方便后续识别
{
    string result;
    for (char c : str)
    {
        result += tolower(c);
    }
    return result;
}

vector<string> extractWord(const string& line)//提取文章中的单词
{
    vector<string> words;
    string word;
    for (char ch : line)
    {
        if (isalpha(ch))
        {
            word += tolower(ch);
        }
        else//识别到空格
        {
            if (!word.empty()) //有完整单词
            {
                words.push_back(word);//将单词存入vector
                word.clear();//清空word，为读取下个单词做准备
            }
        }
    }

    if (!word.empty())//处理行尾单词没有空格结尾
    {
        words.push_back(word);
    }
    return words;
}

int findKeyword(const string keywords[], int keywordcnt, const string& word)
{
    for (int i = 0; i < keywordcnt; i++)
    {
        if (keywords[i] == word)
        {
            return i;//返回索引
        }
    }
    return -1;
}

int main()
{
    string filename;
    cout << "请输入文件名: ";
    cin >> filename;
    cin.ignore();  // 清除回车符

    ifstream infile(filename);
    if (!infile.is_open())
    {
        cerr << "无法打开文件" << filename << endl;
        return 1;

    }

    string keywordLine;
    getline(infile, keywordLine);//读取一行

    string keywords[100];//储存要识别的关键词
    int keywordscnt = 0;//实际存储的词数

    // 添加调试信息
    cout << "读取的第一行内容是: [" << keywordLine << "]" << endl;
    cout << "第一行长度: " << keywordLine.length() << endl;



    stringstream ss(keywordLine);//输出关键词列表
    string  kw;
    cout << "需要统计的单词为:";
    while (ss >> kw && keywordscnt < 100)
    {
        keywords[keywordscnt] = toLower(kw);
        cout << keywords[keywordscnt] << ",";
        keywordscnt++;
    }
    cout << endl;

    // 添加调试信息
    cout << "共读取到 " << keywordscnt << " 个要统计的单词" << endl;


    int wordCount[100] = { 0 };
    vector<int> wordLines[100];//统计数组

    string line;//逐行扫描
    int lineNo = 1;

    while (getline(infile, line))
    {
        vector<string>words = extractWord(line);

        for (const string& word : words)
        {
            int idx = findKeyword(keywords, keywordscnt, word);
            if (idx != -1)
            {
                wordCount[idx]++;
                if (wordLines[idx].empty() || wordLines[idx].back() != lineNo)
                {
                    wordLines[idx].push_back(lineNo);
                }
            }
        }
        lineNo++;
    }

    cout << "================== 统计结果 ==================\n\n";

    for (int i = 0; i < keywordscnt; i++) {
        cout << "单词: " << keywords[i] << endl;
        cout << "  出现次数: " << wordCount[i] << endl;
        cout << "  所在行号: ";

        if (wordLines[i].empty()) {
            cout << "无";
        }
        else {
            for (size_t j = 0; j < wordLines[i].size(); j++) {
                if (j > 0) cout << ", ";
                cout << wordLines[i][j];
            }
        }
        cout << "\n\n";
    }

    return 0;



}