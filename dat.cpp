#include <fstream>
#include <map>
#include "dat.hpp"

using namespace std;

int IntDATrieDict::LoadRawDict(const char *file_name)
{
    map<string, int> key_map;
    map<string, int>::const_iterator it;
    vector<string> lines;
    vector<const char *> key_vec;
    vector<size_t> key_len_vec;
    vector<int> value_index_vec;
    m_value.clear();
    if (file_util::ReadTextFile(file_name, lines) < 0)
        return -1;
    for (size_t i = 0; i < lines.size(); i++)
    {
        vector<string> fields;
        string_util::Split(lines[i], "\t", fields);
        if (fields.size() < 2)
            continue;
        m_value.push_back(atoi(fields[1].c_str()));
        key_map[fields[0]] = m_value.size() - 1;
    }

    // Build da_trie
    if (key_map.size() == 0)
        return -1;
    for (it = key_map.begin(); it != key_map.end(); it++)
    {
        key_vec.push_back(it->first.c_str());
        key_len_vec.push_back(it->first.length());
        value_index_vec.push_back(it->second);
    }
    if (m_trie.Build(key_vec.size(), &key_vec[0],
        &key_len_vec[0], &value_index_vec[0]))
    {
        return 0;
    }
    return -1;
}

int IntDATrieDict::LoadBinDict(const char *key_fname, const char *val_fname)
{
    if (!m_trie.Open(key_fname))
        return -1;
    m_value.clear();
    vector<string> lines;
    file_util::ReadTextFile(val_fname, lines);
    for (size_t i = 0; i < lines.size(); i++)
    {
        m_value.push_back(atoi(lines[i].c_str()));
    }
    return 0;
}

int IntDATrieDict::SaveBinDict(const char *key_fname, const char *val_fname)
{
    if (!m_trie.Save(key_fname))
        return -1;

    FILE *fout = fopen(val_fname, "wb");
    for (uint32_t i = 0; i < m_value.size(); i ++)
    {
        fprintf(fout, "%d\n", m_value[i]);
    }
    fclose(fout);
    return 0;
}

int IntDATrieDict::Release()
{
    return 0;
}


bool IntDATrieDict::ExactMatch(const char *text, uint32_t text_len, int **value)
{
    int val = m_trie.ExactMatchSearch(text, text_len);
    if (val < 0 || (size_t)val >= m_value.size())
        return false;
    *value = &m_value[val];
    return true;
}

uint32_t IntDATrieDict::LongestMatch(const char *text,
    const std::vector<uint32_t> &seg_pos, int **value)
{
    /*
    uint32_t seg_num = seg_pos.size();
    if (seg_num == 0)
        return 0;

    size_t search_len = seg_pos[seg_num -1];
    while (1)
    {
        size_t match_len = 0;
        int val = m_trie.LongestMatchSearch(text, match_len, search_len);
        if (val == -1 || (size_t)val > m_value.size())
            return 0;

        // Check match_len;
        for (uint32_t i = 0; i < seg_num; i++)
        {
            if (seg_pos[i] == match_len)
            {
                *value = &m_value[val];
                return i + 1;
            }
        }

        search_len = match_len - 1;
        if (search_len < seg_pos[0] ||
            search_len > seg_pos[seg_num -1])
            return 0;
    }

    return 0;*/
    return LongestMatch(text, seg_pos, 0, value);
}

uint32_t IntDATrieDict::LongestMatch(const char *text,
        const std::vector<uint32_t> &seg_pos, uint32_t offset, int **value)
{
    if (offset >= seg_pos.size()) {
        return 0;
    }

    size_t beg_pos = offset == 0 ? 0 : seg_pos[offset - 1];
    size_t search_len = seg_pos[seg_pos.size() -1] - beg_pos;
    while (1) {
        size_t match_len = 0;
        int val = m_trie.LongestMatchSearch(text + beg_pos, match_len, search_len);
        if (val == -1 || (size_t)val > m_value.size() || match_len == 0)
            return 0;

        // Check match_len;
        for (uint32_t i = offset; i < seg_pos.size(); i++){
            if (seg_pos[i] == beg_pos + match_len) {
                *value = &m_value[val];
                return i + 1 - offset;
            }
        }

        search_len = match_len - 1;
        if (search_len + beg_pos < seg_pos[offset] ||
            search_len + beg_pos > seg_pos[seg_pos.size() -1])
            return 0;
    }

    return 0;
}


int FloatDATrieDict::LoadRawDict(const char *file_name)
{
    map<string, int> key_map;
    map<string, int>::const_iterator it;
    vector<string> lines;
    vector<const char *> key_vec;
    vector<size_t> key_len_vec;
    vector<int> value_index_vec;
    m_value.clear();
    if (file_util::ReadTextFile(file_name, lines) < 0)
        return -1;
    for (size_t i = 0; i < lines.size(); i++)
    {
        vector<string> fields;
        string_util::Split(lines[i], "\t", fields);
        if (fields.size() < 2)
            continue;
        m_value.push_back(atof(fields[1].c_str()));
        key_map[fields[0]] = m_value.size() - 1;
    }

    // Build da_trie
    if (key_map.size() == 0)
        return -1;
    for (it = key_map.begin(); it != key_map.end(); it++)
    {
        key_vec.push_back(it->first.c_str());
        key_len_vec.push_back(it->first.length());
        value_index_vec.push_back(it->second);
    }
    if (m_trie.Build(key_vec.size(), &key_vec[0],
        &key_len_vec[0], &value_index_vec[0]))
    {
        return 0;
    }
    return -1;
}

int FloatDATrieDict::LoadBinDict(const char *key_fname, const char *val_fname)
{
    if (!m_trie.Open(key_fname))
        return -1;
    m_value.clear();
    vector<string> lines;
    file_util::ReadTextFile(val_fname, lines);
    for (size_t i = 0; i < lines.size(); i++)
    {
        m_value.push_back(atof(lines[i].c_str()));
    }
    return 0;
}

int FloatDATrieDict::SaveBinDict(const char *key_fname, const char *val_fname)
{
    if (!m_trie.Save(key_fname))
        return -1;

    FILE *fout = fopen(val_fname, "wb");
    for (uint32_t i = 0; i < m_value.size(); i ++)
    {
        fprintf(fout, "%f\n", m_value[i]);
    }
    fclose(fout);
    return 0;
}

int FloatDATrieDict::Release()
{
    return 0;
}


bool FloatDATrieDict::ExactMatch(const char *text, uint32_t text_len, float **value)
{
    int val = m_trie.ExactMatchSearch(text, text_len);
    if (val < 0 || (size_t)val >= m_value.size())
        return false;
    *value = &m_value[val];
    return true;
}

uint32_t FloatDATrieDict::LongestMatch(const char *text,
    const std::vector<uint32_t> &seg_pos, float **value)
{
    uint32_t seg_num = seg_pos.size();
    if (seg_num == 0)
        return 0;

    size_t search_len = seg_pos[seg_num -1];
    while (1)
    {
        size_t match_len = 0;
        int val = m_trie.LongestMatchSearch(text, match_len, search_len);
        if (val == -1 || (size_t)val > m_value.size())
            return 0;

        // Check match_len;
        for (uint32_t i = 0; i < seg_num; i++)
        {
            if (seg_pos[i] == match_len)
            {
                *value = &m_value[val];
                return i + 1;
            }
        }

        search_len = match_len - 1;
        if (search_len < seg_pos[0] ||
            search_len > seg_pos[seg_num -1])
            return 0;
    }

    return 0;
}


