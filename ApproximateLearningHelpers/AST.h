#pragma once
#include <vector>
#include <map>

enum AST_TYPES
{
    T_EQA,
    T_VAL, //Constant Value
    T_ADD,
    T_SUB,
    T_MUL,
    T_DIV,
    T_FUNC, //NOT SUPPORTED
    T_INPUT,
    T_ROUND,
    T_POW,
};

static const char* AST_TypeNames[] =
{
    "EQA",
    "VAL", //Constant Value
    "ADD",
    "SUB",
    "MUL",
    "DIV",
    "FUNC", //NOT SUPPORTED
    "INPUT",
    "ROUND",
    "POW"
};

template<typename T>
class AST
{
public:
    AST(AST_TYPES _type) : m_resolved(false), m_type(_type), m_result(-123), m_refCount(1)
    {
        static int inputCount = 0;

        if (_type == T_INPUT)
        {
            m_inputID = inputCount;
            inputCount++;
        }
    }

    ~AST()
    {
    }

    AST_TYPES m_type;
    std::vector<AST*> m_connections;

    bool m_resolved;
    T    m_result;
    int  m_refCount;
    int  m_inputID;

    void Print(std::string& _output, int level = 0)
    {
        for (int i = 0; i < level - 1; i++)
        {
            _output.append("|\t");
        }
        if (level > 0)
        {
            _output.append("|----");
            _output.append("-");
        }


        if (m_type != T_VAL && m_type != T_INPUT)
        {
            _output.append(AST_TypeNames[m_type]);
        }
        else if (m_type == T_VAL)
        {
            _output.append(std::to_string(m_result));
        }
        else if (m_type == T_INPUT)
        {
            _output.append("INPUT_");
            _output.append(std::to_string(m_inputID));
        }

        _output.append("\n");
        for (auto i : m_connections)
        {
            i->Print(_output, level + 1);
        }
    }

    void SetInput(int _inputVal)
    {
        if (m_type == T_INPUT)
        {
            m_result = _inputVal;
            m_resolved = true;
        }
    }

    T Execute()
    {
        if (m_resolved)
        {
            return m_result;
        }
        else
        {
            switch (m_type)
            {
            case T_EQA:
            {
                m_result = m_connections[0]->Execute();
                break;
            }
            case T_VAL:
            {
                break;
            }
            case T_ADD:
            {
                m_result = m_connections[0]->Execute() + m_connections[1]->Execute();
                break;
            }
            case T_SUB:
            {
                m_result = m_connections[0]->Execute() - m_connections[1]->Execute();
                break;
            }
            case T_MUL:
            {
                m_result = m_connections[0]->Execute() * m_connections[1]->Execute();
                break;
            }
            case T_DIV:
            {
                m_result = m_connections[0]->Execute() / m_connections[1]->Execute();
                break;
            }
            case T_FUNC:
            {
                // NOT IMPLEMENTED
                break;
            }
            case T_POW:
            {
                T n = m_connections[0]->Execute();
                T p = m_connections[1]->Execute();
                m_result = std::pow(n, p);
                break;
            }
            case T_ROUND:
            {
                m_result = std::round(m_connections[0]->Execute());
                break;
            }
            }
        }

        m_resolved = true;
        return m_result;
    }

    void AddConnection(AST* _node)
    {
        _node->IncRefChildren();
        m_connections.push_back(_node);
    }

    void ClearTree()
    {
        if (m_type != T_VAL && m_type != T_INPUT)
        {
            m_resolved = false;
            m_result = -123;
            for (auto i : m_connections)
            {
                i->ClearTree();
            }
        }
    }

    void IncRefChildren()
    {
        m_refCount++;
        for (auto i : m_connections)
            i->IncRefChildren();
    }
    void DeRefChildren()
    {
        m_refCount--;
        for (auto i : m_connections)
            i->DeRefChildren();
    }
};



template< typename T >
class TreeOfTree
{
public:
    TreeOfTree()
    {
        m_value = 0;
        Init();

    }

    TreeOfTree(AST<T>* _child)
    {
        m_value = _child;
        Init();

    }

    void Init()
    {
        static int counter = 0;
        m_index = counter;
        counter++;

    }

    AST<T>* m_value;
    int  m_index;
    std::vector<TreeOfTree> m_children;


    TreeOfTree* AddChild(AST<T>* _child)
    {
        m_children.push_back(TreeOfTree(_child));
        return &m_children[m_children.size() - 1];
    }

