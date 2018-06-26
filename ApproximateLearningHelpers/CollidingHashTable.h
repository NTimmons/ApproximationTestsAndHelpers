#pragma once

template<typename Key, typename T, unsigned int Size, unsigned int(*hashFunction)(T)>
class CollidingHashTable
{
public:
    CollidingHashTable() : m_lastOperationIndex(0), m_staleThreshold(Size)
    {
    }
    struct HashEntry
    {
        HashEntry() { hits = 0; hashValue = -1; storageValue = -1; lastTouchIndex = 0; }
        T hashValue;
        T storageValue;
        unsigned int hits;
        unsigned int lastTouchIndex;
    };

    int Get(Key _hashValue, T& result)
    {
        m_lastOperationIndex++;

        unsigned int hash = hashFunction(_hashValue);
        if (m_table[hash].hits == 0)
        {
            return -1;
        }
        else
        {
            m_table[hash].hits++;
            result = m_table[hash].storageValue;
            m_table[hash].lastTouchIndex = m_lastOperationIndex;
            return 1;
        }
    }

    //Ret: 0-> Added, 1 -> Replaced, 2-> Occupied, -1 -> Error
    int Insert(Key _hashValue, T _storageValue)
    {
        //Increment the index of this operation and get the hash index.
        m_lastOperationIndex++;
        unsigned int hash = hashFunction(_hashValue);

        if (hash < 0 || hash >= Size)
        {
            return -1;
        }

        int returnVal = -1;

        //If the entry is empty or stale replace.
        if (m_table[hash].hits == 0)
        {
            returnVal = 0;
        }
        else if ((m_lastOperationIndex - m_table[hash].lastTouchIndex) > m_staleThreshold)
        {
            returnVal = 1;
        }
        else
        {
            return 2;
        }

        m_table[hash].storageValue = _storageValue;
        m_table[hash].hashValue = _hashValue;
        m_table[hash].hits = 1;
        m_table[hash].lastTouchIndex = m_lastOperationIndex;

        return returnVal;
    }

    std::string ToString()
    {
        std::string text;
        text.append("hashValue");
        text.append(", ");
        text.append("storageValue");
        text.append(", ");
        text.append("hits");
        text.append(", ");
        text.append("lastTouchIndex");
        text.append("\n");
        for (int i = 0; i < Size; i++)
        {
            text.append(std::to_string(m_table[i].hashValue));
            text.append(",");
            text.append(std::to_string(m_table[i].storageValue));
            text.append(",");
            text.append(std::to_string(m_table[i].hits));
            text.append(",");
            text.append(std::to_string(m_table[i].lastTouchIndex));
            text.append("\n");
        }
        return text;
    }

    std::array<HashEntry, Size> m_table;
    unsigned int				m_lastOperationIndex;
    unsigned int				m_staleThreshold;
};