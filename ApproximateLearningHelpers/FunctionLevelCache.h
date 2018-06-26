#pragma once

#include <map>
#include <string>
#include <array>
#include <type_traits>

//hard-coded test
struct FunctionInformation_IntInt_Int
{
	FunctionInformation_IntInt_Int() : inputA(0), inputB(0), returnVal(0){}
	int inputA;
	int inputB;
	int returnVal;
};

template < typename T >
class InputVariableTemplateA
{
public:
	T m_inputVariableA;
};

template < typename T >
class InputVariableTemplateB
{
public:
	T m_inputVariableB;
};

template < typename T >
class InputVariableTemplateC
{
public:
	T m_inputVariableC;
};

template < typename T >
class InputVariableTemplateD
{
public:
	T m_inputVariableD;
};

template < typename T >
class InputVariableTemplateE
{
public:
	T m_inputVariableE;
};


// End of Test
/////////////////////////////////////////////////////////////


class ZeroSizeClass
{
public:
    ZeroSizeClass() {}
    template<typename T> ZeroSizeClass(T _0) {}
};



// On compile created class.
// Allows for the creation and storage of input variables to a function.

// TODO: Template for inputs A B ... should be changed into a variadic template.
template	<	typename R, 
				typename A = ZeroSizeClass, typename B = ZeroSizeClass, 
				typename C = ZeroSizeClass, typename D = ZeroSizeClass, 
				typename E = ZeroSizeClass  >
class FunctionInformation
	:	std::conditional< std::is_same<A, ZeroSizeClass>::value, InputVariableTemplateA<ZeroSizeClass>, InputVariableTemplateA<A> >::type,
		std::conditional< std::is_same<B, ZeroSizeClass>::value, InputVariableTemplateB<ZeroSizeClass>, InputVariableTemplateB<B> >::type,
		std::conditional< std::is_same<C, ZeroSizeClass>::value, InputVariableTemplateC<ZeroSizeClass>, InputVariableTemplateC<C> >::type,
		std::conditional< std::is_same<D, ZeroSizeClass>::value, InputVariableTemplateD<ZeroSizeClass>, InputVariableTemplateD<D> >::type,
		std::conditional< std::is_same<E, ZeroSizeClass>::value, InputVariableTemplateE<ZeroSizeClass>, InputVariableTemplateE<E> >::type
{
public:
	FunctionInformation() {}

    // This could be changed to a variadic or have multiple implementations for each input count, but for now this is enough.
	void Set(	A _a = 0,
				B _b = 0,	
				C _c = 0,
				D _d = 0,	
				E _e = 0
	)
	{
		if (!std::is_same<A, ZeroSizeClass>::value)
			m_inputVariableA = _a;
		if (!std::is_same<B, ZeroSizeClass>::value)
			m_inputVariableB = _b;
		if (!std::is_same<C, ZeroSizeClass>::value)
			m_inputVariableC = _c;
		if (!std::is_same<D, ZeroSizeClass>::value)
			m_inputVariableD = _d;
		if (!std::is_same<E, ZeroSizeClass>::value)
			m_inputVariableE = _e;
	}

	R _returnValue;
};


// Get the location of each position to check.
// Should be integrated into FunctionInformation Class so it is only calculated once.
template<typename... Types>
constexpr auto GetTypesOffsets() 
{
	auto typeSizes	= std::array<std::size_t, sizeof...(Types)>{sizeof(Types)...};
	auto offsets	= std::array<std::size_t, sizeof...(Types)>();

	int offset = 0;
	offsets[0] = 0;
	for (int index = 1; index < sizeof...(Types); index++)
	{
		offsets[index] = offset + typeSizes[index - 1];
	}

	return offsets;
}

// Returns the value from variadic
template<int N, typename T, T ... Ts>
struct GetN 
{
	static T values[sizeof...(Ts)] = { Ts... };
	static const T value = values[N];
};

template<int N, typename T, T ... Ts>
T GetN<N, T, Ts...>::values[sizeof...(Ts)];

template<typename T>
bool CheckValid(const T& inputValue, const T& referenceValue, const double& percentage)
{
	const double min = (double)(referenceValue)-(double)(referenceValue*(1.0 - percentage));
	const double max = (double)(referenceValue)+(double)(referenceValue*(1.0 - percentage));
	if (inputValue < min || inputValue > max)
	{
		return false;
	}
	return true;
}

//Gets the member variable from pointer.
template<typename T, typename ret, typename argV>
const argV MemberVariablePointer(const char* voidFi, const int& _index, const T& _offsets)
{
	const char* dataPointer = voidFi + _offsets[_index];// +sizeof(ret);
	const argV* d = (const argV*)dataPointer;
	return *d;
}



//Hard-coded Approximate Memoise
int ApproximateMemoise_Addition(int _A, int _B)
{
	//Approximate Information
	static std::array< FunctionInformation_IntInt_Int, 5 > globalFunctionResultCache;
	int allowedOffset_A = 2;
	int allowedOffset_B = 2;
	static int currentIndex = 0;

	//Check if there is a stored close enough value
	for (auto fi : globalFunctionResultCache)
	{
		bool valid_a = abs(_A - fi.inputA) < allowedOffset_A;
		bool valid_b = abs(_B - fi.inputB) < allowedOffset_B;
		if (valid_a && valid_b)
		{
			//We have a match in the cache.
			return fi.returnVal;
		}
	}

	//No match found.
	//Do work and store cache result

	int result = _A + _B;

	//Fill cache entry.
	FunctionInformation_IntInt_Int funcInfo;
	funcInfo.inputA = _A;
	funcInfo.inputB = _B;
	funcInfo.returnVal = result;

	globalFunctionResultCache[currentIndex] = std::move(funcInfo);
	currentIndex++;

	return result;

}