    void Print(std::string& _output, int level = 0)
    {
        for (int i = 0; i < level - 1; i++)
        {
            _output.append("|\t");
        }
        if (level > 0)
        {
            _output.append("|----");
            _output.append("-");
        }

        _output.append(std::to_string(m_index));

        _output.append("\n");
        for (auto i : m_children)
        {
            i.Print(_output, level + 1);
        }
    }
};

template<typename T>
void GetAllSubTrees(AST<T>* _node, std::vector<AST<T>*>& _treeList, TreeOfTree<T>* _treeMap)
{
    if (_node->m_connections.size())
    {
        _treeList.push_back(_node);
        _treeMap = _treeMap->AddChild(_node);

        for (auto i : _node->m_connections)
        {
            GetAllSubTrees(i, _treeList, _treeMap);
        }
    }
}

template<typename T>
void GetAllInputs(AST<T>* _node, std::map<AST<T>*, AST<T>*>& _treeList)
{
    if (_node->m_type == T_INPUT)
    {
        _treeList.insert(std::make_pair(_node, _node));
    }

    for (auto i : _node->m_connections)
    {
        GetAllInputs(i, _treeList);
    }
}

template < typename T >
class ASTWrapper
{
public:
    ASTWrapper(T _data, AST<T>* _node)
    {
        m_data = _data;
        m_node = _node;
    }

    ~ASTWrapper()
    {
        //TODO Re-enable and double check this...

        //m_node->DeRefChildren();
        //if (m_node->m_refCount <= 0)
        //	delete m_node;
    }

    ASTWrapper(T x, AST_TYPES inputFlag = T_VAL)
    {
        m_data = x;
        m_node = new AST<T>(inputFlag);
        m_node->m_result = x;
        m_node->m_resolved = true;
    }

    AST<T>* m_node;
    T m_data;

    //Hacky Functions
    ASTWrapper<T> round(ASTWrapper<T>& _v)
    {
        AST<T>* node = new AST<T>(T_ROUND);
        node->AddConnection(_v.m_node);

        T val = std::round(_v.m_data);
        return ASTWrapper<T>(val, node);
    }

    ASTWrapper<T> pow(ASTWrapper<T>& _v, ASTWrapper<T>& _p)
    {
        AST<T>* node = new AST<T>(T_POW);
        node->AddConnection(_v.m_node);
        node->AddConnection(_p.m_node);

        T val = std::pow(_v.m_data, _p.m_data);
        return ASTWrapper<T>(val, node);
    }

    //Operators overload
    ASTWrapper<T> operator+(const ASTWrapper<T>& _in)
    {
        AST<T>* node = new AST<T>(T_ADD);
        node->AddConnection(m_node);
        node->AddConnection(_in.m_node);

        T val = m_data + _in.m_data;
        return ASTWrapper<T>(val, node);
    }

    ASTWrapper<T> operator-(const ASTWrapper<T>& _in)
    {
        AST<T>* node = new AST<T>(T_SUB);
        node->AddConnection(m_node);
        node->AddConnection(_in.m_node);

        T val = m_data - _in.m_data;
        return ASTWrapper<T>(val, node);
    }

    ASTWrapper<T> operator*(const ASTWrapper<T>& _in)
    {
        AST<T>* node = new AST<T>(T_MUL);
        node->AddConnection(m_node);
        node->AddConnection(_in.m_node);

        T val = m_data * _in.m_data;
        return ASTWrapper<T>(val, node);
    }

    ASTWrapper<T> operator/(const ASTWrapper<T>& _in)
    {
        AST<T>* node = new AST<T>(T_DIV);
        node->AddConnection(m_node);
        node->AddConnection(_in.m_node);

        T val = m_data / _in.m_data;
        return ASTWrapper<T>(val, node);
    }

    const ASTWrapper<T>& operator=(const ASTWrapper<T>& _in)
    {
        //AST<T>* node = new AST<T>(T_EQA);
        //node->AddConnection(_in.m_node);
        //
        ////Cutting this out.
        //m_node->DeRefChildren();

        m_node = _in.m_node;
        m_data = _in.m_data;
        return *this;
    }

    bool operator<=(const ASTWrapper<T>& _in)
    {
        if (m_data <= _in.m_data)
            return true;

        return false;
    }

    //Wrapper<T>& operator++()
    //{
    //	AST* node = new AST(T_ADD);
    //	node->AddConnection(m_node);
    //	node->AddConnection(_in.m_node);
    //
    //	T val = m_data / _in.m_data;
    //	return Wrapper<T>(val, node);
    //	return *this;
    //}
    //
    //Wrapper<T> operator++(const Wrapper<T>& _in)
    //{
    //	m_data++;
    //	return *this;
    //}
};
