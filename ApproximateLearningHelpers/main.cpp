
//Simple High-Level Abstract Syntax Tree generator
#include "AST.h"
//Wrapper to measuring error propogation and controlling function calls.
#include "ApproximationWrappers.h"
//Templated colliding hash table
#include "CollidingHashTable.h"
//Trending Memoisation
#include "TrendingMemoisation.h"
//Functions for generating compile time lookup tables
#include "CompileTimeTables.h"
//Writing out data functions
#include "DataOutput.h"
//Error Measurement tools
#include "ErrorMeasurement.h"
//Float analysis tools
#include "FloatAnalysis.h"
//Wrapper to use a function-level cache of a function.
#include "FunctionLevelCacheMemoisation.h"
//Useful Preprocessor code
#include "Preprocessor.h"
//Error Classes
#include "Testing.h"
//Timer and timing wrappers
#include "Timing.h"
// Simple Vector Classes
#include "VectorClasses.h"
// Loop Perforation
#include "LoopPerforation.h"

#include "UnitTestingResources.h"

#include <algorithm>
#include <numeric>




int add(int _a, int _b)
{
    return _a + _b;
}

void ApproxCacheTest()
{

}
#define DIVISOR 2
#define HASHTABLESIZE 360/DIVISOR
#define TWOPI 6.28318530718
#define STEP TWOPI/HASHTABLESIZE

unsigned int DivHash(float _input)
{
    float res = 1.0f - ( (_input/float(DIVISOR)) / TWOPI);
    int index = res*(HASHTABLESIZE - 1);

    return std::max(0, std::min(index, HASHTABLESIZE));
}

unsigned int dydxSinHash(float _input)
{
    float maxOverStep = (STEP * 10);
    float res = (_input / TWOPI);
    res = res + ( std::abs(std::cos(_input)) * maxOverStep);
    res -= maxOverStep;
    int index = res*(HASHTABLESIZE - 1);
    std::cout << "Res:" << res << "\n";
    std::cout << "Index:" << index << "\n";
    return std::max(0 , std::min(index, HASHTABLESIZE-1) );
}

template<typename T>
float SinApprox(float _input, T& _table)
{
    float sinVal = 0.f;
    if (_table.Get( _input, sinVal ) == 1)
    {
        //std::cout << "sin(" << _input << ") =" << sinVal << " : Using existing values in table.\n";
        return sinVal;
    }
    else
    {
        sinVal = std::sin(_input);
        int res = _table.Insert(_input, sinVal);

#ifdef VERBOSE
        if (res == 0)
            std::cout << "sin(" << _input << ") =" << sinVal << " : Added to empty hash index.\n";
        if (res == 1)
            std::cout << "sin(" << _input << ") =" << sinVal << " : replaced what was at hash index.\n";
        if (res == 2)
            std::cout << "sin(" << _input << ") =" << sinVal << " : hash was occupied so did nothing.\n";
        if (res == -1)
            std::cout << "sin(" << _input << ") =" << sinVal << " : encountered an error in the hashing process.\n";
#endif
        return sinVal;
    }
}


constexpr int gRequiredFloatAccuracy = 10;


float FullAccuracyFunction( float _input)
{
	return _input + 0.4444444;
}

template<int AccuracyScore>
ApproxType<float, 4> ApproxFunction(ApproxType<float, AccuracyScore> _input)
{
	auto res = ApproxType<float, 4>(0.4444f) + _input;
	return res;
}

#define ALLOWAPPROXIMATION

float SimpleFunction(float inputValue)
{
	#ifdef ALLOWAPPROXIMATION
		// Approx Float with Zero Uncertainty
		ApproxType<float, 0>  inputValueApprox(inputValue); 

		// 'auto' type is ApproxType with updated uncertainty value.
		auto result = ApproxFunction(inputValueApprox);		

		// Compile-time check that the returned type is within the systems constraints.
		if constexpr (result.Accuracy() < gRequiredFloatAccuracy)
		{
			// The programmer has to explicitly remove the data from the
			// approximate wrapper to be able to use the results with 
			// non-approximated code.
			return result.Get();
		}
	#endif

	// If approximation isn't allowed, or the attached function is too inaccurate,
	// the standard function is used.
	return FullAccuracyFunction(inputValue);
}

