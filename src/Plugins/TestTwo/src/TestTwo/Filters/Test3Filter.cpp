#include "Test3Filter.hpp"

#include "complex/Common/StringLiteral.hpp"
#include "complex/DataStructure/DataArray.hpp"
#include "complex/Parameters/ChoicesParameter.hpp"
#include "complex/Parameters/NumberParameter.hpp"
#include "complex/Parameters/NumericTypeParameter.hpp"
#include "complex/Parameters/StringParameter.hpp"
#include "complex/Parameters/ArrayCreationParameter.hpp"
#include "complex/Parameters/ArraySelectionParameter.hpp"
#include "complex/Parameters/DataPathSelectionParameter.hpp"
#include "complex/Filter/Actions/CreateArrayAction.hpp"
#include "complex/Utilities/DataArrayUtilities.hpp"
#include "complex/Common/TypesUtility.hpp"

using namespace complex;

namespace
{

constexpr int32 k_MissingInputArray = -123;

// Addition operator
template <typename T>
void performOperationPlus(const DataArray<T>& input, IDataArray* output, T value)
{
  /*
  int i = 10;
  int* ptr = &i;
  int j = *ptr;

  int array[10];
  int array_4 = array[4];
  int* arrayPtr = &i;
  array_4 = arrayPtr[4];
  */

  DataArray<T>& outputRef = dynamic_cast<DataArray<T>&>(*output);
  size_t numValues = input.getNumberOfTuples();
  for(size_t i = 0; i < numValues; ++i)
  {
    outputRef[i] = input[i] + value;
    // (*output)[i] = input[i] + value;
  }
}

// Subtraction operator
template <typename T>
void performOperationMinus(const DataArray<T>& input, IDataArray* output, T value)
{
  DataArray<T>& outputRef = dynamic_cast<DataArray<T>&>(*output);
  size_t numValues = input.getNumberOfTuples();
  for(size_t i = 0; i < numValues; ++i)
  {
    outputRef[i] = input[i] - value;
  }
}

// Multiplication operator
template <typename T>
void performOperationMultiply(const DataArray<T>& input, IDataArray* output, T value)
{
  DataArray<T>& outputRef = dynamic_cast<DataArray<T>&>(*output);
  size_t numValues = input.getNumberOfTuples();
  for(size_t i = 0; i < numValues; ++i)
  {
    outputRef[i] = input[i] * value;
  }
}

// Division operator
template <typename T>
void performOperationDivide(const DataArray<T>& input, IDataArray* output, T value)
{
  DataArray<T>& outputRef = dynamic_cast<DataArray<T>&>(*output);
  size_t numValues = input.getNumberOfTuples();
  for(size_t i = 0; i < numValues; ++i)
  {
    outputRef[i] = input[i] / value; 
  }
}

template <typename T>
void switchOverOperator(const DataArray<T>& input, IDataArray* output, Test3Filter::OperatorType chosenOperator, int32 value)
{
  switch(chosenOperator)
  {
  case Test3Filter::OperatorType::Plus:
    performOperationPlus(input, output, static_cast<T>(value));
    break;
  case Test3Filter::OperatorType::Minus:
    performOperationMinus(input, output, static_cast<T>(value));
    break;
  case Test3Filter::OperatorType::Multiply:
    performOperationMultiply(input, output, static_cast<T>(value));
    break;
  case Test3Filter::OperatorType::Divide:
    performOperationDivide(input, output, static_cast<T>(value));
    break;
  }
}
} // namespace
// ------------------------------------------------------------------

Test3Filter::Test3Filter() = default;

Test3Filter::~Test3Filter() = default;

std::string Test3Filter::name() const
{
  return FilterTraits<Test3Filter>::name;
}

std::string Test3Filter::className() const
{
  return FilterTraits<Test3Filter>::className;
}

complex::Uuid Test3Filter::uuid() const
{
  return FilterTraits<Test3Filter>::uuid;
}

std::string Test3Filter::humanName() const
{
  return "Test Filter 3";
}

complex::Parameters Test3Filter::parameters() const
{
  // Parameters describe the input to the filter
  Parameters params;
  params.insertSeparator({"Parameters"});
  params.insert(std::make_unique<ChoicesParameter>(k_OperatorType_Key, "Choice Parameter", "Choice parameter for operator type", 0, ChoicesParameter::Choices{"+", "-", "*", "/"}));
  params.insert(std::make_unique<Int32Parameter>(k_NumParameter_Key, "Num Parameter", "Number Parameter for input array and operator to be performed against", 1));

  // Selected input array
  params.insertSeparator({"Input Array"});
  params.insert(std::make_unique<ArraySelectionParameter>(k_InputArrayPath_Key, "Input Array", "Input Array", DataPath{}, GetAllNumericTypes()));

  // Created (output) array 
  params.insertSeparator({"Output Array"});
  params.insert(std::make_unique<ArrayCreationParameter>(k_OutputArray_Key, "Output Array", "Output Array", DataPath{}));

  return params;
}

