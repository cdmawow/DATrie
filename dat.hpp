/* Copyright (C), 1998-2011, Tencent Technology Company Limited
 *
 * @author genelin
 * @since 2011-10-17
 * @brief double array trie 字典
 */

#ifndef DA_TRIE_DICT_H
#define DA_TRIE_DICT_H

#include <stdint.h>
#include <string>
#include <stdexcept>
#include "string_util.hpp"
#include "double_array_trie.h"

namespace new_word
{

template<typename TValue>
class IDATrieDict
{
public:
    IDATrieDict() {};
    virtual ~IDATrieDict() {};

    virtual int LoadRawDict(const char *file_name) = 0;
    virtual int LoadBinDict(const char *key_fname, const char *val_fname) = 0;
    virtual int SaveBinDict(const char *key_fname, const char *val_fname) = 0;
    virtual int Release() = 0;

    // 最长匹配
    // @param text - 查找文本
    // @param seg_position - 分词位置
    // @param value[out] - 如果匹配成功，value指向对应的值的地址
    // @return term_num - 匹配到几个term
    virtual uint32_t LongestMatch(const char *text,
        const std::vector<uint32_t> &seg_pos, TValue **value) = 0;

    // 完全匹配
    // @param text - 查找文本
    // @param text_len - 文本长度
    // @param value - 如果匹配成功，value指向对应的值的地址
    // @return value - 命中返回true， 否则返回false
    virtual bool ExactMatch(const char *text, uint32_t text_len, TValue **value) = 0;
};



/* 简单的DATrie字典
 * RawDict每行格式: "key\tvalue", 其中value域为数字，做为字典的value
 */
class IntDATrieDict : public IDATrieDict<int>
{
public:
    IntDATrieDict() {}
    virtual ~IntDATrieDict() {}

    virtual int LoadRawDict(const char *file_name);
    virtual int LoadBinDict(const char *key_fname, const char *val_fname);
    virtual int SaveBinDict(const char *key_fname, const char *val_fname);
    virtual int Release();
    virtual int Clear() {m_trie.Clear(); return 0;}
    virtual uint32_t LongestMatch(const char *text,
        const std::vector<uint32_t> &seg_pos, int **value);

    uint32_t LongestMatch(const char *text,
        const std::vector<uint32_t> &seg_pos, uint32_t offset, int **value);

    virtual bool ExactMatch(const char *text, uint32_t text_len, int **value);

private:
    new_word::DATrie m_trie;
    std::vector<int> m_value;
};


/* 简单的DATrie字典
 * RawDict每行格式: "key\tvalue", 其中value域为数字，做为字典的value
 */
class FloatDATrieDict : public IDATrieDict<float>
{
public:
    FloatDATrieDict() {}
    virtual ~FloatDATrieDict() {}

    virtual int LoadRawDict(const char *file_name);
    virtual int LoadBinDict(const char *key_fname, const char *val_fname);
    virtual int SaveBinDict(const char *key_fname, const char *val_fname);
    virtual int Release();
    virtual int Clear() { m_trie.Clear(); return 0;}
    virtual uint32_t LongestMatch(const char *text,
        const std::vector<uint32_t> &seg_pos, float **value);

    virtual bool ExactMatch(const char *text, uint32_t text_len, float **value);
private:
    new_word::DATrie m_trie;
    std::vector<float> m_value;

};
} // namespace new_word

#endif // DA_TRIE_DICT_H