int main()
{
	// Approx Types
	{
		float res = SimpleFunction(10.f);
	}
    // AST Test
    {
        ASTWrapper<float> a(4, T_INPUT);
        auto b = XCubed(a);
        std::string outputString;
        b.m_node->Print(outputString);
        std::cout << outputString << "\n";

        std::map<AST<float>*, AST<float>*> _inputNodes;
        GetAllInputs(a.m_node, _inputNodes);
        _inputNodes.begin()->first->Print(outputString);
        std::cout << outputString << "\n";

        std::vector<AST<float>*> _subTrees;
        TreeOfTree<float> _treeIndices;
        GetAllSubTrees(a.m_node, _subTrees, &_treeIndices);
        for (auto i : _subTrees)
        {
            i->Print(outputString);
            std::cout << outputString << "\n";
        }
    }

    //Trending Memoisation
    TestTheTrend();

    //Approximation Cache[Test]
    {    
        //TODO
        int ff0 = MemoWrapper(add, 80)(5, 5);   // returns 10
        int ff1 = MemoWrapper(add, 80)(2, 1);   // returns 3
        int ff5 = MemoWrapper(add, 80)(5, 33);  // returns 38
        int ff6 = MemoWrapper(add, 80)(5, 6);   // returns 10 because of ff0 being -
                                                // in the cache and within the approximate range (80%).
    }

    //Colliding Hash Table [Test]
    {
        //TODO
        CollidingHashTable< float, float, HASHTABLESIZE, dydxSinHash> WeightedHashTable;
        CollidingHashTable< float, float, HASHTABLESIZE, DivHash> LinearHashTable;
        std::vector<float> WeightedResults;
        std::vector<float> linearResults;
        std::vector<float> realResults;
        int counter = 0;
        for (float i = 0 ; counter < 360; i+= (TWOPI/360), counter++)
        {
            float res = SinApprox(i, WeightedHashTable);
            WeightedResults.push_back(res);

            res = SinApprox(i, LinearHashTable);
            linearResults.push_back(res);

            realResults.push_back(std::sin(i));
        }
        
        std::vector<float> diffWeight;
        std::vector<float> diffLinear;
        for (int i = 0; i < 360; i++)
        {
            diffWeight.push_back(std::abs(WeightedResults[i] - realResults[i]));
            diffLinear.push_back(std::abs(linearResults[i] - realResults[i]));
        }

        float totalError = std::accumulate(diffWeight.begin(), diffWeight.end(), 0.f);
        std::cout << "Total Error from weighted data = " << totalError << "\n";

        float linTotalError = std::accumulate(diffLinear.begin(), diffLinear.end(), 0.f);
        std::cout << "Total Error from linear data = " << linTotalError << "\n";

    }

    //Loop Perforation
    {

        //Goal is to reduce the compute operations.
        constexpr float input = 2;

        std::cout << "Unrolled-Loop Based Output is: " << ForLoopAccurate(input) << "\n";
        std::cout << "Loop Based Output is: " << UnrolledLoopAccurate(input) << "\n";
        std::cout << "\n\n";

        std::cout << "Loop Based Approximate Output is: " << HandEditedLoopApproximate(input) << "\n";
        std::cout << "Unrolled Loop Based Approximate Output is: " << HandEditedUnrolledApproximate(input) << "\n";
        std::cout << "Automated Approximate Output is: " << ConstExprApproximate(input) << "\n";

        int idxRight = ForLoopBinSearch(testList, 17);
        int idx0 = ForLoopBinSearchPerforated(testList, 17, 2);
        int idx1 = ForLoopBinSearchPerforated(testList, 17, 1);

        int testVal = 7;
        int idxTestRef = ForLoopBinSearch(testList, testVal);
        int idxtest0 = ForLoopBinSearchPerforated(testList, testVal, GetMaxStepSkip(testList.size(), 1, 4));
    }

    //Compile Time Table [Test]
    {
        //TODO
    }

    //Data Output [Test]
    {
        //TODO
    }

    //Error Measurement [Test]
    {
        //TODO
    }

    //Float Analysis [Test]
    {
        //TODO
    }

    //Function Level Cache Memoisation [Test]
    {
        //TODO
    }

    //Preprocessor [Test]
    {
        //TODO
    }

    //Testing [Test]
    {
        //TODO
    }

    //Timing [Test]
    {
        //TODO
    }

    //Vector Classes [Test]
    {
        //TODO
    }

    return 0;
}