complex::IFilter::UniquePointer Test3Filter::clone() const
{
  return std::make_unique<Test3Filter>();
}

complex::IFilter::PreflightResult Test3Filter::preflightImpl(const DataStructure& data, const Arguments& args, const MessageHandler& messageHandler, const std::atomic_bool& shouldCancel) const
{
 // Preflight produces actions
  auto chosenOperator = static_cast<OperatorType>(args.value<ChoicesParameter::ValueType>(k_OperatorType_Key));
  auto chosenValue = args.value<Int32Parameter::ValueType>(k_NumParameter_Key);
  auto inputArrayPath = args.value<DataPath>(k_InputArrayPath_Key);
  auto* inputArray = data.getDataAs<IDataArray>(inputArrayPath);
  auto outputArrayPath = args.value<DataPath>(k_OutputArray_Key); 

  if (inputArray == nullptr)
  {
    return {MakeErrorResult<OutputActions>(-666001, fmt::format("Could not find input array at path {}", inputArrayPath.toString()))};
  }

  OutputActions actions;
  auto action = std::make_unique<CreateArrayAction>(inputArray->getDataType(), inputArray->getIDataStore()->getTupleShape(), inputArray->getIDataStore()->getComponentShape(), outputArrayPath);
  actions.actions.push_back(std::move(action));

  return {std::move(actions)};
}

complex::Result<> Test3Filter::executeImpl(DataStructure& data, const Arguments& args, const PipelineFilter* pipelineNode, const MessageHandler& messageHandler,
                                           const std::atomic_bool& shouldCancel) const
{
  // Result<> packages results, warnings, and error messages
  auto chosenOperator = static_cast<OperatorType>(args.value<ChoicesParameter::ValueType>(k_OperatorType_Key));
  auto chosenValue = args.value<Int32Parameter::ValueType>(k_NumParameter_Key);
  auto inputArrayPath = args.value<DataPath>(k_InputArrayPath_Key);
  auto outputArrayPath = args.value<DataPath>(k_OutputArray_Key);

  const auto& inputArray = data.getDataRefAs<IDataArray>(inputArrayPath);
  auto outputArray = data.getDataAs<IDataArray>(outputArrayPath);
  
  auto dataType = inputArray.getDataType();

  // Main algorithm/logic will go here:
  switch (dataType)
  {
  case DataType::int8:
    switchOverOperator<int8>(dynamic_cast<const DataArray<int8>&>(inputArray), outputArray, chosenOperator, chosenValue);
    break;
  case DataType::int16:
    switchOverOperator<int16>(dynamic_cast<const DataArray<int16>&>(inputArray), outputArray, chosenOperator, chosenValue);
    break;
  case DataType::int32:
    switchOverOperator<int32>(dynamic_cast<const DataArray<int32>&>(inputArray), outputArray, chosenOperator, chosenValue);
    break;
  case DataType::int64:
    switchOverOperator<int64>(dynamic_cast<const DataArray<int64>&>(inputArray), outputArray, chosenOperator, chosenValue);
    break;
  case DataType::uint8:
    switchOverOperator<uint8>(dynamic_cast<const DataArray<uint8>&>(inputArray), outputArray, chosenOperator, chosenValue);
    break;
  case DataType::uint16:
    switchOverOperator<uint16>(dynamic_cast<const DataArray<uint16>&>(inputArray), outputArray, chosenOperator, chosenValue);
    break;
  case DataType::uint32:
    switchOverOperator<uint32>(dynamic_cast<const DataArray<uint32>&>(inputArray), outputArray, chosenOperator, chosenValue);
    break;
  case DataType::uint64:
    switchOverOperator<uint64>(dynamic_cast<const DataArray<uint64>&>(inputArray), outputArray, chosenOperator, chosenValue);
    break;
  case DataType::float32:
    switchOverOperator<float32>(dynamic_cast<const DataArray<float32>&>(inputArray), outputArray, chosenOperator, chosenValue);
    break;
  case DataType::float64:
    switchOverOperator<float64>(dynamic_cast<const DataArray<float64>&>(inputArray), outputArray, chosenOperator, chosenValue);
    break;
  case DataType::boolean:
    //switchOverOperator<bool>(dynamic_cast<const DataArray<bool>&>(inputArray), outputArray, chosenOperator, chosenValue);
    break;
  }

  return {};
}