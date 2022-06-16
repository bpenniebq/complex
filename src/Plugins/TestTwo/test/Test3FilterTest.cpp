#include <catch2/catch.hpp>

#include "TestTwo/Filters/Test3Filter.hpp"

#include "complex/Common/Numbers.hpp"
#include "complex/Parameters/ArraySelectionParameter.hpp"
#include "complex/Parameters/ArrayCreationParameter.hpp"
#include "complex/Parameters/ChoicesParameter.hpp"
#include "complex/Parameters/NumberParameter.hpp"
#include "complex/UnitTest/UnitTestCommon.hpp"

using namespace complex;

namespace
{
const std::string k_testInputArray = "TestInputArray";
const std::string k_testOutputArray = "TestOutputArray";

const int k_InputValue = 1;

const DataPath k_InputPath({k_testInputArray});
const DataPath k_OutputPath({k_testOutputArray});

const std::vector<size_t> k_TupleShape = {10};
const std::vector<size_t> k_ComponentShape = {1};
}

void checkResults(std::vector<size_t> tupleShape, std::vector<size_t> componentShape, Int32Array* outputArray, std::vector<int32> comparisonData)
{
  for(size_t t = 0; t < tupleShape[0]; t++)
  {
    for(size_t c = 0; c < componentShape[0]; c++)
    {
      REQUIRE((*outputArray)[t * componentShape[0] + c] == comparisonData[t * componentShape[0] + c]);
    }
  }
}

TEST_CASE("Test3Filter: Instantiation and Parameter Check", "[Test3Filter]")
{
  // Instantiate the filter, a DataStructure object, and an Arguments Object
  Test3Filter filter;
  DataStructure ds;
  Arguments args;

  // Create default Parameters for the filter.
  // This should fail
  args.insertOrAssign(Test3Filter::k_OperatorType_Key, std::make_any<ChoicesParameter::ValueType>(0));
  args.insertOrAssign(Test3Filter::k_NumParameter_Key, std::make_any<Int32Parameter::ValueType>(k_InputValue));
  args.insertOrAssign(Test3Filter::k_InputArrayPath_Key, std::make_any<DataPath>(k_InputPath));
  args.insertOrAssign(Test3Filter::k_OutputArray_Key, std::make_any<DataPath>(k_OutputPath));

  // Preflight the filter and check result
  auto preflightResult = filter.preflight(ds, args);
  REQUIRE(preflightResult.outputActions.invalid());

  // This should fail because the parameter is out of range
  args.insertOrAssign(Test3Filter::k_OperatorType_Key, std::make_any<ChoicesParameter::ValueType>(4)); 
  preflightResult = filter.preflight(ds, args);
  REQUIRE(preflightResult.outputActions.invalid());

  // This should fail because of division by 0
  //args.insertOrAssign(Test3Filter::k_OperatorType_Key, std::make_any<ChoicesParameter::ValueType>(3));
  args.insertOrAssign(Test3Filter::k_NumParameter_Key, std::make_any<Int32Parameter::ValueType>(0));
  preflightResult = filter.preflight(ds, args);
  REQUIRE(preflightResult.outputActions.invalid());
}

TEST_CASE("Test3Filter: Addition Check", "[Test3Filter]") 
{
  // Instantiate the filter, a DataStructure object, and an Arguments Object
  Test3Filter filter;
  DataStructure ds;
  Arguments args;

  Int32Array* input = UnitTest::CreateTestDataArray<int>(ds, k_testInputArray, k_TupleShape, k_ComponentShape);

  std::vector<int32> comparisonData(10);

  for(size_t i = 0; i < 10; i++)
  {
    (*input)[i] = i;
    comparisonData[i] = i + k_InputValue;
  }

  // Create default Parameters for the filter
  args.insertOrAssign(Test3Filter::k_OperatorType_Key, std::make_any<ChoicesParameter::ValueType>(0));
  args.insertOrAssign(Test3Filter::k_NumParameter_Key, std::make_any<Int32Parameter::ValueType>(k_InputValue));
  args.insertOrAssign(Test3Filter::k_InputArrayPath_Key, std::make_any<DataPath>(k_InputPath));
  args.insertOrAssign(Test3Filter::k_OutputArray_Key, std::make_any<DataPath>(k_OutputPath));

  // Preflight the filter and check results
  auto preflightResult = filter.preflight(ds, args);
  REQUIRE(preflightResult.outputActions.valid());

  // Execute the filter and check the results
  auto executeResult = filter.execute(ds, args);
  REQUIRE(executeResult.result.valid());
  
  Int32Array* outputArray = ds.getDataAs<Int32Array>(k_OutputPath);
  REQUIRE(outputArray != nullptr);

  // Int32Array& foo = ds.getDataRefAs<Int32Array>(k_OutputPath);
  // REQUIRE(foo[t * componentShape[0] + c] == comparisonData[t * componentShape[0] + c]);
  
  // Check the results
  checkResults(k_TupleShape, k_ComponentShape, outputArray, comparisonData);
}

TEST_CASE("Test3Filter: Subtraction Check", "[Test3Filter]")
{
  // Instantiate the filter, a DataStructure object, and an Arguments Object
  Test3Filter filter;
  DataStructure ds;
  Arguments args;

  Int32Array* input = UnitTest::CreateTestDataArray<int>(ds, k_testInputArray, k_TupleShape, k_ComponentShape);

  std::vector<int32> comparisonData(10);

  for(size_t i = 0; i < 10; i++)
  {
    (*input)[i] = i;
    comparisonData[i] = i - k_InputValue;
  }

  // Create default Parameters for the filter
  args.insertOrAssign(Test3Filter::k_OperatorType_Key, std::make_any<ChoicesParameter::ValueType>(1));
  args.insertOrAssign(Test3Filter::k_NumParameter_Key, std::make_any<Int32Parameter::ValueType>(k_InputValue));
  args.insertOrAssign(Test3Filter::k_InputArrayPath_Key, std::make_any<DataPath>(k_InputPath));
  args.insertOrAssign(Test3Filter::k_OutputArray_Key, std::make_any<DataPath>(k_OutputPath));

  // Preflight the filter and check results
  auto preflightResult = filter.preflight(ds, args);
  REQUIRE(preflightResult.outputActions.valid());

  // Execute the filter and check the results
  auto executeResult = filter.execute(ds, args);
  REQUIRE(executeResult.result.valid());
  
  Int32Array* outputArray = ds.getDataAs<Int32Array>(k_OutputPath);
  REQUIRE(outputArray != nullptr);

  // Check the results
  checkResults(k_TupleShape, k_ComponentShape, outputArray, comparisonData);
}

TEST_CASE("Test3Filter: Multiplication Check", "[Test3Filter]")
{
  // Instantiate the filter, a DataStructure object, and an Arguments Object
  Test3Filter filter;
  DataStructure ds;
  Arguments args;;

  Int32Array* input = UnitTest::CreateTestDataArray<int>(ds, k_testInputArray, k_TupleShape, k_ComponentShape);

  std::vector<int32> comparisonData(10);

  for(size_t i = 0; i < 10; i++)
  {
    (*input)[i] = i;
    comparisonData[i] = i * k_InputValue;
  }

  // Create default Parameters for the filter
  args.insertOrAssign(Test3Filter::k_OperatorType_Key, std::make_any<ChoicesParameter::ValueType>(2));
  args.insertOrAssign(Test3Filter::k_NumParameter_Key, std::make_any<Int32Parameter::ValueType>(k_InputValue));
  args.insertOrAssign(Test3Filter::k_InputArrayPath_Key, std::make_any<DataPath>(k_InputPath));
  args.insertOrAssign(Test3Filter::k_OutputArray_Key, std::make_any<DataPath>(k_OutputPath));

  // Preflight the filter and check results
  auto preflightResult = filter.preflight(ds, args);
  REQUIRE(preflightResult.outputActions.valid());

  // Execute the filter and check the results
  auto executeResult = filter.execute(ds, args);
  REQUIRE(executeResult.result.valid());
  
  // Check if outputArray is a nullpointer
  Int32Array* outputArray = ds.getDataAs<Int32Array>(k_OutputPath);
  REQUIRE(outputArray != nullptr);

  // Check the results
  checkResults(k_TupleShape, k_ComponentShape, outputArray, comparisonData);
}

TEST_CASE("Test3Filter: Division Check", "[Test3Filter]")
{
   // Instantiate the filter, a DataStructure object, and an Arguments Object
  Test3Filter filter;
  DataStructure ds;
  Arguments args;

  Int32Array* input = UnitTest::CreateTestDataArray<int>(ds, k_testInputArray, k_TupleShape, k_ComponentShape);

  std::vector<int32> comparisonData(10);

  for(size_t i = 0; i < 10; i++)
  {
    (*input)[i] = i;
    comparisonData[i] = i / k_InputValue;
  }

  // Create default Parameters for the filter
  args.insertOrAssign(Test3Filter::k_OperatorType_Key, std::make_any<ChoicesParameter::ValueType>(3));
  args.insertOrAssign(Test3Filter::k_NumParameter_Key, std::make_any<Int32Parameter::ValueType>(k_InputValue));
  args.insertOrAssign(Test3Filter::k_InputArrayPath_Key, std::make_any<DataPath>(k_InputPath));
  args.insertOrAssign(Test3Filter::k_OutputArray_Key, std::make_any<DataPath>(k_OutputPath));

  // Preflight the filter and check results
  auto preflightResult = filter.preflight(ds, args);
  REQUIRE(preflightResult.outputActions.valid());

  // Execute the filter and check the results
  auto executeResult = filter.execute(ds, args);
  REQUIRE(executeResult.result.valid());
  
  Int32Array* outputArray = ds.getDataAs<Int32Array>(k_OutputPath);
  REQUIRE(outputArray != nullptr);
  
  // Int32Array& foo = ds.getDataRefAs<Int32Array>(k_OutputPath);

  // Check the results
  checkResults(k_TupleShape, k_ComponentShape, outputArray, comparisonData);
}